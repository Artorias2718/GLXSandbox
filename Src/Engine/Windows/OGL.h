/*
	This file helps with working with OGL under Windows
*/

#ifndef WINDOWS_OGL_H
#define WINDOWS_OGL_H

// Header Files
//=============

#include "Includes.h"

#include <string>

// Interface
//==========

namespace Engine
{
	namespace Windows
	{
		namespace OGL
		{
			// Initializing OGL in Windows requires a device context,
			// and so a hidden window must be created and a device context created
			// before any OGL functions can called

			struct sHiddenWindowInfo
			{
				HWND window;
				ATOM windowClass;
				HDC deviceContext;
				HGLRC OGLRenderingContext;

				sHiddenWindowInfo() : window( NULL ), windowClass( NULL ), deviceContext( NULL ), OGLRenderingContext( NULL ) {}
			};

			// It is ok if the HINSTANCE that is passed to CreateHiddenContextWindow() is NULL,
			// but if it is then the valid HINSTANCE that CreateHiddenContextWindow() assigns _must_ be passed to FreeHiddenContextWindow()
			bool CreateHiddenContextWindow( HINSTANCE& io_hInstance, sHiddenWindowInfo& o_info, std::string* const o_errorMessage = NULL );
			bool FreeHiddenContextWindow( HINSTANCE& i_hInstance, sHiddenWindowInfo& io_info, std::string* const o_errorMessage = NULL );
		}
	}
}

#endif	// WINDOWS_OGL_H
