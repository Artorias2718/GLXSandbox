/*
	This file manages time-related functionality
*/

#ifndef ENGINE_TIME_TIME_H
#define ENGINE_TIME_TIME_H

// Interface
//==========

namespace Engine
{
	namespace Time
	{
		// Time
		//-----

		float ElapsedSeconds();
		float DeltaTime();

		void OnNewFrame();

		// Initialization / Clean Up
		//--------------------------

		bool Initialize();
		bool CleanUp();
	}
}

#endif	// !ENGINE_TIME_TIME_H
