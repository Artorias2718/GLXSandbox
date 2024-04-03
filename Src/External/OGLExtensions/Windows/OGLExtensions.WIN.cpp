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
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
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
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

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
#define OGLExtensions_LOADFUNCTION( i_functionName, i_functionType )														\
		i_functionName = reinterpret_cast<i_functionType>( GetGLFunctionAddress( #i_functionName, o_errorMessage ) );	\
		if ( !i_functionName )																							\
			goto OnExit;

	OGLExtensions_LOADFUNCTION( glActiveTexture, PFNGLACTIVETEXTUREPROC );
	OGLExtensions_LOADFUNCTION( glAttachShader, PFNGLATTACHSHADERPROC );
	OGLExtensions_LOADFUNCTION( glBindBuffer, PFNGLBINDBUFFERPROC );
	OGLExtensions_LOADFUNCTION( glBindBufferBase, PFNGLBINDBUFFERBASEPROC );
	OGLExtensions_LOADFUNCTION( glBindVertexArray, PFNGLBINDVERTEXARRAYPROC );
	OGLExtensions_LOADFUNCTION( glBufferData, PFNGLBUFFERDATAPROC );
	OGLExtensions_LOADFUNCTION( glBufferSubData, PFNGLBUFFERSUBDATAPROC );
	OGLExtensions_LOADFUNCTION( glCompileShader, PFNGLCOMPILESHADERPROC );
	OGLExtensions_LOADFUNCTION( glCompressedTexImage2D, PFNGLCOMPRESSEDTEXIMAGE2DPROC );
	OGLExtensions_LOADFUNCTION( glCreateProgram, PFNGLCREATEPROGRAMPROC );
	OGLExtensions_LOADFUNCTION( glCreateShader, PFNGLCREATESHADERPROC );
	OGLExtensions_LOADFUNCTION( glDeleteBuffers, PFNGLDELETEBUFFERSPROC );
	OGLExtensions_LOADFUNCTION( glDeleteProgram, PFNGLDELETEPROGRAMPROC );
	OGLExtensions_LOADFUNCTION( glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC );
	OGLExtensions_LOADFUNCTION( glDeleteShader, PFNGLDELETESHADERPROC );
	OGLExtensions_LOADFUNCTION( glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYARBPROC );
	OGLExtensions_LOADFUNCTION( glGenBuffers, PFNGLGENBUFFERSPROC );
	OGLExtensions_LOADFUNCTION( glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC );
	OGLExtensions_LOADFUNCTION( glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC );
	OGLExtensions_LOADFUNCTION( glGetProgramiv, PFNGLGETPROGRAMIVPROC );
	OGLExtensions_LOADFUNCTION( glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC );
	OGLExtensions_LOADFUNCTION( glGetShaderiv, PFNGLGETSHADERIVPROC );
	OGLExtensions_LOADFUNCTION( glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC );
	OGLExtensions_LOADFUNCTION( glLinkProgram, PFNGLLINKPROGRAMPROC );
	OGLExtensions_LOADFUNCTION( glShaderSource, PFNGLSHADERSOURCEPROC );
	OGLExtensions_LOADFUNCTION( glUniform1fv, PFNGLUNIFORM1FVPROC );
	OGLExtensions_LOADFUNCTION( glUniform1i, PFNGLUNIFORM1IPROC );
	OGLExtensions_LOADFUNCTION( glUniform2fv, PFNGLUNIFORM2FVPROC );
	OGLExtensions_LOADFUNCTION( glUniform3fv, PFNGLUNIFORM3FVPROC );
	OGLExtensions_LOADFUNCTION( glUniform4fv, PFNGLUNIFORM4FVPROC );
	OGLExtensions_LOADFUNCTION( glUniformBlockBinding, PFNGLUNIFORMBLOCKBINDINGPROC );
	OGLExtensions_LOADFUNCTION( glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC );
	OGLExtensions_LOADFUNCTION( glUseProgram, PFNGLUSEPROGRAMPROC );
	OGLExtensions_LOADFUNCTION( glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC );
	OGLExtensions_LOADFUNCTION( wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC );
	OGLExtensions_LOADFUNCTION( wglCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC );

#undef OGLExtensions_LOADFUNCTION

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
