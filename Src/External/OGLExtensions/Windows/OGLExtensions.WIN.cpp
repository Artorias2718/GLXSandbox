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
	void* GetOGLFunctionAddress( const char* i_functionName, std::string* o_errorMessage = NULL );
}

// Interface
//==========

// OGL Extension Definitions
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
	// (The Windows OGL implementation requires an OGL window to be created
	// before any of the functions can be called,
	// and so we create a hidden window in order to load the extensions
	// and then destroy it afterwards,
	// all before doing anything with the main window)
	HINSTANCE hInstance = NULL;
	Engine::Windows::OGL::sHiddenWindowInfo hiddenWindowInfo;
	if ( Engine::Windows::OGL::CreateHiddenContextWindow( hInstance, hiddenWindowInfo, o_errorMessage ) )
	{
		ASSERTF( wglGetCurrentContext() != NULL, "OGL extensions can't be loaded without a current OGL context" );
	}
	else
	{
		wereThereErrors = true;
		ASSERTF( false, o_errorMessage->c_str() );
		goto OnExit;
	}

	// Load each extension
#define GLEXTENSIONS_LOADFUNCTION( i_functionName, i_functionType )														\
		i_functionName = reinterpret_cast<i_functionType>( GetOGLFunctionAddress( #i_functionName, o_errorMessage ) );	\
		if ( !i_functionName )																							\
			goto OnExit;

	GLEXTENSIONS_LOADFUNCTION( glActiveTexture, PFNGLACTIVETEXTUREPROC );
	GLEXTENSIONS_LOADFUNCTION( glAttachShader, PFNGLATTACHSHADERPROC );
	GLEXTENSIONS_LOADFUNCTION( glBindBuffer, PFNGLBINDBUFFERPROC );
	GLEXTENSIONS_LOADFUNCTION( glBindBufferBase, PFNGLBINDBUFFERBASEPROC );
	GLEXTENSIONS_LOADFUNCTION( glBindVertexArray, PFNGLBINDVERTEXARRAYPROC );
	GLEXTENSIONS_LOADFUNCTION( glBufferData, PFNGLBUFFERDATAPROC );
	GLEXTENSIONS_LOADFUNCTION( glBufferSubData, PFNGLBUFFERSUBDATAPROC );
	GLEXTENSIONS_LOADFUNCTION( glCompileShader, PFNGLCOMPILESHADERPROC );
	GLEXTENSIONS_LOADFUNCTION( glCompressedTexImage2D, PFNGLCOMPRESSEDTEXIMAGE2DPROC );
	GLEXTENSIONS_LOADFUNCTION( glCreateProgram, PFNGLCREATEPROGRAMPROC );
	GLEXTENSIONS_LOADFUNCTION( glCreateShader, PFNGLCREATESHADERPROC );
	GLEXTENSIONS_LOADFUNCTION( glDeleteBuffers, PFNGLDELETEBUFFERSPROC );
	GLEXTENSIONS_LOADFUNCTION( glDeleteProgram, PFNGLDELETEPROGRAMPROC );
	GLEXTENSIONS_LOADFUNCTION( glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC );
	GLEXTENSIONS_LOADFUNCTION( glDeleteShader, PFNGLDELETESHADERPROC );
	GLEXTENSIONS_LOADFUNCTION( glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYARBPROC );
	GLEXTENSIONS_LOADFUNCTION( glGenBuffers, PFNGLGENBUFFERSPROC );
	GLEXTENSIONS_LOADFUNCTION( glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC );
	GLEXTENSIONS_LOADFUNCTION( glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC );
	GLEXTENSIONS_LOADFUNCTION( glGetProgramiv, PFNGLGETPROGRAMIVPROC );
	GLEXTENSIONS_LOADFUNCTION( glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC );
	GLEXTENSIONS_LOADFUNCTION( glGetShaderiv, PFNGLGETSHADERIVPROC );
	GLEXTENSIONS_LOADFUNCTION( glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC );
	GLEXTENSIONS_LOADFUNCTION( glLinkProgram, PFNGLLINKPROGRAMPROC );
	GLEXTENSIONS_LOADFUNCTION( glShaderSource, PFNGLSHADERSOURCEPROC );
	GLEXTENSIONS_LOADFUNCTION( glUniform1fv, PFNGLUNIFORM1FVPROC );
	GLEXTENSIONS_LOADFUNCTION( glUniform1i, PFNGLUNIFORM1IPROC );
	GLEXTENSIONS_LOADFUNCTION( glUniform2fv, PFNGLUNIFORM2FVPROC );
	GLEXTENSIONS_LOADFUNCTION( glUniform3fv, PFNGLUNIFORM3FVPROC );
	GLEXTENSIONS_LOADFUNCTION( glUniform4fv, PFNGLUNIFORM4FVPROC );
	GLEXTENSIONS_LOADFUNCTION( glUniformBlockBinding, PFNGLUNIFORMBLOCKBINDINGPROC );
	GLEXTENSIONS_LOADFUNCTION( glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC );
	GLEXTENSIONS_LOADFUNCTION( glUseProgram, PFNGLUSEPROGRAMPROC );
	GLEXTENSIONS_LOADFUNCTION( glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC );
	GLEXTENSIONS_LOADFUNCTION( wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC );
	GLEXTENSIONS_LOADFUNCTION( wglCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC );

#undef GLEXTENSIONS_LOADFUNCTION

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
	void* GetOGLFunctionAddress( const char* i_functionName, std::string* o_errorMessage )
	{
		void* address = reinterpret_cast<void*>( wglGetProcAddress( i_functionName ) );
		// The documentation says that NULL will be returned if the function isn't found,
		// but according to https://www.OGL.org/wiki/Load_OGL_Functions
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
			wglErrorMessage = Engine::Windows::GetLastSystemError();
			ASSERTF( false, "The OGL extension function \"%s\" wasn't found"
				" (it will now be looked for in the non-extension Windows functions)", i_functionName );
		}
		// wglGetProcAddress() won't return the address of any 1.1 or earlier OGL functions
		// that are built into Windows' OGL32.dll
		// but an attempt can be made to load those manually
		// in case the user of this function has made a mistake
		{
			// This library should already be loaded,
			// and so this function will just retrieve a handle to it
			HMODULE glLibrary = LoadLibrary( "OGL32.dll" );
			if ( glLibrary != NULL )
			{
				// Look for an old OGL function
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
					const std::string windowsErrorMessage = Engine::Windows::GetLastSystemError();
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

		// If this code is reached the OGL function wasn't found
		if ( o_errorMessage )
		{
			std::ostringstream errorMessage;
			errorMessage << "Windows failed to find the address of the OGL function \"" << i_functionName << "\"";
			if ( !wglErrorMessage.empty() )
			{
				errorMessage << ": " << wglErrorMessage;
			}
			*o_errorMessage = errorMessage.str();
		}

		return NULL;
	}
}
