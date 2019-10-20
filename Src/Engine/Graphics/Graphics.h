/*
	This file contains the function declarations for graphics
*/

#ifndef ENGINE_GRAPHICS_H
#define ENGINE_GRAPHICS_H

// Header Files
//=============

#include "Configuration.h"

#if defined( WINDOWS_API )
	#include "../Windows/Includes.h"
#endif

// Interface
//==========

namespace Engine
{
	namespace Graphics
	{
		// Render
		//-------

		void RenderFrame();

		// Initialization / Clean Up
		//--------------------------

		struct sInitializationParameters
		{
#if defined( WINDOWS_API )
			HWND mainWindow;
	#if defined( D3D_API )
			unsigned int resolutionWidth, resolutionHeight;
	#elif defined( GL_API )
			HINSTANCE thisInstanceOfTheApplication;
	#endif
#endif
		};

		bool Initialize( const sInitializationParameters& i_initializationParameters );
		bool CleanUp();
	}
}

#endif	// !ENGINE_GRAPHICS_H
