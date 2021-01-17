// Header Files
//=============

#include "../Graphics.h"
#include "../Structures/sVertex.h"
#include "../Assets/cMesh.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include "Includes.h"
#include <string>
#include <sstream>
#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"
#include "../../Time/Time.h"
#include "../../Windows/Functions.h"
#include "../../Windows/OGL.h"
#include "../../../External/OGLExtensions/OGLExtensions.h"
#include "../../UserOutput/UserOutput.h"

#include "../Interfaces/cConstantBuffer.h"
#include "../Structures/sFrame.h"
#include "../Structures/sDrawCall.h"

#include "../../Shared/cGameObject.h"

// Static Data Initialization
//===========================

namespace
{
	// The is the main window handle from Windows
	HWND s_renderingWindow = NULL;
	// These are Windows-specific interfaces
	HDC s_deviceContext = NULL;
	HGLRC s_openGlRenderingContext = NULL;

	// OpenGL encapsulates a matching vertex shader and fragment shader into what it calls a "program".

	// A vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* Position
	//		(So that the graphics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want

	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	GLuint s_programId = 0;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateConstantBuffers();
	bool CreateProgram();
	bool CreateRenderingContext();
	bool EnableDepthTesting();
	bool LoadFragmentShader(const GLuint i_programId);
	bool LoadVertexShader(const GLuint i_programId);

	// This helper struct exists to be able to dynamically allocate memory to get "log info"
	// which will automatically be freed when the struct goes out of scope
	struct sLogInfo
	{
		GLchar* memory;
		sLogInfo(const size_t i_size) { memory = reinterpret_cast<GLchar*>(malloc(i_size)); }
		~sLogInfo() { if (memory) free(memory); }
	};
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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		ASSERT(glGetError() == GL_NO_ERROR);
		glDepthMask(GL_TRUE);
		glClearDepth(1.0f);
		const GLbitfield clearColorAndDepth = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		glClear(clearColorAndDepth);
		ASSERT(glGetError() == GL_NO_ERROR);
	}

	// Draw the geometry
	{
		// Set the vertex and fragment shaders
		{
			glUseProgram(s_programId);
			ASSERT(glGetError() == GL_NO_ERROR);
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
	// In order to display it, the contents of the back buffer must be swapped with the "front buffer"
	// (which is what the user sees)
	{
		BOOL result = SwapBuffers(s_deviceContext);
		ASSERT(result != FALSE);
	}
}

// Initialization / Clean Up
//==========================

bool Engine::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
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
	if (!CreateProgram())
	{
		ASSERT(false);
		return false;
	}
	if (!CreateConstantBuffers())
	{
		ASSERT(false);
		return false;
	}

	return true;
}

bool Engine::Graphics::CleanUp()
{
	bool wereThereErrors = false;

	if (s_openGlRenderingContext != NULL)
	{
		if (s_programId != 0)
		{
			glDeleteProgram(s_programId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to delete the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
			s_programId = 0;
		}

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

	s_renderingWindow = NULL;

	return !wereThereErrors;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateConstantBuffers()
	{
		bool wereThereErrors = false;

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

		Engine::Graphics::s_frameBuffer->Bind(Engine::Graphics::Interfaces::FRAME);
		Engine::Graphics::s_drawCallBuffer->Bind(Engine::Graphics::Interfaces::DRAWCALL);

		return !wereThereErrors;
	}

	bool CreateProgram()
	{
		// Create a program
		{
			s_programId = glCreateProgram();
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to create a program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return false;
			}
			else if (s_programId == 0)
			{
				ASSERT(false);
				Engine::Logging::OutputError("OpenGL failed to create a program");
				return false;
			}
		}
		// Load and attach the shaders
		if (!LoadVertexShader(s_programId))
		{
			ASSERT(false);
			return false;
		}
		if (!LoadFragmentShader(s_programId))
		{
			ASSERT(false);
			return false;
		}
		// Link the program
		{
			glLinkProgram(s_programId);
			GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				// Get link info
				// (this won't be used unless linking fails
				// but it can be useful to look at when debugging)
				std::string linkInfo;
				{
					GLint infoSize;
					glGetProgramiv(s_programId, GL_INFO_LOG_LENGTH, &infoSize);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						sLogInfo info(static_cast<size_t>(infoSize));
						GLsizei* dontReturnLength = NULL;
						glGetProgramInfoLog(s_programId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							linkInfo = info.memory;
						}
						else
						{
							ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							Engine::Logging::OutputError("OpenGL failed to get link info of the program: %s",
								reinterpret_cast<const char*>(gluErrorString(errorCode)));
							return false;
						}
					}
					else
					{
						ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Engine::Logging::OutputError("OpenGL failed to get the length of the program link info: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return false;
					}
				}
				// Check to see if there were link errors
				GLint didLinkingSucceed;
				{
					glGetProgramiv(s_programId, GL_LINK_STATUS, &didLinkingSucceed);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						if (didLinkingSucceed == GL_FALSE)
						{
							ASSERTF(false, linkInfo.c_str());
							Engine::Logging::OutputError("The program failed to link: %s",
								linkInfo.c_str());
							return false;
						}
					}
					else
					{
						ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Engine::Logging::OutputError("OpenGL failed to find out if linking of the program succeeded: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						return false;
					}
				}
			}
			else
			{
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to link the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return false;
			}
		}

		return true;
	}

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

	bool LoadFragmentShader(const GLuint i_programId)
	{
		// Verify that compiling shaders at run-time is supported
		{
			GLboolean isShaderCompilingSupported;
			glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
			if (!isShaderCompilingSupported)
			{
				Engine::UserOutput::Print("Compiling shaders at run-time isn't supported on this implementation (this should never happen)");
				return false;
			}
		}

		bool wereThereErrors = false;

		// Load the source code from file and set it into a shader
		GLuint fragmentShaderId = 0;
		Engine::Platform::sDataFromFile dataFromFile;
		{
			// Load the shader source code
			{
				const char* path_sourceCode = "data/shaders/fragment.shader";
				std::string errorMessage;
				if (!Engine::Platform::LoadBinaryFile(path_sourceCode, dataFromFile, &errorMessage))
				{
					wereThereErrors = true;
					ASSERTF(false, errorMessage.c_str());
					Engine::Logging::OutputError("Failed to load the fragment shader \"%s\": %s",
						path_sourceCode, errorMessage.c_str());
					goto OnExit;
				}
			}
			// Generate a shader
			fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
			{
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to get an unused fragment shader ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
				else if (fragmentShaderId == 0)
				{
					wereThereErrors = true;
					ASSERT(false);
					Engine::Logging::OutputError("OpenGL failed to get an unused fragment shader ID");
					goto OnExit;
				}
			}
			// Set the source code into the shader
			{
				const GLsizei shaderSourceCount = 1;
				const GLint length = static_cast<GLuint>(dataFromFile.size);
				glShaderSource(fragmentShaderId, shaderSourceCount, reinterpret_cast<GLchar**>(&dataFromFile.data), &length);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to set the fragment shader source code: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
		}
		// Compile the shader source code
		{
			glCompileShader(fragmentShaderId);
			GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				// Get compilation info
				// (this won't be used unless compilation fails
				// but it can be useful to look at when debugging)
				std::string compilationInfo;
				{
					GLint infoSize;
					glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoSize);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						sLogInfo info(static_cast<size_t>(infoSize));
						GLsizei* dontReturnLength = NULL;
						glGetShaderInfoLog(fragmentShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							compilationInfo = info.memory;
						}
						else
						{
							wereThereErrors = true;
							ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							Engine::Logging::OutputError("OpenGL failed to get compilation info about the fragment shader source code: %s",
								reinterpret_cast<const char*>(gluErrorString(errorCode)));
							goto OnExit;
						}
					}
					else
					{
						wereThereErrors = true;
						ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Engine::Logging::OutputError("OpenGL failed to get the length of the fragment shader compilation info: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						goto OnExit;
					}
				}
				// Check to see if there were compilation errors
				GLint didCompilationSucceed;
				{
					glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						if (didCompilationSucceed == GL_FALSE)
						{
							wereThereErrors = true;
							ASSERTF(false, compilationInfo.c_str());
							Engine::Logging::OutputError("OpenGL failed to compile the fragment shader: %s",
								compilationInfo.c_str());
							goto OnExit;
						}
					}
					else
					{
						wereThereErrors = true;
						ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Engine::Logging::OutputError("OpenGL failed to find if compilation of the fragment shader source code succeeded: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						goto OnExit;
					}
				}
			}
			else
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to compile the fragment shader source code: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		// Attach the shader to the program
		{
			glAttachShader(i_programId, fragmentShaderId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to attach the fragment to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

	OnExit:

		if (fragmentShaderId != 0)
		{
			// Even if the shader was successfully compiled
			// once it has been attached to the program we can (and should) delete our reference to it
			// (any associated memory that OpenGL has allocated internally will be freed
			// once the program is deleted)
			glDeleteShader(fragmentShaderId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to delete the fragment shader ID %u: %s",
					fragmentShaderId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
			fragmentShaderId = 0;
		}
		dataFromFile.Free();

		return !wereThereErrors;
	}

	bool LoadVertexShader(const GLuint i_programId)
	{
		// Verify that compiling shaders at run-time is supported
		{
			GLboolean isShaderCompilingSupported;
			glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
			if (!isShaderCompilingSupported)
			{
				Engine::UserOutput::Print( "Compiling shaders at run-time isn't supported on this implementation (this should never happen)" );
				return false;
			}
		}

		bool wereThereErrors = false;

		// Load the source code from file and set it into a shader
		GLuint vertexShaderId = 0;
		Engine::Platform::sDataFromFile dataFromFile;
		{
			// Load the shader source code
			{
				const char* path_sourceCode = "data/shaders/vertex.shader";
				std::string errorMessage;
				if (!Engine::Platform::LoadBinaryFile(path_sourceCode, dataFromFile, &errorMessage))
				{
					wereThereErrors = true;
					ASSERTF(false, errorMessage.c_str());
					Engine::Logging::OutputError("Failed to load the vertex shader \"%s\": %s",
						path_sourceCode, errorMessage.c_str());
					goto OnExit;
				}
			}
			// Generate a shader
			vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
			{
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to get an unused vertex shader ID: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
				else if (vertexShaderId == 0)
				{
					wereThereErrors = true;
					ASSERT(false);
					Engine::Logging::OutputError("OpenGL failed to get an unused vertex shader ID");
					goto OnExit;
				}
			}
			// Set the source code into the shader
			{
				const GLsizei shaderSourceCount = 1;
				const GLint length = static_cast<GLuint>(dataFromFile.size);
				glShaderSource(vertexShaderId, shaderSourceCount, reinterpret_cast<GLchar**>(&dataFromFile.data), &length);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to set the vertex shader source code: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
		}
		// Compile the shader source code
		{
			glCompileShader(vertexShaderId);
			GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				// Get compilation info
				// (this won't be used unless compilation fails
				// but it can be useful to look at when debugging)
				std::string compilationInfo;
				{
					GLint infoSize;
					glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoSize);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						sLogInfo info(static_cast<size_t>(infoSize));
						GLsizei* dontReturnLength = NULL;
						glGetShaderInfoLog(vertexShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							compilationInfo = info.memory;
						}
						else
						{
							wereThereErrors = true;
							ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
							Engine::Logging::OutputError("OpenGL failed to get compilation info about the vertex shader source code: %s",
								reinterpret_cast<const char*>(gluErrorString(errorCode)));
							goto OnExit;
						}
					}
					else
					{
						wereThereErrors = true;
						ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Engine::Logging::OutputError("OpenGL failed to get the length of the vertex shader compilation info: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						goto OnExit;
					}
				}
				// Check to see if there were compilation errors
				GLint didCompilationSucceed;
				{
					glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						if (didCompilationSucceed == GL_FALSE)
						{
							wereThereErrors = true;
							ASSERTF(false, compilationInfo.c_str());
							Engine::Logging::OutputError("OpenGL failed to compile the vertex shader: %s",
								compilationInfo.c_str());
							goto OnExit;
						}
					}
					else
					{
						wereThereErrors = true;
						ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Engine::Logging::OutputError("OpenGL failed to find if compilation of the vertex shader source code succeeded: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
						goto OnExit;
					}
				}
			}
			else
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to compile the vertex shader source code: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		// Attach the shader to the program
		{
			glAttachShader(i_programId, vertexShaderId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to attach the vertex to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

	OnExit:

		if (vertexShaderId != 0)
		{
			// Even if the shader was successfully compiled
			// once it has been attached to the program we can (and should) delete our reference to it
			// (any associated memory that OpenGL has allocated internally will be freed
			// once the program is deleted)
			glDeleteShader(vertexShaderId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to delete the vertex shader ID %u: %s",
					vertexShaderId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
			vertexShaderId = 0;
		}
		dataFromFile.Free();

		return !wereThereErrors;
	}
}