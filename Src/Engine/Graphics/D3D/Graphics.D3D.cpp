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

	// This struct determines the layout of the constant data that the CPU will send to the GPU
	struct
	{
		union
		{
			float g_elapsedSeconds;
			float register0[4];	// You won't have to worry about why I do this until a later assignment
		};
	} s_constantBufferData;
	ID3D11Buffer* s_constantBuffer = NULL;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateConstantBuffer();
	bool CreateDevice(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
	bool CreateVertexBuffer(Engine::Platform::sDataFromFile& i_compiledShader);
	bool CreateView(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
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

	// Update the constant buffer
	{
		// Update the struct (i.e. the memory that we own)
		s_constantBufferData.g_elapsedSeconds = Time::ElapsedSeconds();
		// Get a pointer from Direct3D that can be written to
		void* memoryToWriteTo = NULL;
		{
			D3D11_MAPPED_SUBRESOURCE mappedSubResource;
			{
				// Discard previous contents when writing
				const unsigned int noSubResources = 0;
				const D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
				const unsigned int noFlags = 0;
				const HRESULT result = Interfaces::D3DInterfaces::s_direct3dImmediateContext->Map(s_constantBuffer, noSubResources, mapType, noFlags, &mappedSubResource);
				if (SUCCEEDED(result))
				{
					memoryToWriteTo = mappedSubResource.pData;
				}
				else
				{
					ASSERT(false);
				}
			}
		}
		if (memoryToWriteTo)
		{
			// Copy the new data to the memory that Direct3D has provided
			memcpy(memoryToWriteTo, &s_constantBufferData, sizeof(s_constantBufferData));
			// Let Direct3D know that the memory contains the data
			// (the pointer will be invalid after this call)
			const unsigned int noSubResources = 0;
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->Unmap(s_constantBuffer, noSubResources);
			memoryToWriteTo = NULL;
		}
		// Bind the constant buffer to the shader
		{
			const unsigned int registerAssignedInShader = 0;
			const unsigned int bufferCount = 1;
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->VSSetConstantBuffers(registerAssignedInShader, bufferCount, &s_constantBuffer);
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->PSSetConstantBuffers(registerAssignedInShader, bufferCount, &s_constantBuffer);
		}
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

		for (std::vector<Engine::Graphics::Assets::cMesh*>::iterator itor = meshes.begin(); itor != meshes.end(); ++itor)
		{
			(*itor)->Render();
		}
		meshes.clear();
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
	if (!CreateView(i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight))
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
	if (!CreateConstantBuffer())
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

		if (s_constantBuffer)
		{
			s_constantBuffer->Release();
			s_constantBuffer = NULL;
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
	bool CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC bufferDescription = { 0 };
		{
			// The byte width must be rounded up to a multiple of 16
			bufferDescription.ByteWidth = sizeof(s_constantBufferData);
			bufferDescription.Usage = D3D11_USAGE_DYNAMIC;	// The CPU must be able to update the buffer
			bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// The CPU must write, but doesn't read
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used
		}
		D3D11_SUBRESOURCE_DATA initialData = { 0 };
		{
			// Fill in the constant buffer
			s_constantBufferData.g_elapsedSeconds = Engine::Time::ElapsedSeconds();
			initialData.pSysMem = &s_constantBufferData;
			// (The other data members are ignored for non-texture buffers)
		}

		const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &s_constantBuffer);
		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			ASSERT(false);
			Engine::Logging::OutputError("Direct3D failed to create a constant buffer with HRESULT %#010x", result);
			return false;
		}
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

	bool CreateView(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight)
	{
		bool wereThereErrors = false;

		// Create and bind the render target view
		ID3D11Texture2D* backBuffer = NULL;
		{
			// Get the back buffer from the swap chain
			HRESULT result;
			{
				const unsigned int bufferIndex = 0;	// This must be 0 since the swap chain is discarded
				result = Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain->GetBuffer(bufferIndex, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
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
				result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateRenderTargetView(backBuffer, accessAllSubResources, &Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView);
			}
			if (SUCCEEDED(result))
			{
				// Bind it
				const unsigned int renderTargetCount = 1;
				ID3D11DepthStencilView* const noDepthStencilState = NULL;
				Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->OMSetRenderTargets(renderTargetCount, &Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView, noDepthStencilState);
			}
			else
			{
				ASSERT(false);
				Engine::Logging::OutputError("Direct3D failed to create the render target view with HRESULT %#010x", result);
				goto OnExit;
			}
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