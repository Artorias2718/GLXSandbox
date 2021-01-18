// Header Files
//=============

#include "cMaterialBuilder.h"

#include <sstream>
#include <climits>
#include <fstream>

#include "../AssetBuildLibrary/UtilityFunctions.h"

#include "../../Engine/Graphics/Structures/sMaterial.h"
#include "../../Engine/Graphics/Interfaces/cConstantBuffer.h"

#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Logging/Logging.h"
#include "../../External/Lua/Includes.h"

// Inherited Implementation
//=========================

// Build
//------

bool Tools::MaterialBuilder::cMaterialBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;
	lua_State* luaState = luaL_newstate();
	std::string effectName;
	std::ofstream binPtr(m_path_target, std::ios::binary);

	if (luaState == NULL)
	{
		AssetBuildLibrary::UtilityFunctions::OutputErrorMessage("Lua state wasn't created");
		goto OnExit;
	}

	int result = luaL_dofile(luaState, m_path_source);
	if (result != LUA_OK)
	{
		goto OnExit;
	}

	// Initialize constant buffer data with the default (white) color, change it if m_color exists
	Engine::Graphics::Structures::sMaterial *materialData = new Engine::Graphics::Structures::sMaterial();

	lua_pushstring(luaState, "g_color");
	lua_gettable(luaState, -2);

	if (lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "r");
		lua_gettable(luaState, -2);
		materialData->g_color.r = (float)lua_tonumber(luaState, -1);
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "g");
		lua_gettable(luaState, -2);
		materialData->g_color.g = (float)lua_tonumber(luaState, -1);
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "b");
		lua_gettable(luaState, -2);
		materialData->g_color.b = (float)lua_tonumber(luaState, -1);
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "a");
		lua_gettable(luaState, -2);
		materialData->g_color.a = (float)lua_tonumber(luaState, -1);
		lua_pop(luaState, 1);
	}
	lua_pop(luaState, 1);

	lua_pushstring(luaState, "effectName");
	lua_gettable(luaState, -2);
	effectName = lua_tostring(luaState, -1);
	lua_pop(luaState, 1);

	if (binPtr.fail())
	{
		wereThereErrors = true;
		AssetBuildLibrary::UtilityFunctions::OutputErrorMessage("There was an problem opening the file!");
		goto OnExit;
	}
	else
	{
		std::string* errorMessage = new std::string;

		// Convert Material Constant Data to its binary representation

		binPtr.write(reinterpret_cast<char*>(materialData), sizeof(Engine::Graphics::Structures::sMaterial));

		binPtr.write(effectName.c_str(), effectName.length());
		binPtr.put('\0');

		effectName[0] = 0;

		binPtr.close();
	}


OnExit:

	if (luaState)
	{
		lua_close(luaState);
		luaState = nullptr;
	}

	return !wereThereErrors;
}