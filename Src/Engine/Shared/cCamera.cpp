#include "cCamera.h"
#include "../Asserts/Asserts.h"
#include "../../External/Lua/Includes.h"
#include "../UserSettings/UserSettings.h"
#include "../Math/Functions.h"

#include "../Shared/MouseParams.h"
#include "../Time/Time.h"
#include "../UserInput/UserInput.h"
#include "../Windows/Includes.h"

Engine::Shared::cCamera::cCamera(const std::string& i_fileName)
{
	m_aspectRatio = static_cast<float>(UserSettings::GetResolutionWidth()) / UserSettings::GetResolutionHeight();

	lua_State* luaState = luaL_newstate();

	if (luaState == NULL)
	{
		ASSERT("Lua state wasn't created!");
	}

	if (!i_fileName.empty())
	{
		std::string fullPath = "data/gameobjects/" + i_fileName + ".gameobject";

		int result = luaL_dofile(luaState, fullPath.c_str());

		if (result != LUA_OK)
		{
			ASSERT("There was a problem reading the lua file!");
		}

		// Read in file contents

		lua_pushstring(luaState, "name");
		lua_gettable(luaState, -2);
		m_name = lua_tostring(luaState, -1);
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "active");
		lua_gettable(luaState, -2);
		m_active = (lua_toboolean(luaState, -1)) ? true : false;
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "cameraType");
		lua_gettable(luaState, -2);
		m_cameraType = lua_tonumber(luaState, -1) == 1 ? Engine::Shared::FIRST_PERSON : Engine::Shared::THIRD_PERSON;
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "fieldOfView");
		lua_gettable(luaState, -2);
		m_fieldOfView = glm::radians<float>(static_cast<float>(lua_tonumber(luaState, -1)));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "nearPlane");
		lua_gettable(luaState, -2);
		m_nearPlane = static_cast<float>(lua_tonumber(luaState, -1));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "farPlane");
		lua_gettable(luaState, -2);
		m_farPlane = static_cast<float>(lua_tonumber(luaState, -1));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "movementSpeed");
		lua_gettable(luaState, -2);
		m_movementSpeed = static_cast<float>(lua_tonumber(luaState, -1));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "rotationSpeed");
		lua_gettable(luaState, -2);
		m_rotationSpeed = static_cast<float>(lua_tonumber(luaState, -1));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "transform");
		lua_gettable(luaState, -2);
		{
			glm::vec3 position;

			lua_pushstring(luaState, "position");
			lua_gettable(luaState, -2);

			lua_pushstring(luaState, "x");
			lua_gettable(luaState, -2);

			position.x = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "y");
			lua_gettable(luaState, -2);

			position.y = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "z");
			lua_gettable(luaState, -2);

			position.z = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);
			lua_pop(luaState, 1);

			m_transform.position = position;
		}

		{
			float angle;
			glm::vec3 axis;

			lua_pushstring(luaState, "orientation");
			lua_gettable(luaState, -2);

			lua_pushstring(luaState, "angle");
			lua_gettable(luaState, -2);

			angle = glm::radians<float>(static_cast<float>(lua_tonumber(luaState, -1)));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "axis");
			lua_gettable(luaState, -2);

			lua_pushstring(luaState, "x");
			lua_gettable(luaState, -2);

			axis.x = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "y");
			lua_gettable(luaState, -2);

			axis.y = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "z");
			lua_gettable(luaState, -2);

			axis.z = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);
			lua_pop(luaState, 1);
			lua_pop(luaState, 1);

			m_transform.orientation = glm::angleAxis(angle, axis);
		}

		{
			glm::vec3 scale;

			lua_pushstring(luaState, "scale");
			lua_gettable(luaState, -2);

			lua_pushstring(luaState, "x");
			lua_gettable(luaState, -2);

			scale.x = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "y");
			lua_gettable(luaState, -2);

			scale.y = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "z");
			lua_gettable(luaState, -2);

			scale.z = static_cast<float>(lua_tonumber(luaState, -1));
			lua_pop(luaState, 1);
			lua_pop(luaState, 1);

			m_transform.scale = scale;
		}
	}
}

Engine::Shared::cCamera::~cCamera() {}

bool Engine::Shared::cCamera::Move()
{
	const float OFFSET = m_movementSpeed * Time::DeltaTime();

	if (UserInput::IsKeyPressed('A'))
	{
		m_transform.position -= OFFSET * m_transform.right;
	}
	if (UserInput::IsKeyPressed('D'))
	{
		m_transform.position += OFFSET * m_transform.right;
	}
	if (UserInput::IsKeyPressed('W'))
	{
		m_transform.position += OFFSET * m_transform.up;
	}
	if (UserInput::IsKeyPressed('S'))
	{
		m_transform.position -= OFFSET * m_transform.up;
	}
	if (UserInput::IsKeyPressed('Q'))
	{
		m_transform.position -= OFFSET * m_transform.forward;
	}
	if (UserInput::IsKeyPressed('E'))
	{
		m_transform.position += OFFSET * m_transform.forward;
	}

	if (MouseParams::mouseMoved)
	{
		MouseParams::mouseMoved = false;
		Rotate();
	}

	return true;
}

bool Engine::Shared::cCamera::Rotate()
{
	glm::quat rotation(glm::angleAxis(glm::radians(m_rotationSpeed), Engine::Shared::MouseParams::verticalAxis));
	m_transform.orientation = m_transform.orientation * rotation;
	m_transform.orientation = glm::normalize(m_transform.orientation);

	m_transform.right = m_transform.right * rotation;
	m_transform.up = m_transform.up * rotation;
	m_transform.forward = m_transform.forward * rotation;

	return true;
}

bool Engine::Shared::cCamera::Update()
{
	m_worldToCamera = Math::CreateWorldToCameraTransform(m_transform);
	m_cameraToScreen = Math::CreateCameraToScreenTransform(
		m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);

	return true;
}

glm::mat4 Engine::Shared::cCamera::WorldToCamera()
{
	return m_worldToCamera;
}

glm::mat4 Engine::Shared::cCamera::CameraToScreen()
{
	return m_cameraToScreen;
}

