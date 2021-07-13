// Header Files
//=============

#include "Graphics.h"

#include "../Asserts/Asserts.h"
#include "../Logging/Logging.h"

#include "Assets/cMesh.h"
#include "Assets/cMaterial.h"

#include "Interfaces/cConstantBuffer.h"
#include "Structures/sFrame.h"
#include "Structures/sDrawCall.h"

#include "../Shared/cCamera.h"
#include "../Time/Time.h"

#if defined D3D_API
#include "D3D/Includes.h"
#include "D3D/Interfaces/D3DInterfaces.h"
#elif defined OGL_API
#include "OGL/Includes.h"
#endif

#include "../Windows/Functions.h"
#include "../Math/Functions.h"

std::vector<Engine::Shared::cGameObject*> Engine::Graphics::meshObjects;
Engine::Graphics::Structures::sFrame Engine::Graphics::frameData;
Engine::Graphics::Structures::sDrawCall Engine::Graphics::drawCallData;

Engine::Graphics::Interfaces::cConstantBuffer* Engine::Graphics::s_frameBuffer = nullptr;
Engine::Graphics::Interfaces::cConstantBuffer* Engine::Graphics::s_drawCallBuffer = nullptr;

namespace
{
	// This is the main window handle from Windows
	HWND s_renderingWindow = NULL;

#if defined D3D_API
#elif defined OGL_API
	// These are Windows-specific interfaces
	HDC s_deviceContext = NULL;
	HGLRC s_openGlRenderingContext = NULL;
	GLuint s_samplerStateId = 0;
#endif
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateConstantBuffers();
	bool CreateSamplerState();
#if defined D3D_API
	bool CreateDevice(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
	bool CreateViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
#elif defined OGL_API
	bool CreateConstantBuffers();
	bool CreateRenderingContext();
	bool EnableDepthTesting();
#endif
}

// Interface
//==========

// Render
//-------

void Engine::Graphics::RenderFrame()
{
#if defined D3D_API
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Interfaces::D3DInterfaces::s_direct3dImmediateContext->ClearRenderTargetView(Interfaces::D3DInterfaces::s_renderTargetView, clearColor);
	}
	// Clear depth buffer
	{
		const float depthToClear = 1.0f;
		const uint8_t stencilToClear = 0;
		Interfaces::D3DInterfaces::s_direct3dImmediateContext->ClearDepthStencilView(Interfaces::D3DInterfaces::s_depthStencilView, D3D11_CLEAR_DEPTH, depthToClear, stencilToClear);
	}
#elif defined OGL_API
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		ASSERT(glGetError() == GL_NO_ERROR);
		glDepthMask(GL_TRUE);
		glClearDepth(1.0f);
		// In addition to the color, "depth" and "stencil" can also be cleared,
		// but for now we only care about color
		const GLbitfield clearColorAndDepth = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		glClear(clearColorAndDepth);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
#endif

	// Draw the geometry
	{
		for (std::vector<Shared::cGameObject*>::iterator itor = meshObjects.begin(); itor != meshObjects.end(); ++itor)
		{
			Engine::Graphics::drawCallData.g_localToWorld = Engine::Math::UpdateTransform((*itor)->m_transform);
			s_drawCallBuffer->Update(Interfaces::DRAWCALL, &Engine::Graphics::drawCallData);
			(*itor)->m_material->Bind();
			(*itor)->m_mesh->Render();
		}
		meshObjects.clear();
	}

#if defined D3D_API
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (to the front buffer)
	{
		const unsigned int swapImmediately = 0;
		const unsigned int presentNextFrame = 0;
		const HRESULT result = Interfaces::D3DInterfaces::s_swapChain->Present(swapImmediately, presentNextFrame);
		ASSERT(SUCCEEDED(result));
	}
#elif defined OGL_API
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be swapped with the "front buffer"
	// (which is what the user sees)
	{
		BOOL result = SwapBuffers(s_deviceContext);
		ASSERT(result != FALSE);
	}
#endif
}

// Initialization
//---------------

bool Engine::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	bool wereThereErrors = false;

	s_renderingWindow = i_initializationParameters.mainWindow;

#if defined D3D_API
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
#elif defined OGL_API
	std::string errorMessage;

	s_renderingWindow = i_initializationParameters.mainWindow;

	// Load any required OpenGL extensions
	if (!External::OGLExtensions::Load(&errorMessage))
	{
		ASSERTF(false, errorMessage.c_str());
		Logging::OutputError(errorMessage.c_str());
		return false;
	}
	// Create an OpenGL rendering context
	if (!CreateRenderingContext())
	{
		ASSERT(false);
		return false;
	}
	if (!EnableDepthTesting())
	{
		ASSERT(false);
		return false;
	}
#endif

	// Initialize the graphics objects
	if (!CreateConstantBuffers())
	{
		wereThereErrors = true;
		goto OnExit;
	}

	// Create a Texture Sampler State
	if (!CreateSamplerState())
	{
		wereThereErrors = true;
		goto OnExit;
	}

OnExit:

	return !wereThereErrors;
}

// Clean Up
//---------

bool Engine::Graphics::CleanUp()
{
	bool wereThereErrors = false;

#if defined D3D_API
	if (Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice)
	{
		if (Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout)
		{
			Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout->Release();
			Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout = NULL;
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

		if (Engine::Graphics::Interfaces::D3DInterfaces::s_depthStencilView)
		{
			Engine::Graphics::Interfaces::D3DInterfaces::s_depthStencilView->Release();
			Engine::Graphics::Interfaces::D3DInterfaces::s_depthStencilView = NULL;
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
#elif defined OGL_API
	if (s_openGlRenderingContext != NULL)
	{
		if (!s_frameBuffer->CleanUp())
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to delete the frame constant buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
		}

		if (!s_drawCallBuffer->CleanUp())
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to delete the drawcall constant buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
		}

		if (wglMakeCurrent(s_deviceContext, NULL) != FALSE)
		{
			if (wglDeleteContext(s_openGlRenderingContext) == FALSE)
			{
				wereThereErrors = true;
				const std::string windowsErrorMessage = Windows::Functions::GetLastSystemError();
				ASSERTF(false, windowsErrorMessage.c_str());
				Logging::OutputError("Windows failed to delete the OpenGL rendering context: %s", windowsErrorMessage.c_str());
			}
		}
		else
		{
			wereThereErrors = true;
			const std::string windowsErrorMessage = Windows::Functions::GetLastSystemError();
			ASSERTF(false, windowsErrorMessage.c_str());
			Logging::OutputError("Windows failed to unset the current OpenGL rendering context: %s", windowsErrorMessage.c_str());
		}
		s_openGlRenderingContext = NULL;
	}

	if (s_deviceContext != NULL)
	{
		// The documentation says that this call isn't necessary when CS_OWNDC is used
		ReleaseDC(s_renderingWindow, s_deviceContext);
		s_deviceContext = NULL;
	}
#endif

	s_renderingWindow = NULL;

	return !wereThereErrors;
}

bool Engine::Graphics::SubmitGameObject(Engine::Shared::cGameObject* i_object)
{
	if (i_object->m_mesh)
	{
		meshObjects.push_back(i_object);
		return true;
	}
	else if (dynamic_cast<Engine::Shared::cCamera*>(i_object))
	{
		Engine::Shared::cCamera* i_camera = dynamic_cast<Engine::Shared::cCamera*>(i_object);

		frameData.g_worldToCamera = i_camera->WorldToCamera();
		frameData.g_cameraToScreen = i_camera->CameraToScreen();
		frameData.g_elapsedSeconds = Engine::Time::ElapsedSeconds();
		s_frameBuffer->Update(Interfaces::eConstantBufferType::FRAME, &frameData);
		i_camera->Update();
		return true;
	}
	else
	{
		Engine::Logging::OutputError("Object mesh is uninitialized!");
		return false;
	}
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateConstantBuffers()
	{
		bool wereThereErrors = false;

#if defined D3D_API
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
#elif defined OGL_API
		Engine::Graphics::s_frameBuffer = new Engine::Graphics::Interfaces::cConstantBuffer(
			Engine::Graphics::Interfaces::FRAME,
			sizeof(Engine::Graphics::Structures::sFrame),
			&Engine::Graphics::frameData
		);

		Engine::Graphics::s_drawCallBuffer = new Engine::Graphics::Interfaces::cConstantBuffer(
			Engine::Graphics::Interfaces::DRAWCALL,
			sizeof(Engine::Graphics::Structures::sDrawCall),
			new Engine::Graphics::Structures::sDrawCall()
		);
#endif

		Engine::Graphics::s_frameBuffer->Bind(Engine::Graphics::Interfaces::FRAME);
		Engine::Graphics::s_drawCallBuffer->Bind(Engine::Graphics::Interfaces::DRAWCALL);

		return !wereThereErrors;
	}

	bool CreateSamplerState()
	{
#if defined D3D_API 
		// Create a sampler state object
		{
			D3D11_SAMPLER_DESC samplerStateDescription;
			{
				// Linear filtering
				samplerStateDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				// If UVs go outside [0,1] wrap them around (so that textures can tile)
				samplerStateDescription.AddressU = samplerStateDescription.AddressV
					= samplerStateDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				// Default values
				samplerStateDescription.MipLODBias = 0.0f;
				samplerStateDescription.MaxAnisotropy = 1;
				samplerStateDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
				samplerStateDescription.BorderColor[0] = samplerStateDescription.BorderColor[1]
					= samplerStateDescription.BorderColor[2] = samplerStateDescription.BorderColor[3] = 1.0f;
				samplerStateDescription.MinLOD = -FLT_MAX;
				samplerStateDescription.MaxLOD = FLT_MAX;
			}
			const HRESULT result = Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice->CreateSamplerState(&samplerStateDescription, &Engine::Graphics::Interfaces::D3DInterfaces::s_samplerState);
			if (FAILED(result))
			{
				ASSERT(false);
				Engine::Logging::OutputError("Direct3D failed to create a sampler state with HRESULT %#010x", result);
				return false;
			}
		}
		// Bind the sampler state
		{
			const unsigned int samplerStateRegister = 0; // This must match the SamplerState register defined in the shader
			const unsigned int samplerStateCount = 1;
			Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext->PSSetSamplers(samplerStateRegister, samplerStateCount, &Engine::Graphics::Interfaces::D3DInterfaces::s_samplerState);
		}
#elif defined OGL_API 
		// Create a sampler state object
		{
			const GLsizei samplerStateCount = 1;
			glGenSamplers(samplerStateCount, &s_samplerStateId);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				if (s_samplerStateId != 0)
				{
					// Linear Filtering
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					ASSERT(glGetError() == GL_NO_ERROR);
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					ASSERT(glGetError() == GL_NO_ERROR);
					// If UVs go outside [0,1] wrap them around (so that textures can tile)
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_WRAP_S, GL_REPEAT);
					ASSERT(glGetError() == GL_NO_ERROR);
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_WRAP_T, GL_REPEAT);
					ASSERT(glGetError() == GL_NO_ERROR);
				}
				else
				{
					ASSERT(false);
					Engine::Logging::OutputError("OpenGL failed to create a sampler state");
					return false;
				}
			}
			else
			{
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to create a sampler state: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return false;
			}
		}
		// Bind the sampler state
		{
			// We will never be required to use more than one texture in an Effect in this class,
			// but it doesn't hurt to bind the state to a few extra texture units
			// just in case you decide to try using more
			const GLuint maxTextureUnitCountYouThinkYoullUse = 5;
			for (GLuint i = 0; i < maxTextureUnitCountYouThinkYoullUse; ++i)
			{
				glBindSampler(i, s_samplerStateId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to bind the sampler state to texture unit %u: %s",
						i, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return false;
				}
			}
		}
#endif
		return true;
	}

#if defined D3D_API
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
#elif defined OGL_API
	bool CreateRenderingContext()
	{
		// Get the device context
		{
			s_deviceContext = GetDC(s_renderingWindow);
			if (s_deviceContext == NULL)
			{
				ASSERT(false);
				Engine::Logging::OutputError("Windows failed to get the device context");
				return false;
			}
		}
		// Set the pixel format for the window
		// (This can only be done _once_ for a given window)
		{
			// Get the ID of the desired pixel format
			int pixelFormatId;
			{
				// Create a key/value list of attributes that the pixel format should have
				const int desiredAttributes[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
					WGL_COLOR_BITS_ARB, 24,
					WGL_RED_BITS_ARB, 8,
					WGL_GREEN_BITS_ARB, 8,
					WGL_BLUE_BITS_ARB, 8,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_STENCIL_BITS_ARB, 8,
					// NULL terminator
					NULL
				};
				const float* const noFloatAttributes = NULL;
				const unsigned int onlyReturnBestMatch = 1;
				unsigned int returnedFormatCount;
				if (wglChoosePixelFormatARB(s_deviceContext, desiredAttributes, noFloatAttributes, onlyReturnBestMatch,
					&pixelFormatId, &returnedFormatCount) != FALSE)
				{
					if (returnedFormatCount == 0)
					{
						ASSERT(false);
						Engine::Logging::OutputError("Windows couldn't find a pixel format that satisfied the desired attributes");
						return false;
					}
				}
				else
				{
					const std::string windowsErrorMessage = Engine::Windows::Functions::GetLastSystemError();
					ASSERTF(false, windowsErrorMessage.c_str());
					Engine::Logging::OutputError("Windows failed to choose the closest pixel format: %s", windowsErrorMessage.c_str());
					return false;
				}
			}
			// Set it
			{
				PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };
				{
					// I think that the values of this struct are ignored
					// and unnecessary when using wglChoosePixelFormatARB() instead of ChoosePixelFormat(),
					// but the documentation is very unclear and so filling it in seems the safest bet
					pixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
					pixelFormatDescriptor.nVersion = 1;
					pixelFormatDescriptor.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
					pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
					pixelFormatDescriptor.cColorBits = 24;
					pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;
					pixelFormatDescriptor.cDepthBits = 24;
					pixelFormatDescriptor.cStencilBits = 8;
				}
				if (SetPixelFormat(s_deviceContext, pixelFormatId, &pixelFormatDescriptor) == FALSE)
				{
					const std::string windowsErrorMessage = Engine::Windows::Functions::GetLastSystemError();
					ASSERTF(false, windowsErrorMessage.c_str());
					Engine::Logging::OutputError("Windows couldn't set the desired pixel format: %s", windowsErrorMessage.c_str());
					return false;
				}
			}
		}
		// Create an OpenGL rendering context and make it current
		{
			// Create the context
			{
				// Create a key/value list of attributes that the context should have
				const int desiredAttributes[] =
				{
					// Request at least version 4.2
					WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
					WGL_CONTEXT_MINOR_VERSION_ARB, 2,
					// Request only "core" functionality and not "compatibility"
					// (i.e. only use modern features of version 4.2)
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef GRAPHICS_ISDEVICEDEBUGINFOENABLED
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
					// NULL terminator
					NULL
				};
				const HGLRC noSharedContexts = NULL;
				s_openGlRenderingContext = wglCreateContextAttribsARB(s_deviceContext, noSharedContexts, desiredAttributes);
				if (s_openGlRenderingContext == NULL)
				{
					DWORD errorCode;
					const std::string windowsErrorMessage = Engine::Windows::Functions::GetLastSystemError(&errorCode);
					std::ostringstream errorMessage;
					errorMessage << "Windows failed to create an OpenGL rendering context: ";
					if ((errorCode == ERROR_INVALID_VERSION_ARB)
						|| (HRESULT_CODE(errorCode) == ERROR_INVALID_VERSION_ARB))
					{
						errorMessage << "The requested version number is invalid";
					}
					else if ((errorCode == ERROR_INVALID_PROFILE_ARB)
						|| (HRESULT_CODE(errorCode) == ERROR_INVALID_PROFILE_ARB))
					{
						errorMessage << "The requested profile is invalid";
					}
					else
					{
						errorMessage << windowsErrorMessage;
					}
					ASSERTF(false, errorMessage.str().c_str());
					Engine::Logging::OutputError(errorMessage.str().c_str());

					return false;
				}
			}
			// Set it as the rendering context of this thread
			if (wglMakeCurrent(s_deviceContext, s_openGlRenderingContext) == FALSE)
			{
				const std::string windowsErrorMessage = Engine::Windows::Functions::GetLastSystemError();
				ASSERTF(false, windowsErrorMessage.c_str());
				Engine::Logging::OutputError("Windows failed to set the current OpenGL rendering context: %s",
					windowsErrorMessage.c_str());
				return false;
			}
		}

		return true;
	}

	bool EnableDepthTesting()
	{
		glEnable(GL_CULL_FACE);
		// Depth Testing
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);

		return true;
	}
#endif
}
