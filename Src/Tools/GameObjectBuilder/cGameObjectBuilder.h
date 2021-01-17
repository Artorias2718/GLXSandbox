/*
	This class builds meshes
*/

#ifndef TOOLS_GAMEOBJECTBUILDER_CGAMEOBJECTBUILDER_H
#define TOOLS_GAMEOBJECTBUILDER_CGAMEOBJECTBUILDER_H

// Header Files
//=============

#include "../AssetBuildLibrary/cBaseBuilder.h"

// Class Declaration
//==================

namespace Tools 
{
	namespace GameObjectBuilder 
	{
		class cGameObjectBuilder : public AssetBuildLibrary::cBaseBuilder
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

#endif // !TOOLS_GAMEOBJECTBUILDER_CGAMEOBJECTBUILDER_H