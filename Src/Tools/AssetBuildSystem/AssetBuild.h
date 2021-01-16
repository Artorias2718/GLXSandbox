/*
	These functions are responsible for building assets
*/

#ifndef TOOLS_ASSETBUILDSYSTEM_ASSETBUILD_H
#define TOOLS_ASSETBUILDSYSTEM_ASSETBUILD_H

// Interface
//==========

namespace Tools 
{
	namespace AssetBuildSystem
	{
		namespace AssetBuild
		{
			// Build
			//------

			bool BuildAssets(const char* const i_path_assetsToBuild);

			// Initialization / Clean Up
			//--------------------------

			bool Initialize();
			bool CleanUp();
		}
	}
}

#endif	// !TOOLS_ASSETBUILDSYSTEM_ASSETBUILD_H
