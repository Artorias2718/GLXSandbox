/*
	This class builds meshes
*/

#ifndef TOOLS_MESHBUILDER_CMESHBUILDER_H
#define TOOLS_MESHBUILDER_CMESHBUILDER_H

// Header Files
//=============

#include "../AssetBuildLibrary/cBaseBuilder.h"

// Class Declaration
//==================

namespace Tools 
{
	namespace MeshBuilder
	{
		class cMeshBuilder : public AssetBuildLibrary::cBaseBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			virtual bool Build(const std::vector<std::string>& i_arguments);
		};
	}
}

#endif	// !TOOLS_MESHBUILDER_CMESHBUILDER_H