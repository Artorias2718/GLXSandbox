/*
	This class builds hardware-ready DDS textures from source images
*/

#ifndef TOOLS_TEXTUREBUILDER_CTEXTUREBUILDER_H
#define TOOLS_TEXTUREBUILDER_CTEXTUREBUILDER_H

// Header Files
//=============

#include "../AssetBuildLibrary/cBaseBuilder.h"

// Class Declaration
//==================

namespace Tools
{
	namespace TextureBuilder
	{
		class cTextureBuilder : public AssetBuildLibrary::cBaseBuilder
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

#endif // !TOOLS_TEXTUREBUILDER_CTEXTUREBUILDER_H