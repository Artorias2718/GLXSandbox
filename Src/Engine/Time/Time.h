/*
	This file manages time-related functionality
*/

#ifndef TIME_H
#define TIME_H

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

#endif	// TIME_H
