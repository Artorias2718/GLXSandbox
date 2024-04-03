// Header Files
//=============

#include "../cShaderBuilder.h"

#include <sstream>
#include "../../AssetBuildLibrary/UtilityFunctions.h"
#include "../../../Engine/Platform/Platform.h"

// Interface
//==========

// Build
//------

bool Tools::ShaderBuilder::cShaderBuilder::Build(const Tools::ShaderBuilder::eShaderType i_shaderType, const std::vector<std::string>& i_arguments)
{
	// Get the path to the shader compiler
	std::string path_fxc;
	{
		// Get the path to the DirectX SDK
		std::string path_sdk;
		{
			std::string errorMessage;
			if (!Engine::Platform::GetEnvironmentVariable("DXSDK_DIR", path_sdk, &errorMessage))
			{
				std::ostringstream decoratedErrorMessage;
				decoratedErrorMessage << "Failed to get the path to the DirectX SDK: " << errorMessage;
				Tools::AssetBuildLibrary::UtilityFunctions::OutputErrorMessage(decoratedErrorMessage.str().c_str(), __FILE__);
				return false;
			}
		}
		path_fxc = path_sdk + "Utilities/bin/" +
#ifndef _WIN64
			"x86"
#else
			"x64"
#endif
			+ "/fxc.exe";
	}
	// Create the command to run
	std::string command;
	{
		std::ostringstream commandToBuild;
		commandToBuild << "\"" << path_fxc << "\"";
		// Target profile
		switch (i_shaderType)
		{
		case Tools::ShaderBuilder::eShaderType::VERTEX:
			commandToBuild << " /Tvs_4_0";
			break;
		case Tools::ShaderBuilder::eShaderType::FRAGMENT:
			commandToBuild << " /Tps_4_0";
			break;
		}
		// Entry point
		commandToBuild << " /Emain"
			// #define the platform
			<< " /DD3D_API"
#ifdef GRAPHICS_AREDEBUGSHADERSENABLED
			// Disable optimizations so that debugging is easier
			<< " /Od"
			// Enable debugging
			<< " /Zi"
#endif
			// Target file
			<< " /Fo\"" << m_path_target << "\""
			// Don't output the logo
			<< " /nologo"
			// Source file
			<< " \"" << m_path_source << "\""
			;
		command = commandToBuild.str();
	}
	// Execute the command
	{
		int exitCode;
		std::string errorMessage;
		if (Engine::Platform::ExecuteCommand(command.c_str(), &exitCode, &errorMessage))
		{
			return exitCode == EXIT_SUCCESS;
		}
		else
		{
			Tools::AssetBuildLibrary::UtilityFunctions::OutputErrorMessage(errorMessage.c_str(), m_path_source);
			return false;
		}
	}
}