/*
	This file contains the function declarations for graphics
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

// Header Files
//=============

#include "Configuration.h"

#if defined( WINDOWS_API )
	#include "../Windows/Includes.h"
#endif

#include <vector>

// Interface
//==========

namespace Engine
{
	namespace Shared
	{
		class cGameObject;
	}

	namespace Graphics
	{
		namespace Assets
		{
			class cMesh;
		}

		namespace Interfaces
		{
			class cConstantBuffer;
		}

		namespace Structures
		{
			struct sFrame;
			struct sDrawCall;
			struct sMaterial;
		}

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
	#elif defined( OGL_API )
			HINSTANCE thisInstanceOfTheApplication;
	#endif
#endif
		};

		bool Initialize( const sInitializationParameters& i_initializationParameters );
		bool SubmitGameObject(Shared::cGameObject* i_object);
		bool CleanUp();

		extern std::vector<Shared::cGameObject*> meshes;
		
		extern Interfaces::cConstantBuffer* s_frameBuffer;
		extern Interfaces::cConstantBuffer* s_drawCallBuffer;

		extern Structures::sFrame frameData;
		extern Structures::sDrawCall drawCallData;
	}
}

#endif	// GRAPHICS_H
