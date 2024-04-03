// Header Files
//=============

#include "UserSettings.h"
#include <cmath>
#include <sstream>
#include <string>
#include "../Asserts/Asserts.h"
#include "../Logging/Logging.h"
#include "../Platform/Platform.h"
#include "../../External/Lua/Includes.h"

// Static Data Initialization
//===========================

namespace
{
	unsigned int s_resolutionHeight = 512;
	unsigned int s_resolutionWidth = 512;

	const char* const s_userSettingsFileName = "Settings.ini";
}

// Helper Function Declarations
//=============================

namespace
{
	bool InitializeIfNecessary();
	bool IsNumberAnInteger(const lua_Number i_number);
	bool LoadUserSettingsIntoLuaTable(lua_State& io_luaState, bool& o_doesUserSettingsFileExist);
	bool PopulateUserSettingsFromLuaTable(lua_State& io_luaState);
}

// Interface
//==========

unsigned int Engine::UserSettings::GetResolutionHeight()
{
	InitializeIfNecessary();
	return s_resolutionHeight;
}

unsigned int Engine::UserSettings::GetResolutionWidth()
{
	InitializeIfNecessary();
	return s_resolutionWidth;
}

// Helper Function Definitions
//============================

namespace
{
	bool InitializeIfNecessary()
	{
		static bool isInitialized = false;
		if (isInitialized)
		{
			return true;
		}

		bool wereThereErrors = false;

		// Create a new Lua state
		lua_State* luaState = NULL;
		bool wasUserSettingsEnvironmentCreated = false;
		{
			luaState = luaL_newstate();
			if (luaState == NULL)
			{
				ASSERTF(false, "Failed to create a new Lua state");
				Engine::Logging::OutputError("Failed to create a new Lua state for the user settings");
				wereThereErrors = true;
				goto CleanUp;
			}
		}
		// Create an empty table to be used as the Lua environment for the user settings
		{
			lua_newtable(luaState);
			wasUserSettingsEnvironmentCreated = true;
		}
		// Load the user settings
		bool doesUserSettingsFileExist;
		if (LoadUserSettingsIntoLuaTable(*luaState, doesUserSettingsFileExist))
		{
			if (doesUserSettingsFileExist)
			{
				// Populate the user settings in C from the user settings in the Lua environment
				if (!PopulateUserSettingsFromLuaTable(*luaState))
				{
					wereThereErrors = true;
					goto CleanUp;
				}
			}
		}
		else
		{
			wereThereErrors = true;
			goto CleanUp;
		}

		// Free the Lua environment

	CleanUp:

		if (luaState)
		{
			if (wasUserSettingsEnvironmentCreated)
			{
				lua_pop(luaState, 1);
			}
			ASSERTF(lua_gettop(luaState) == 0, "Lua stack is inconsistent");
			lua_close(luaState);
		}

		isInitialized = !wereThereErrors;
		return isInitialized;
	}

	bool IsNumberAnInteger(const lua_Number i_number)
	{
		lua_Number integralPart;
		lua_Number fractionalPart = modf(i_number, &integralPart);
		return integralPart == i_number;
	}

	bool LoadUserSettingsIntoLuaTable(lua_State& io_luaState, bool& o_doesUserSettingsFileExist)
	{
		// Load the file into a Lua function
		if (Engine::Platform::DoesFileExist(s_userSettingsFileName))
		{
			int result = luaL_loadfile(&io_luaState, s_userSettingsFileName);
			if (result == LUA_OK)
			{
				o_doesUserSettingsFileExist = true;

				// Set the Lua function's environment
				{
					lua_pushvalue(&io_luaState, -2);
					const char* upValueName = lua_setupvalue(&io_luaState, -2, 1);
					if (upValueName == NULL)
					{
						ASSERT(false);
						Engine::Logging::OutputError("Internal error setting the Lua environment for the user settings file \"%s\"!"
							" This should never happen", s_userSettingsFileName);
						lua_pop(&io_luaState, 2);
						return false;
					}
				}
				// Call the Lua function
				{
					int noArguments = 0;
					int noReturnValues = 0;
					int noErrorMessageHandler = 0;
					result = lua_pcall(&io_luaState, noArguments, noReturnValues, noErrorMessageHandler);
					if (result == LUA_OK)
					{
						return true;
					}
					else
					{
						std::string luaErrorMessage(lua_tostring(&io_luaState, -1));
						lua_pop(&io_luaState, 1);

						ASSERTF(false, "User settings file error: %s", luaErrorMessage.c_str());
						if (result == LUA_ERRRUN)
						{
							Engine::Logging::OutputError("Error in the user settings file \"%s\": %s",
								s_userSettingsFileName, luaErrorMessage);
						}
						else
						{
							Engine::Logging::OutputError("Error processing the user settings file \"%s\": %s",
								s_userSettingsFileName, luaErrorMessage);
						}

						return false;
					}
				}
			}
			else
			{
				o_doesUserSettingsFileExist = false;

				std::string luaErrorMessage(lua_tostring(&io_luaState, -1));
				lua_pop(&io_luaState, 1);

				if (result == LUA_ERRFILE)
				{
					ASSERTF("Error opening or reading user settings file: %s", luaErrorMessage.c_str());
					Engine::Logging::OutputError("Error opening or reading the user settings file \"%s\" even though it exists: %s",
						s_userSettingsFileName, luaErrorMessage.c_str());
				}
				else if (result == LUA_ERRSYNTAX)
				{
					ASSERTF("Syntax error in user settings file: %s", luaErrorMessage.c_str());
					Engine::Logging::OutputError("Syntax error in the user settings file \"%s\": %s",
						s_userSettingsFileName, luaErrorMessage.c_str());
				}
				else
				{
					ASSERTF("Error loading user settings file: %s", luaErrorMessage.c_str());
					Engine::Logging::OutputError("Error loading the user settings file \"%s\": %s",
						s_userSettingsFileName, luaErrorMessage.c_str());
				}

				return false;
			}
		}
		else
		{
			o_doesUserSettingsFileExist = false;

			// If loading the file failed because the file doesn't exist it's ok;
			// default values will be used
			Engine::Logging::OutputMessage("The user settings file \"%s\" doesn't exist. Using default settings instead.",
				s_userSettingsFileName);
			return true;
		}
	}

	bool PopulateUserSettingsFromLuaTable(lua_State& io_luaState)
	{
		// Resolution
		{
			const char* keyResolution = "resolution";
			lua_pushstring(&io_luaState, keyResolution);
			lua_gettable(&io_luaState, -2);
			if (lua_istable(&io_luaState, -1))
			{
				// Width
				{
					const char* keyWidth = "width";

					lua_pushstring(&io_luaState, keyWidth);
					lua_gettable(&io_luaState, -2);
					if (lua_isnumber(&io_luaState, -1))
					{
						lua_Number floatingPointResult = lua_tonumber(&io_luaState, -1);
						if (IsNumberAnInteger(floatingPointResult))
						{
							if (floatingPointResult >= lua_Number(0))
							{
								s_resolutionWidth = static_cast<unsigned int>(floatingPointResult + 0.5f);
								//Engine::Logging::OutputMessage("Current window width: " + s_resolutionWidth);
							}
							else
							{
								Engine::Logging::OutputError("The user settings file %s specifies a negative resolution width of %f. Using default %u instead",
									floatingPointResult, s_resolutionWidth);
							}
						}
					}
					lua_pop(&io_luaState, 1);
				}
				// Height
				{
					const char* keyHeight = "height";

					lua_pushstring(&io_luaState, keyHeight);
					lua_gettable(&io_luaState, -2);
					if (lua_isnumber(&io_luaState, -1))
					{
						lua_Number floatingPointResult = lua_tonumber(&io_luaState, -1);
						if (IsNumberAnInteger(floatingPointResult))
						{
							if (floatingPointResult >= lua_Number(0))
							{
								s_resolutionHeight = static_cast<unsigned int>(floatingPointResult + 0.5f);
								//Engine::Logging::OutputMessage("Current window height: " + s_resolutionHeight);
							}
							else
							{
								Engine::Logging::OutputError("The user settings file %s specifies a negative resolution height of %f. Using default %u instead",
									floatingPointResult, s_resolutionHeight);
							}
						}
					}
					lua_pop(&io_luaState, 1);
				}
			}
			lua_pop(&io_luaState, 1);
		}

		return true;
	}
}
