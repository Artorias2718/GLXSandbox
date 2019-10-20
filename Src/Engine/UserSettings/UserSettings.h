/*
	This file manages user-configurable settings
	(ones that the user can set in an INI file)
*/

#ifndef ENGINE_USERSETTINGS_USERSETTINGS_H
#define ENGINE_USERSETTINGS_USERSETTINGS_H

// Interface
//==========

namespace Engine
{
	namespace UserSettings
	{
		unsigned int GetResolutionHeight();
		unsigned int GetResolutionWidth();
	}
}

#endif // !ENGINE_USERSETTINGS_USERSETTINGS_H