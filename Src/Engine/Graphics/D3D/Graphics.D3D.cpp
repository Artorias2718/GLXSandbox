// Header Files
//=============

#include "../Graphics.h"
#include "../Structures/sVertex.h"
#include "Includes.h"
#include "Interfaces/D3DInterfaces.h"
#include "../Assets/cMesh.h"

#include <cstddef>
#include <cstdint>
#include "Includes.h"
#include "../../Asserts/Asserts.h"
#include "../Functions.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"
#include "../../Time/Time.h"

#include "../Interfaces/cConstantBuffer.h"
#include "../Structures/sFrame.h"
#include "../Structures/sDrawCall.h"

#include "../../Shared/cGameObject.h"

// Static Data Initialization
//===========================

namespace
{
	// This is the main window handle from Windows
	HWND s_renderingWindow = NULL;

	// The vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* Position
	//		(So that the graphics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want
	ID3D11VertexShader* s_vertexShader = NULL;
	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	ID3D11PixelShader* s_fragmentShader = NULL;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateConstantBuffers();
	bool CreateDevice(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
	bool CreateVertexBuffer(Engine::Platform::sDataFromFile& i_compiledShader);
	bool CreateViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
	bool LoadFragmentShader();
	bool LoadVertexShader(Engine::Platform::sDataFromFile& o_compiledShader);
}

// Interface
//==========

// Render
//-------

void Engine::Graphics::RenderFrame()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->ClearRenderTargetView(Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView, clearColor);
	}

	// Clear depth buffer
	{
		const float depthToClear = 1.0f;
		const uint8_t stencilToClear = 0;
		Interfaces::D3DInterfaces::s_direct3dImmediateContext->ClearDepthStencilView(Interfaces::D3DInterfaces::s_depthStencilView, D3D11_CLEAR_DEPTH, depthToClear, stencilToClear);
	}

	// Draw the geometry
	{
		// Set the vertex and fragment shaders
		{
			ID3D11ClassInstance** const noInterfaces = NULL;
			const unsigned int interfaceCount = 0;
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->VSSetShader(s_vertexShader, noInterfaces, interfaceCount);
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->PSSetShader(s_fragmentShader, noInterfaces, interfaceCount);
		}

		for (std::vector<Shared::cGameObject*>::iterator itor = meshObjects.begin(); itor != meshObjects.end(); ++itor)
		{
			Engine::Graphics::drawCallData.g_localToWorld = Engine::Math::cMatrix_Transformation((*itor)->m_transform);
			s_drawCallBuffer->Update(Engine::Graphics::Interfaces::DRAWCALL, &Engine::Graphics::drawCallData);

			(*itor)->m_mesh->Render();
		}
		meshObjects.clear();
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (to the front buffer)
	{
		const unsigned int swapImmediately = 0;
		const unsigned int presentNextFrame = 0;
		const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain->Present(swapImmediately, presentNextFrame);
		ASSERT(SUCCEEDED(result));
	}
}

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	bool wereThereErrors = false;

	s_renderingWindow = i_initializationParameters.mainWindow;
	Platform::sDataFromFile compiledVertexShader;

	// Create an interface to a Direct3D device
	if (!CreateDevice(i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight))
	{
		wereThereErrors = true;
		goto OnExit;
	}
	// Initialize the viewport of the device
	if (!CreateViews(i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight))
	{
		wereThereErrors = true;
		goto OnExit;
	}

	// Initialize the graphics objects
	if (!LoadVertexShader(compiledVertexShader))
	{
		wereThereErrors = true;
		goto OnExit;
	}
	if (!CreateVertexBuffer(compiledVertexShader))
	{
		wereThereErrors = true;
		goto OnExit;
	}
	if (!LoadFragmentShader())
	{
		wereThereErrors = true;
		goto OnExit;
	}
	if (!CreateConstantBuffers())
	{
		wereThereErrors = true;
		goto OnExit;
	}

OnExit:

	// A vertex shader object is used to render the triangle.
	// The compiled vertex shader is the actual compiled code,
	// and once it has been used to create the vertex input layout
	// it can be freed.
	if (&compiledVertexShader)
	{
		compiledVertexShader.Free();
	}

	return !wereThereErrors;
}

bool Engine::Graphics::CleanUp()
{
	bool wereThereErrors = false;

	if (Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice)
	{
		if (Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout)
		{
			Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout->Release();
			Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout = NULL;
		}

		if (s_vertexShader)
		{
			s_vertexShader->Release();
			s_vertexShader = NULL;
		}
		if (s_fragmentShader)
		{
			s_fragmentShader->Release();
			s_fragmentShader = NULL;
		}

		if (s_frameBuffer)
		{
			s_frameBuffer->CleanUp();
			s_frameBuffer = NULL;
		}

		if (s_drawCallBuffer)
		{
			s_drawCallBuffer->CleanUp();
			s_drawCallBuffer = NULL;
		}

		if (Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView)
		{
			Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView->Release();
			Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView = NULL;
		}

		Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->Release();
		Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice = NULL;
	}
	if (Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext)
	{
		Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->Release();
		Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext = NULL;
	}
	if (Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain)
	{
		Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain->Release();
		Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain = NULL;
	}

	s_renderingWindow = NULL;

	return !wereThereErrors;
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateConstantBuffers()
	{
		{
			D3D11_BUFFER_DESC bufferDescription = { 0 };
			D3D11_SUBRESOURCE_DATA initialData = { 0 };

			Engine::Graphics::s_frameBuffer = new Engine::Graphics::Interfaces::cConstantBuffer(
				Engine::Graphics::Interfaces::FRAME,
				bufferDescription,
				sizeof(Engine::Graphics::Structures::sFrame),
				initialData,
				&Engine::Graphics::frameData
			);
		}
		{
			D3D11_BUFFER_DESC bufferDescription = { 0 };
			D3D11_SUBRESOURCE_DATA initialData = { 0 };

			Engine::Graphics::s_drawCallBuffer = new Engine::Graphics::Interfaces::cConstantBuffer(
				Engine::Graphics::Interfaces::DRAWCALL,
				bufferDescription,
				sizeof(Engine::Graphics::Structures::sDrawCall),
				initialData,
				new Engine::Graphics::Structures::sDrawCall()
			);
		}

		Engine::Graphics::s_frameBuffer->Bind(Engine::Graphics::Interfaces::FRAME);
		Engine::Graphics::s_drawCallBuffer->Bind(Engine::Graphics::Interfaces::DRAWCALL);

		return true;
	}

	bool CreateDevice(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight)
	{
		IDXGIAdapter* const useDefaultAdapter = NULL;
		const D3D_DRIVER_TYPE useHardwareRendering = D3D_DRIVER_TYPE_HARDWARE;
		const HMODULE dontUseSoftwareRendering = NULL;
		unsigned int flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		{
#ifdef GRAPHICS_ISDEVICEDEBUGINFOENABLED
			flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		}
		D3D_FEATURE_LEVEL* const useDefaultFeatureLevels = NULL;
		const unsigned int requestedFeatureLevelCount = 0;
		const unsigned int sdkVersion = D3D11_SDK_VERSION;
		DXGI_SWAP_CHAIN_DESC swapChainDescription = { 0 };
		{
			{
				DXGI_MODE_DESC& bufferDescription = swapChainDescription.BufferDesc;

				bufferDescription.Width = i_resolutionWidth;
				bufferDescription.Height = i_resolutionHeight;
				{
					DXGI_RATIONAL& refreshRate = bufferDescription.RefreshRate;

					refreshRate.Numerator = 0;	// Refresh as fast as possible
					refreshRate.Denominator = 1;
				}
				bufferDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				bufferDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				bufferDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			}
			{
				DXGI_SAMPLE_DESC& multiSamplingDescription = swapChainDescription.SampleDesc;

				multiSamplingDescription.Count = 1;
				multiSamplingDescription.Quality = 0;	// Anti-aliasing is disabled
			}
			swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDescription.BufferCount = 1;
			swapChainDescription.OutputWindow = s_renderingWindow;
			swapChainDescription.Windowed = TRUE;
			swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDescription.Flags = 0;
		}
		D3D_FEATURE_LEVEL highestSupportedFeatureLevel;
		const HRESULT result = D3D11CreateDeviceAndSwapChain(useDefaultAdapter, useHardwareRendering, dontUseSoftwareRendering,
			flags, useDefaultFeatureLevels, requestedFeatureLevelCount, sdkVersion, &swapChainDescription,
			&Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain, &Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice, &highestSupportedFeatureLevel, &Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext);
		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			ASSERT(false);
			Engine::Logging::OutputError("Direct3D failed to create a Direct3D11 device with HRESULT %#010x", result);
			return false;
		}
	}

	bool CreateVertexBuffer(Engine::Platform::sDataFromFile& i_compiledShader)
	{
		// Create the vertex layout
		{
			// These elements must match the VertexFormat::sVertex layout struct exactly.
			// They instruct Direct3D how to match the binary data in the vertex buffer
			// to the input elements in a vertex shader
			// (by using so-called "semantic" names so that, for example,
			// "POSITION" here matches with "POSITION" in shader code).
			// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
			const unsigned int vertexElementCount = 2;
			D3D11_INPUT_ELEMENT_DESC layoutDescription[vertexElementCount] = { 0 };

			// Initialize the vertex format
			Engine::Graphics::Functions::CreateVertexFormat(layoutDescription);

			const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateInputLayout(layoutDescription, vertexElementCount,
				i_compiledShader.data, i_compiledShader.size, &Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout);
			if (FAILED(result))
			{
				ASSERT(false);
				Engine::Logging::OutputError("Direct3D failed to create a vertex input layout with HRESULT %#010x", result);
				return false;
			}
		}

		return true;
	}

	bool CreateViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight)
	{
		bool wereThereErrors = false;

		ID3D11Texture2D* backBuffer = NULL;
		ID3D11Texture2D* depthBuffer = NULL;

		// Create a "render target view" of the back buffer
		// (the back buffer was already created by the call to D3D11CreateDeviceAndSwapChain(),
		// but we need a "view" of it to use as a "render target",
		// meaning a texture that the GPU can render to)
		{
			// Get the back buffer from the swap chain
			{
				const unsigned int bufferIndex = 0; // This must be 0 since the swap chain is discarded
				const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain->GetBuffer(bufferIndex, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
				if (FAILED(result))
				{
					ASSERT(false);
					Engine::Logging::OutputError("Direct3D failed to get the back buffer from the swap chain with HRESULT %#010x", result);
					goto OnExit;
				}
			}
			// Create the view
			{
				const D3D11_RENDER_TARGET_VIEW_DESC* const accessAllSubResources = NULL;
				const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateRenderTargetView(backBuffer, accessAllSubResources, &Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView);
				if (FAILED(result))
				{
					ASSERT(false);
					Engine::Logging::OutputError("Direct3D failed to create the render target view with HRESULT %#010x", result);
					goto OnExit;
				}
			}
		}
		// Create a depth/stencil buffer and a view of it
		{
			// Unlike the back buffer no depth/stencil buffer exists until and unless we create it
			{
				D3D11_TEXTURE2D_DESC textureDescription = { 0 };
				{
					textureDescription.Width = i_resolutionWidth;
					textureDescription.Height = i_resolutionHeight;
					textureDescription.MipLevels = 1; // A depth buffer has no MIP maps
					textureDescription.ArraySize = 1;
					textureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24 bits for depth and 8 bits for stencil
					{
						DXGI_SAMPLE_DESC& sampleDescription = textureDescription.SampleDesc;
						sampleDescription.Count = 1; // No multisampling
						sampleDescription.Quality = 0; // Doesn't matter when Count is 1
					}
					textureDescription.Usage = D3D11_USAGE_DEFAULT; // Allows the GPU to write to it
					textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					textureDescription.CPUAccessFlags = 0; // CPU doesn't need access
					textureDescription.MiscFlags = 0;
				}
				// The GPU renders to the depth/stencil buffer and so there is no initial data
				// (like there would be with a traditional texture loaded from disk)
				const D3D11_SUBRESOURCE_DATA* const noInitialData = NULL;
				const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateTexture2D(&textureDescription, noInitialData, &depthBuffer);
				if (FAILED(result))
				{
					ASSERT(false);
					Engine::Logging::OutputError("Direct3D failed to create the depth buffer resource with HRESULT %#010x", result);
					goto OnExit;
				}
			}
			// Create the view
			{
				const D3D11_DEPTH_STENCIL_VIEW_DESC* const noSubResources = NULL;
				const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateDepthStencilView(depthBuffer, noSubResources, &Engine::Graphics::Interfaces::D3DInterfaces::s_depthStencilView);
				if (FAILED(result))
				{
					ASSERT(false);
					Engine::Logging::OutputError("Direct3D failed to create the depth stencil view with HRESULT %#010x", result);
					goto OnExit;
				}
			}
		}

		// Bind the views
		{
			const unsigned int renderTargetCount = 1;
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->OMSetRenderTargets(renderTargetCount, &Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView, Engine::Graphics::Interfaces::D3DInterfaces::s_depthStencilView);
		}
		// Specify that the entire render target should be visible
		{
			D3D11_VIEWPORT viewPort = { 0 };
			viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;
			viewPort.Width = static_cast<float>(i_resolutionWidth);
			viewPort.Height = static_cast<float>(i_resolutionHeight);
			viewPort.MinDepth = 0.0f;
			viewPort.MaxDepth = 1.0f;
			const unsigned int viewPortCount = 1;
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->RSSetViewports(viewPortCount, &viewPort);
		}

	OnExit:

		if (backBuffer)
		{
			backBuffer->Release();
			backBuffer = NULL;
		}
		if (depthBuffer)
		{
			depthBuffer->Release();
			depthBuffer = NULL;
		}

		return !wereThereErrors;
	}

	bool LoadFragmentShader()
	{
		bool wereThereErrors = false;

		// Load the compiled shader
		Engine::Platform::sDataFromFile compiledShader;
		const char* const path = "data/shaders/fragment.shader";
		{
			std::string errorMessage;
			if (!Engine::Platform::LoadBinaryFile(path, compiledShader, &errorMessage))
			{
				wereThereErrors = true;
				ASSERTF(false, errorMessage.c_str());
				Engine::Logging::OutputError("Failed to load the shader file \"%s\": %s", path, errorMessage.c_str());
				goto OnExit;
			}
		}
		// Create the shader object
		{
			ID3D11ClassLinkage* const noInterfaces = NULL;
			const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreatePixelShader(
				compiledShader.data, compiledShader.size, noInterfaces, &s_fragmentShader);
			if (FAILED(result))
			{
				wereThereErrors = true;
				ASSERT(false);
				Engine::Logging::OutputError("Direct3D failed to create the shader %s with HRESULT %#010x", path, result);
				goto OnExit;
			}
		}

	OnExit:

		compiledShader.Free();

		return !wereThereErrors;
	}

	bool LoadVertexShader(Engine::Platform::sDataFromFile& o_compiledShader)
	{
		bool wereThereErrors = false;

		const char* const path = "data/shaders/vertex.shader";
		// Load the source code and compile it
		{
			std::string errorMessage;
			if (!Engine::Platform::LoadBinaryFile(path, o_compiledShader, &errorMessage))
			{
				wereThereErrors = true;
				ASSERTF(false, errorMessage.c_str());
				Engine::Logging::OutputError("Failed to load the shader file \"%s\": %s", path, errorMessage.c_str());
				goto OnExit;
			}
		}
		// Create the shader object
		{
			ID3D11ClassLinkage* const noInterfaces = NULL;
			const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateVertexShader(
				o_compiledShader.data, o_compiledShader.size, noInterfaces, &s_vertexShader);
			if (FAILED(result))
			{
				wereThereErrors = true;
				ASSERT(false);
				Engine::Logging::OutputError("Direct3D failed to create the shader %s with HRESULT %#010x", path, result);
				goto OnExit;
			}
		}

	OnExit:
		return !wereThereErrors;
	}
}