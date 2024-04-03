/*
	The main() function is where the program starts execution
*/

// Header Files
//=============

#include <cstdlib>
#include "AssetBuild.h"
#include <sstream>
#include "../AssetBuildLibrary/UtilityFunctions.h"

// Static Data Initialization
//===========================

namespace
{
	std::string s_AuthoredAssetDir;
	std::string s_BuiltAssetDir;
}

// Entry Point
//============

int main(int i_argumentCount, char** i_arguments)
{
	bool were_there_errors = false;

	if (!Tools::AssetBuildSystem::AssetBuild::Initialize())
	{
		were_there_errors = true;
		goto OnExit;
	}

	// The command line should have a path to the list of assets to build
	if (i_argumentCount == 2)
	{
		const char* const path_assetsToBuild = i_arguments[1];
		if (!Tools::AssetBuildSystem::AssetBuild::BuildAssets(path_assetsToBuild))
		{
			were_there_errors = true;
			goto OnExit;
		}
	}
	else
	{
		std::stringstream errorMessage;
		errorMessage << "AssetBuildSystem.exe must be run with a single command line argument which is the path to the list of assets to build"
			" (the invalid argument count being passed to main is " << i_argumentCount << ")";
		Tools::AssetBuildLibrary::UtilityFunctions::OutputErrorMessage(errorMessage.str().c_str());
	}

OnExit:

	if (!Tools::AssetBuildSystem::AssetBuild::CleanUp())
	{
		were_there_errors = true;
	}

	if (!were_there_errors)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}
