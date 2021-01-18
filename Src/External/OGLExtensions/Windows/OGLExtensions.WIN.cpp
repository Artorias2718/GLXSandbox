// Header Files
//=============

#include "../OGLExtensions.h"

#include <sstream>
#include "../../../Engine/Asserts/Asserts.h"
#include "../../../Engine/Windows/Functions.h"
#include "../../../Engine/Windows/OGL.h"

// Helper Function Declarations
//=============================

namespace
{
	void* GetGLFunctionAddress( const char* i_functionName, std::string* o_errorMessage = NULL );
}

// Interface
//==========

// OpenGL Extension Definitions
//-----------------------------

PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = NULL;
PFNGLBINDSAMPLERPROC glBindSampler = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLGENSAMPLERSPROC glGenSamplers = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
#if defined ( WINDOWS_API )
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
#endif

// Initialization
//---------------

bool External::OGLExtensions::Load( std::string* o_errorMessage )
{
	bool wereThereErrors = false;

	// Load a hidden window
	// (The Windows OpenGL implementation requires an OpenGL window to be created
	// before any of the functions can be called,
	// and so we create a hidden window in order to load the extensions
	// and then destroy it afterwards,
	// all before doing anything with the main window)
	HINSTANCE hInstance = NULL;
	Engine::Windows::OGL::sHiddenWindowInfo hiddenWindowInfo;
	if ( Engine::Windows::OGL::CreateHiddenContextWindow( hInstance, hiddenWindowInfo, o_errorMessage ) )
	{
		ASSERTF( wglGetCurrentContext() != NULL, "OpenGL extensions can't be loaded without a current OpenGL context" );
	}
	else
	{
		wereThereErrors = true;
		ASSERTF( false, o_errorMessage->c_str() );
		goto OnExit;
	}

	// Load each extension
#define OGLEXTENSIONS_LOADFUNCTION( i_functionName, i_functionType )														\
		i_functionName = reinterpret_cast<i_functionType>( GetGLFunctionAddress( #i_functionName, o_errorMessage ) );	\
		if ( !i_functionName )																							\
			goto OnExit;

	OGLEXTENSIONS_LOADFUNCTION(glActiveTexture, PFNGLACTIVETEXTUREPROC);
	OGLEXTENSIONS_LOADFUNCTION(glAttachShader, PFNGLATTACHSHADERPROC);
	OGLEXTENSIONS_LOADFUNCTION(glBindBuffer, PFNGLBINDBUFFERPROC);
	OGLEXTENSIONS_LOADFUNCTION(glBindBufferBase, PFNGLBINDBUFFERBASEPROC);
	OGLEXTENSIONS_LOADFUNCTION(glBindSampler, PFNGLBINDSAMPLERPROC);
	OGLEXTENSIONS_LOADFUNCTION(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC);
	OGLEXTENSIONS_LOADFUNCTION(glBufferData, PFNGLBUFFERDATAPROC);
	OGLEXTENSIONS_LOADFUNCTION(glBufferSubData, PFNGLBUFFERSUBDATAPROC);
	OGLEXTENSIONS_LOADFUNCTION(glCompileShader, PFNGLCOMPILESHADERPROC);
	OGLEXTENSIONS_LOADFUNCTION(glCompressedTexImage2D, PFNGLCOMPRESSEDTEXIMAGE2DPROC);
	OGLEXTENSIONS_LOADFUNCTION(glCreateProgram, PFNGLCREATEPROGRAMPROC);
	OGLEXTENSIONS_LOADFUNCTION(glCreateShader, PFNGLCREATESHADERPROC);
	OGLEXTENSIONS_LOADFUNCTION(glDeleteBuffers, PFNGLDELETEBUFFERSPROC);
	OGLEXTENSIONS_LOADFUNCTION(glDeleteProgram, PFNGLDELETEPROGRAMPROC);
	OGLEXTENSIONS_LOADFUNCTION(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC);
	OGLEXTENSIONS_LOADFUNCTION(glDeleteShader, PFNGLDELETESHADERPROC);
	OGLEXTENSIONS_LOADFUNCTION(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYARBPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGenBuffers, PFNGLGENBUFFERSPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGenSamplers, PFNGLGENSAMPLERSPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGetProgramiv, PFNGLGETPROGRAMIVPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGetShaderiv, PFNGLGETSHADERIVPROC);
	OGLEXTENSIONS_LOADFUNCTION(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC);
	OGLEXTENSIONS_LOADFUNCTION(glLinkProgram, PFNGLLINKPROGRAMPROC);
	OGLEXTENSIONS_LOADFUNCTION(glSamplerParameteri, PFNGLSAMPLERPARAMETERIPROC);
	OGLEXTENSIONS_LOADFUNCTION(glShaderSource, PFNGLSHADERSOURCEPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUniform1fv, PFNGLUNIFORM1FVPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUniform1i, PFNGLUNIFORM1IPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUniform2fv, PFNGLUNIFORM2FVPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUniform3fv, PFNGLUNIFORM3FVPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUniform4fv, PFNGLUNIFORM4FVPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUniformBlockBinding, PFNGLUNIFORMBLOCKBINDINGPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC);
	OGLEXTENSIONS_LOADFUNCTION(glUseProgram, PFNGLUSEPROGRAMPROC);
	OGLEXTENSIONS_LOADFUNCTION(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC);
	OGLEXTENSIONS_LOADFUNCTION(wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC);
	OGLEXTENSIONS_LOADFUNCTION(wglCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC);
#undef OGLEXTENSIONS_LOADFUNCTION

OnExit:

	if ( !Engine::Windows::OGL::FreeHiddenContextWindow( hInstance, hiddenWindowInfo, o_errorMessage ) )
	{
		wereThereErrors = true;
		ASSERTF( false, o_errorMessage->c_str() );
	}

	return !wereThereErrors;
}

// Helper Function Declarations
//=============================

namespace
{
	void* GetGLFunctionAddress( const char* i_functionName, std::string* o_errorMessage )
	{
		void* address = reinterpret_cast<void*>( wglGetProcAddress( i_functionName ) );
		// The documentation says that NULL will be returned if the function isn't found,
		// but according to https://www.opengl.org/wiki/Load_OpenGL_Functions
		// other values can be returned by some implementations
		if ( ( address != NULL )
			&& ( address != reinterpret_cast<void*>( 1 ) ) && ( address != reinterpret_cast<void*>( 2 ) )
			&& ( address != reinterpret_cast<void*>( 3 ) ) && ( address != reinterpret_cast<void*>( -1 ) ) )
		{
			return address;
		}
		std::string wglErrorMessage;
		if ( address == NULL )
		{
			wglErrorMessage = Engine::Windows::Functions::GetLastSystemError();
			ASSERTF( false, "The OpenGL extension function \"%s\" wasn't found"
				" (it will now be looked for in the non-extension Windows functions)", i_functionName );
		}
		// wglGetProcAddress() won't return the address of any 1.1 or earlier OpenGL functions
		// that are built into Windows' Opengl32.dll
		// but an attempt can be made to load those manually
		// in case the user of this function has made a mistake
		{
			// This library should already be loaded,
			// and so this function will just retrieve a handle to it
			HMODULE glLibrary = LoadLibrary( "Opengl32.dll" );
			if ( glLibrary != NULL )
			{
				// Look for an old OpenGL function
				void* address = reinterpret_cast<void*>( GetProcAddress( glLibrary, i_functionName ) );
				// Decrement the library's reference count
				FreeLibrary( glLibrary );
				// Return an address if it was found
				if ( address != NULL )
				{
					return address;
				}
				else
				{
					const std::string windowsErrorMessage = Engine::Windows::Functions::GetLastSystemError();
					if ( wglErrorMessage.empty() )
					{
						wglErrorMessage = windowsErrorMessage;
					}
				}
			}
			else
			{
				ASSERT( false );
			}
		}

		// If this code is reached the OpenGL function wasn't found
		if ( o_errorMessage )
		{
			std::ostringstream errorMessage;
			errorMessage << "Windows failed to find the address of the OpenGL function \"" << i_functionName << "\"";
			if ( !wglErrorMessage.empty() )
			{
				errorMessage << ": " << wglErrorMessage;
			}
			*o_errorMessage = errorMessage.str();
		}

		return NULL;
	}
}
