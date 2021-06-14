// Header Files
//=============

#include "cMeshBuilder.h"

#include <sstream>
#include <climits>
#include <fstream>

#include <sstream>
#include "../AssetBuildLibrary/UtilityFunctions.h"
#include "../../Engine/Platform/Platform.h"
#include "../../Engine/Graphics/Structures/sVertex.h"
#include "../../Engine/Graphics/Structures/sIndexSet.h"

#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Logging/Logging.h"
#include "../../External/Lua/Includes.h"

// Inherited Implementation
//=========================

// Build
//------

bool Tools::MeshBuilder::cMeshBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;

	bool isShort = false;
	uint32_t vertexCount;
	uint32_t indexSetCount;

	Engine::Graphics::Structures::sVertex* vertices = nullptr;
	Engine::Graphics::Structures::sIndexSet16* indexSet16 = nullptr;
	Engine::Graphics::Structures::sIndexSet32* indexSet32 = nullptr;

	lua_State* luaState = luaL_newstate();
	std::ofstream binPtr(m_path_target, std::ios::binary);

	if (luaState == NULL)
	{
		Engine::Logging::OutputError("Lua state wasn't created");
		goto OnExit;
	}

	int result = luaL_dofile(luaState, m_path_source);
	if (result != LUA_OK)
	{
		goto OnExit;
	}

	if (binPtr.fail())
	{
		wereThereErrors = true;
		AssetBuildLibrary::UtilityFunctions::OutputErrorMessage("There was a problem opening the file!");
		goto OnExit;
	}

	// Load Vertex Data
	{
		bool wereThereErrors = false;

		lua_pushstring(luaState, "vertices");
		lua_gettable(luaState, -2);

		if (lua_istable(luaState, -1))
		{
			vertexCount = luaL_len(luaState, -1);
			vertices = (Engine::Graphics::Structures::sVertex*)malloc(vertexCount * sizeof(Engine::Graphics::Structures::sVertex));

			for (uint32_t i = 1; i <= vertexCount; ++i)
			{
				lua_pushinteger(luaState, i);
				lua_gettable(luaState, -2);

				lua_pushstring(luaState, "texture");
				lua_gettable(luaState, -2);

				if (lua_istable(luaState, -1))
				{
					lua_pushstring(luaState, "u");
					lua_gettable(luaState, -2);
					vertices[i - 1].texture.u = static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "v");
					lua_gettable(luaState, -2);
					vertices[i - 1].texture.v =
#if defined D3D_API 
						1.0f -
#endif
						static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);
				}
				lua_pop(luaState, 1);

				lua_pushstring(luaState, "position");
				lua_gettable(luaState, -2);

				if (lua_istable(luaState, -1))
				{
					lua_pushstring(luaState, "x");
					lua_gettable(luaState, -2);
					vertices[i - 1].position.x = static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "y");
					lua_gettable(luaState, -2);
					vertices[i - 1].position.y = static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "z");
					lua_gettable(luaState, -2);
					vertices[i - 1].position.z = static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);
				}
				lua_pop(luaState, 1);

				lua_pushstring(luaState, "color");
				lua_gettable(luaState, -2);

				if (lua_istable(luaState, -1))
				{
					lua_pushstring(luaState, "r");
					lua_gettable(luaState, -2);
					vertices[i - 1].color.r = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "g");
					lua_gettable(luaState, -2);
					vertices[i - 1].color.g = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "b");
					lua_gettable(luaState, -2);
					vertices[i - 1].color.b = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "a");
					lua_gettable(luaState, -2);
					vertices[i - 1].color.a = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
					lua_pop(luaState, 1);
				}

				lua_pop(luaState, 1);
				lua_pop(luaState, 1);
			}
			lua_pop(luaState, 1);
		}

		// Load Index Data
		{
			bool wereThereErrors = false;

			// Create a index buffer object and make it active
			uint32_t indicesPerTriangle = 3;

			lua_pushstring(luaState, "indices");
			lua_gettable(luaState, -2);

			// Iterate through the index buffer container
			uint32_t indexBufferIndex = 1;

			if (lua_istable(luaState, -1))
			{
				indexSetCount = luaL_len(luaState, -1);
				isShort = indicesPerTriangle * indexSetCount <= UINT16_MAX;

				if (isShort)
				{
					indexSet16 = (Engine::Graphics::Structures::sIndexSet16*)malloc(indexSetCount * sizeof(Engine::Graphics::Structures::sIndexSet16));

					for (uint16_t i = 1; i <= indexSetCount; ++i)
					{
						lua_pushinteger(luaState, i);
						lua_gettable(luaState, -2);

						// Get three indices at a time into the index buffer container
						lua_pushstring(luaState, "a");
						lua_gettable(luaState, -2);
						indexSet16[indexBufferIndex - 1].a = static_cast<uint16_t>(lua_tonumber(luaState, -1));
						lua_pop(luaState, 1);

						lua_pushstring(luaState, "b");
						lua_gettable(luaState, -2);
#if defined D3D_API 
						indexSet16[indexBufferIndex - 1].c = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#elif defined OGL_API
						indexSet16[indexBufferIndex - 1].b = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#endif
						lua_pop(luaState, 1);

						lua_pushstring(luaState, "c");
						lua_gettable(luaState, -2);
#if defined D3D_API 
						indexSet16[indexBufferIndex - 1].b = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#elif defined OGL_API 
						indexSet16[indexBufferIndex - 1].c = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#endif
						indexBufferIndex++;
						lua_pop(luaState, 1);
						lua_pop(luaState, 1);
					}
				}
				else
				{
					indexSet32 = (Engine::Graphics::Structures::sIndexSet32*)malloc(indexSetCount * sizeof(Engine::Graphics::Structures::sIndexSet32));

					for (uint32_t i = 1; i <= indexSetCount; ++i)
					{
						lua_pushinteger(luaState, i);
						lua_gettable(luaState, -2);

						// Get three indices at a time into the index buffer container
						lua_pushstring(luaState, "a");
						lua_gettable(luaState, -2);
						indexSet32[indexBufferIndex - 1].a = static_cast<uint32_t>(lua_tonumber(luaState, -1));
						lua_pop(luaState, 1);

						lua_pushstring(luaState, "b");
						lua_gettable(luaState, -2);
#if defined D3D_API 
						indexSet32[indexBufferIndex - 1].c = static_cast<uint32_t>(lua_tonumber(luaState, -1));
#elif defined OGL_API
						indexSet32[indexBufferIndex - 1].b = static_cast<uint32_t>(lua_tonumber(luaState, -1));
#endif
						lua_pop(luaState, 1);

						lua_pushstring(luaState, "c");
						lua_gettable(luaState, -2);
#if defined D3D_API 
						indexSet32[indexBufferIndex - 1].b = static_cast<uint32_t>(lua_tonumber(luaState, -1));
#elif defined OGL_API 
						indexSet32[indexBufferIndex - 1].c = static_cast<uint32_t>(lua_tonumber(luaState, -1));
#endif
						indexBufferIndex++;
						lua_pop(luaState, 1);
						lua_pop(luaState, 1);
					}
				}

				lua_pop(luaState, 1);
			}
		}
	}

	// Write index type identifier

	binPtr.write(reinterpret_cast<char*>(&isShort), sizeof(bool));

	// 3 indices per group because there are 3 points in a triangle
	const uint32_t indicesPerTriangle = 3;

	// Write Vertex Count

	binPtr.write(reinterpret_cast<char*>(&vertexCount), sizeof(uint32_t));

	// Write Index Set Count

	binPtr.write(reinterpret_cast<char*>(&indexSetCount), sizeof(uint32_t));

	// Write Vertex Array

	binPtr.write(reinterpret_cast<char*>(&vertices[0]), vertexCount * sizeof(Engine::Graphics::Structures::sVertex));

	// Write Index Array
	if (isShort)
	{
		binPtr.write(reinterpret_cast<char*>(&indexSet16[0]), indexSetCount * sizeof(Engine::Graphics::Structures::sIndexSet16));
	}
	else
	{
		binPtr.write(reinterpret_cast<char*>(&indexSet32[0]), indexSetCount * sizeof(Engine::Graphics::Structures::sIndexSet32));
	}

	binPtr.close();


OnExit:
	if (luaState)
	{
		lua_close(luaState);
		luaState = nullptr;

		vertices = nullptr;

		if (isShort)
		{
			indexSet16 = nullptr;
		}
		else
		{
			indexSet32 = nullptr;
		}
	}

	return !wereThereErrors;
}