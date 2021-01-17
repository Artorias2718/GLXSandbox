// Header Files
//=============

#include "cGameObjectBuilder.h"

#include <sstream>
#include "../AssetBuildLibrary/UtilityFunctions.h"
#include "../../Engine/Platform/Platform.h"

// Inherited Implementation
//=========================

// Build
//------

bool Tools::GameObjectBuilder::cGameObjectBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;

	// Copy the source to the target
	{
		std::string errorMessage;

		bool succeeded = Engine::Platform::CopyFile(m_path_source, m_path_target, false, false, &errorMessage);

		if (!succeeded)
		{
			wereThereErrors = true;
			std::ostringstream errorMessage;
			errorMessage << "Failed to copy \"" << m_path_source << "\" to \"" << m_path_target << "\": ";
			Tools::AssetBuildLibrary::UtilityFunctions::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
		}
	}

	return !wereThereErrors;
}