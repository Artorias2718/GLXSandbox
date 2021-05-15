/*
	This class builds meshes
*/

#ifndef TOOLS_MATERIALBUILDER_CMATERIALBUILDER_H
#define TOOLS_MATERIALBUILDER_CMATERIALBUILDER_H

// Header Files
//=============

#include "../AssetBuildLibrary/cBaseBuilder.h"

// Class Declaration
//==================

namespace Tools 
{
	namespace MaterialBuilder
	{
		class cMaterialBuilder : public AssetBuildLibrary::cBaseBuilder
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

#endif	// !TOOLS_MATERIALBUILDER_CMATERIALBUILDER_H