#include "cMesh.h"
#include "../Structures/sVertex.h"

#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../../External/Lua/Includes.h"

Engine::Graphics::Assets::cMesh::cMesh(std::string i_fileName)
{
	lua_State* luaState = luaL_newstate();
	std::string fullPath = "data/meshes/" + i_fileName + ".mesh";

	if (luaState == NULL)
	{
		Logging::OutputError("Lua state wasn't created");
		goto OnExit;
	}

	int result = luaL_dofile(luaState, fullPath.c_str());
	if (result != LUA_OK)
	{
		goto OnExit;
	}

	// Load Vertex Data
	{
		bool wereThereErrors = false;

		lua_pushstring(luaState, "vertices");
		lua_gettable(luaState, -2);

		if (lua_istable(luaState, -1))
		{
			m_vertexCount = luaL_len(luaState, -1);
			m_vertices = (Engine::Graphics::Structures::sVertex*)malloc(m_vertexCount * sizeof(Engine::Graphics::Structures::sVertex));

			for (uint16_t i = 1; i <= m_vertexCount; ++i)
			{
				lua_pushinteger(luaState, i);
				lua_gettable(luaState, -2);

				lua_pushstring(luaState, "position");
				lua_gettable(luaState, -2);

				if (lua_istable(luaState, -1))
				{
					lua_pushstring(luaState, "x");
					lua_gettable(luaState, -2);
					m_vertices[i - 1].position.x = static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "y");
					lua_gettable(luaState, -2);
					m_vertices[i - 1].position.y = static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);
				}
				lua_pop(luaState, 1);

				lua_pushstring(luaState, "color");
				lua_gettable(luaState, -2);

				if (lua_istable(luaState, -1))
				{
					lua_pushstring(luaState, "r");
					lua_gettable(luaState, -2);
					m_vertices[i - 1].color.r = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "g");
					lua_gettable(luaState, -2);
					m_vertices[i - 1].color.g = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "b");
					lua_gettable(luaState, -2);
					m_vertices[i - 1].color.b = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "a");
					lua_gettable(luaState, -2);
					m_vertices[i - 1].color.a = static_cast<uint8_t>(lua_tonumber(luaState, -1) * 255);
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

			lua_pushstring(luaState, "indices");
			lua_gettable(luaState, -2);

			// Iterate through the index buffer container
			uint16_t indexBufferIndex = 1;

			if (lua_istable(luaState, -1))
			{
				m_indexSetCount = luaL_len(luaState, -1);
				m_indexSet16 = (Engine::Graphics::Structures::sIndexSet16*)malloc(m_indexSetCount * sizeof(Engine::Graphics::Structures::sIndexSet16));

				for (uint16_t i = 1; i <= m_indexSetCount; ++i)
				{
					lua_pushinteger(luaState, i);
					lua_gettable(luaState, -2);

					// Get three indices at a time into the index buffer container
					lua_pushstring(luaState, "a");
					lua_gettable(luaState, -2);
					m_indexSet16[indexBufferIndex - 1].a = static_cast<uint16_t>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "b");
					lua_gettable(luaState, -2);
#if defined D3D_API 
					m_indexSet16[indexBufferIndex - 1].c = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#elif defined OGL_API 
					m_indexSet16[indexBufferIndex - 1].b = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#endif
					lua_pop(luaState, 1);

					lua_pushstring(luaState, "c");
					lua_gettable(luaState, -2);
#if defined D3D_API 
					m_indexSet16[indexBufferIndex - 1].b = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#elif defined OGL_API 
					m_indexSet16[indexBufferIndex - 1].c = static_cast<uint16_t>(lua_tonumber(luaState, -1));
#endif
					indexBufferIndex++;
					lua_pop(luaState, 1);
					lua_pop(luaState, 1);
				}
				lua_pop(luaState, 1);
			}
		}
	}

OnExit:
	if (luaState)
	{
		lua_close(luaState);
		luaState = nullptr;
	}

	Initialize();
}

Engine::Graphics::Assets::cMesh::~cMesh()
{
	CleanUp();
}