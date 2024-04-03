/*
	This class builds shaders
*/

#ifndef TOOLS_SHADERBUILDER_CSHADERBUILDER_H
#define TOOLS_SHADERBUILDER_CSHADERBUILDER_H

// Header Files
//=============

#include "../AssetBuildLibrary/cBaseBuilder.h"

#include "../../Engine/Graphics/Configuration.h"
#include "eShaderTypes.h"

// Class Declaration
//==================

namespace Tools 
{
	namespace ShaderBuilder
	{
		class cShaderBuilder : public AssetBuildLibrary::cBaseBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			virtual bool Build(const std::vector<std::string>& i_arguments);

			// Implementation
			//===============

		private:

			// Build
			//------

			bool Build(const ShaderBuilder::eShaderType i_shaderType, const std::vector<std::string>& i_arguments);
		};
	}
}

#endif	// !TOOLS_SHADERBUILDER_CSHADERBUILDER_H