#include "cPlayer.h"

#include "../../../External/Lua/Includes.h"
#include "../../../Engine/Asserts/Asserts.h"
#include "../../../Engine/Graphics/Assets/Debug/cLine.h"
#include "../../../Engine/Graphics/Assets/Debug/cSphere.h"
#include "../../../Engine/Graphics/Assets/cMaterial.h"
#include "../../../Engine/Graphics/Assets/cMesh.h"
#include "../../../Engine/Graphics/Structures/sColor.h"
#include "../../../Engine/Graphics/Structures/sVertex.h"
#include "../../../Engine/Graphics/Graphics.h"
#include "../../../Engine/Time/Time.h"
#include "../../../Engine/UserInput/UserInput.h"
#include "../../../Engine/Shared/sRigidbody.h"
#include "../../../Engine/Shared/cCamera.h"
#include "../../../Engine/Math/Functions.h"

#include "../../../Engine/Shared/Physics.h"

#include <vector>

Game::MyGame::cPlayer::cPlayer() : Engine::Shared::cGameObject()
{
	m_camera = new Engine::Shared::cCamera("playercamera");
	m_camera->m_transform = m_transform;

	Engine::Graphics::Assets::cMaterial* debugMaterial = new Engine::Graphics::Assets::cMaterial("debug");

	m_headSphereMesh = new Engine::Graphics::Assets::Debug::cSphere(Engine::Graphics::Structures::sColor8{ 255, 255, 0, 255 });
	m_bodySphereMesh = new Engine::Graphics::Assets::Debug::cSphere(Engine::Graphics::Structures::sColor8{ 255, 255, 0, 255 });

	m_xzAxesMesh = new Engine::Graphics::Assets::Debug::cLine(glm::zero<glm::vec3>(), m_transform.position);
	m_yAxisMesh = new Engine::Graphics::Assets::Debug::cLine(glm::zero<glm::vec3>(), m_transform.position);

	m_xzAxesMesh->SetColor(0, Engine::Graphics::Structures::sColor8{ 255, 0, 0, 255 });
	m_xzAxesMesh->SetColor(1, Engine::Graphics::Structures::sColor8{ 255, 0, 0, 255 });

	m_yAxisMesh->SetColor(0, Engine::Graphics::Structures::sColor8{ 0, 255, 0, 255 });
	m_yAxisMesh->SetColor(1, Engine::Graphics::Structures::sColor8{ 0, 255, 0, 255 });

	m_xzAxes = new Engine::Shared::cGameObject(m_xzAxesMesh, debugMaterial);
	m_yAxis = new Engine::Shared::cGameObject(m_yAxisMesh, debugMaterial);

	m_headSphere = new Engine::Shared::cGameObject(m_headSphereMesh, debugMaterial);
	m_bodySphere = new Engine::Shared::cGameObject(m_bodySphereMesh, debugMaterial);

	m_headSphere->m_transform.position = m_transform.position + glm::vec3(0, -5.0f, 0);
	m_bodySphere->m_transform.position = m_transform.position + glm::vec3(0, -7.5f, 0);

	m_headSphere->m_transform.scale = m_transform.scale;
	m_bodySphere->m_transform.scale = 1.75f * m_transform.scale;
}

Game::MyGame::cPlayer::cPlayer(const std::string& path) : cPlayer()
{
	lua_State* io_luaState = luaL_newstate();
	if (io_luaState == NULL)
	{
		ASSERT("Lua state wasn't created!");
	}

	int result = luaL_dofile(io_luaState, std::string("data\\gameobjects\\" + path + ".gameobject").c_str());
	if (result != LUA_OK)
	{
		ASSERT("There was a problem reading the lua file!");
	}

	// Read in file contents

	lua_pushstring(io_luaState, "name");
	lua_gettable(io_luaState, -2);
	m_name = lua_tostring(io_luaState, -1);
	lua_pop(io_luaState, 1);
	
	lua_pushstring(io_luaState, "active");
	lua_gettable(io_luaState, -2);
	m_active = lua_toboolean(io_luaState, -1) ? true : false;
	m_camera->m_active = m_active;
	lua_pop(io_luaState, 1);

	lua_pushstring(io_luaState, "height");
	lua_gettable(io_luaState, -2);
	m_height = static_cast<float>(lua_tonumber(io_luaState, -1));
	lua_pop(io_luaState, 1);

	lua_pushstring(io_luaState, "width");
	lua_gettable(io_luaState, -2);
	m_width = static_cast<float>(lua_tonumber(io_luaState, -1));
	lua_pop(io_luaState, 1);

	// Player Transform
	{
		// Position
		{
			glm::vec3 position;

			lua_pushstring(io_luaState, "transform");
			lua_gettable(io_luaState, -2);

			lua_pushstring(io_luaState, "position");
			lua_gettable(io_luaState, -2);

			lua_pushstring(io_luaState, "x");
			lua_gettable(io_luaState, -2);
			position.x = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);

			lua_pushstring(io_luaState, "y");
			lua_gettable(io_luaState, -2);
			position.y = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);

			lua_pushstring(io_luaState, "z");
			lua_gettable(io_luaState, -2);
			position.z = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);
			lua_pop(io_luaState, 1);

			m_transform.position = position;
		}
		// Orientation
		{
			float angle;
			glm::vec3 axis;

			lua_pushstring(io_luaState, "orientation");
			lua_gettable(io_luaState, -2);

			lua_pushstring(io_luaState, "angle");
			lua_gettable(io_luaState, -2);
			angle = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);

			lua_pushstring(io_luaState, "axis");
			lua_gettable(io_luaState, -2);

			lua_pushstring(io_luaState, "x");
			lua_gettable(io_luaState, -2);
			axis.x = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);

			lua_pushstring(io_luaState, "y");
			lua_gettable(io_luaState, -2);
			axis.y = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);

			lua_pushstring(io_luaState, "z");
			lua_gettable(io_luaState, -2);
			axis.z = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);
			lua_pop(io_luaState, 1);
			lua_pop(io_luaState, 1);

			m_transform.orientation = glm::angleAxis(glm::radians(angle), axis);
		}
		// Scale
		{
			glm::vec3 scale;

			lua_pushstring(io_luaState, "scale");
			lua_gettable(io_luaState, -2);

			lua_pushstring(io_luaState, "x");
			lua_gettable(io_luaState, -2);
			scale.x = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);

			lua_pushstring(io_luaState, "y");
			lua_gettable(io_luaState, -2);
			scale.y = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);

			lua_pushstring(io_luaState, "z");
			lua_gettable(io_luaState, -2);
			scale.z = (float)lua_tonumber(io_luaState, -1);
			lua_pop(io_luaState, 1);
			lua_pop(io_luaState, 1);
			lua_pop(io_luaState, 1);

			m_transform.scale = scale;
		}
	}

	// Player RigidBody
	{
		float mass;
		bool applyGravity;
		glm::vec3 gravity;

		lua_pushstring(io_luaState, "rigidbody");
		lua_gettable(io_luaState, -2);
		lua_pushstring(io_luaState, "mass");
		lua_gettable(io_luaState, -2);
		mass = ((float)lua_tonumber(io_luaState, -1));
		lua_pop(io_luaState, 1);

		m_rigidbody.m_mass = mass;

		lua_pushstring(io_luaState, "applyGravity");
		lua_gettable(io_luaState, -2);
		applyGravity = lua_toboolean(io_luaState, -1) ? true : false;
		lua_pop(io_luaState, 1);

		m_rigidbody.m_applyGravity = applyGravity;

		lua_pushstring(io_luaState, "gravity");
		lua_gettable(io_luaState, -2);
		lua_pushstring(io_luaState, "x");
		lua_gettable(io_luaState, -2);
		gravity.x = ((float)lua_tonumber(io_luaState, -1));
		lua_pop(io_luaState, 1);

		lua_pushstring(io_luaState, "y");
		lua_gettable(io_luaState, -2);
		gravity.y = ((float)lua_tonumber(io_luaState, -1));
		lua_pop(io_luaState, 1);

		lua_pushstring(io_luaState, "z");
		lua_gettable(io_luaState, -2);
		gravity.z = ((float)lua_tonumber(io_luaState, -1));
		lua_pop(io_luaState, 1);
		lua_pop(io_luaState, 1);
		m_rigidbody.gravity = gravity;

		lua_pushstring(io_luaState, "speed");
		lua_gettable(io_luaState, -2);
		m_rigidbody.m_speed = ((float)lua_tonumber(io_luaState, -1));
		lua_pop(io_luaState, 1);
	}
}

Game::MyGame::cPlayer::~cPlayer() {}

bool Game::MyGame::cPlayer::CheckHorizontalCollisions()
{
	glm::vec3 p(m_xzAxesMesh->m_vertices[0].position.x, m_xzAxesMesh->m_vertices[0].position.y, m_xzAxesMesh->m_vertices[0].position.z);
	glm::vec3 q(m_xzAxesMesh->m_vertices[1].position.x, m_xzAxesMesh->m_vertices[1].position.y, m_xzAxesMesh->m_vertices[1].position.z);

	glm::vec3 collisionPoint;
	glm::vec3 norm;

	if (Engine::Shared::Physics::TriangleRaycast(this, true, p, q, norm, collisionPoint))
	{
		m_rigidbody.velocity.x += m_rigidbody.m_speed * norm.x;
		m_rigidbody.velocity.z += m_rigidbody.m_speed * norm.z;

		return true;
	}
	return false;
}

bool Game::MyGame::cPlayer::CheckInclinedCollisions()
{
	glm::vec3 p(m_yAxisMesh->m_vertices[0].position.x, m_yAxisMesh->m_vertices[0].position.y, m_yAxisMesh->m_vertices[0].position.z);
	glm::vec3 q(m_yAxisMesh->m_vertices[1].position.x, m_yAxisMesh->m_vertices[1].position.y, m_yAxisMesh->m_vertices[1].position.z);

	glm::vec3 collisionPoint;
	glm::vec3 norm;

	if (Engine::Shared::Physics::TriangleRaycast(this, true, p, q, norm, collisionPoint))
	{
		norm = glm::normalize(norm);

		float dot = glm::dot(norm, m_camera->m_transform.forward);

		if (dot == 0)
		{
			Engine::Shared::Physics::ApplyLinearForce(this, m_rigidbody.m_speed * m_camera->m_transform.up, Engine::Time::DeltaTime());
		}

		if (dot < 0)
		{
			if (Engine::UserInput::IsKeyPressed('W'))
			{
				m_rigidbody.velocity += m_rigidbody.m_speed * (m_camera->m_transform.forward + norm);
			}
			if (Engine::UserInput::IsKeyPressed('S'))
			{
				m_rigidbody.velocity -= m_rigidbody.m_speed * (m_camera->m_transform.forward + norm);
			}
		}

		if (dot > 0)
		{
			if (Engine::UserInput::IsKeyPressed('W'))
			{
				m_rigidbody.velocity += m_rigidbody.m_speed * (m_camera->m_transform.forward - norm);
			}
			if (Engine::UserInput::IsKeyPressed('S'))
			{
				m_rigidbody.velocity -= m_rigidbody.m_speed * (m_camera->m_transform.forward - norm);
			}
		}

		return true;
	}
	return false;
}

bool Game::MyGame::cPlayer::CheckVerticalCollisions()
{
	glm::vec3 p(m_yAxisMesh->m_vertices[0].position.x, m_yAxisMesh->m_vertices[0].position.y, m_yAxisMesh->m_vertices[0].position.z);
	glm::vec3 q(m_yAxisMesh->m_vertices[1].position.x, m_yAxisMesh->m_vertices[1].position.y, m_yAxisMesh->m_vertices[1].position.z);

	glm::vec3 collisionPoint;
	glm::vec3 norm;

	return Engine::Shared::Physics::TriangleRaycast(this, false, p, q, norm, collisionPoint);
}

bool Game::MyGame::cPlayer::Update()
{
	m_camera->m_active = m_active;
	m_transform.position += m_rigidbody.velocity;
	m_camera->m_transform.position = m_headSphere->m_transform.position;
	m_headSphere->m_transform.position = m_transform.position + glm::vec3(0, -5.0f, 0);
	m_bodySphere->m_transform.position = m_transform.position + glm::vec3(0, -7.5f, 0);

	//m_xzAxesMesh->SetPosition(0, m_camera->m_transform.position - 6.25f * m_camera->m_transform.forward);
	//m_yAxisMesh->SetPosition(0, m_camera->m_transform.position - 6.25f * m_camera->m_transform.up);

	m_yAxisMesh->SetPosition(1, m_camera->m_transform.position - m_height * m_camera->m_transform.up);

	if (Engine::UserInput::IsKeyPressed('W'))
	{
		m_xzAxesMesh->SetPosition(1, m_camera->m_transform.position + 12.5f * m_camera->m_transform.forward);
		m_yAxisMesh->SetPosition(1, m_camera->m_transform.position);
	}

	if (Engine::UserInput::IsKeyPressed('S'))
	{
		m_xzAxesMesh->SetPosition(1, m_camera->m_transform.position - 12.5f * m_camera->m_transform.forward);
		m_yAxisMesh->SetPosition(1, m_camera->m_transform.position);
	}

	if (Engine::UserInput::IsKeyPressed('A'))
	{
		m_xzAxesMesh->SetPosition(1, m_camera->m_transform.position - 12.5f * m_camera->m_transform.right);
		m_yAxisMesh->SetPosition(1, m_camera->m_transform.position);
	}

	if (Engine::UserInput::IsKeyPressed('D'))
	{
		m_xzAxesMesh->SetPosition(1, m_camera->m_transform.position + 12.5f * m_camera->m_transform.right);
		m_yAxisMesh->SetPosition(1, m_camera->m_transform.position);
	}

	if (Engine::UserInput::IsKeyPressed(VK_SPACE))
	{
		m_yAxisMesh->SetPosition(1, m_camera->m_transform.position);
	}

	return true;
}

bool Game::MyGame::cPlayer::Render()
{
	Engine::Graphics::SubmitGameObject(m_xzAxes);
	Engine::Graphics::SubmitGameObject(m_yAxis);
	Engine::Graphics::SubmitGameObject(m_yAxis);
	Engine::Graphics::SubmitGameObject(m_headSphere);
	Engine::Graphics::SubmitGameObject(m_bodySphere);
	return true;
}

