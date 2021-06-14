#include "sPlayer.h"

#include "../../../External/Lua/Includes.h"
#include "../../../Engine/Asserts/Asserts.h"
#include "../../../Engine/Graphics/Assets/Debug/cLine.h"
#include "../../../Engine/Graphics/Assets/Debug/cSphere.h"
#include "../../../Engine/Graphics/Assets/cMaterial.h"
#include "../../../Engine/Graphics/Assets/cMesh.h"
#include "../../../Engine/Graphics/Graphics.h"
#include "../../../Engine/Time/Time.h"
#include "../../../Engine/UserInput/UserInput.h"
#include "../../../Engine/Shared/sRigidbody.h"
#include "../../../Engine/Shared/cCamera.h"
#include "../../../Engine/Math/Functions.h"

#include "../../../Engine/Graphics/Structures/sColor.h"
#include "../../../Engine/Graphics/Structures/sVertex.h"
#include "../../../Engine/Shared/Physics.h"

#include <vector>

Game::MyGame::sPlayer::sPlayer() : Engine::Shared::cGameObject()
{
	m_camera = new Engine::Shared::cCamera("playercamera");
	m_camera->m_transform = m_transform;

	Engine::Graphics::Assets::cMaterial* debugMat = new Engine::Graphics::Assets::cMaterial("debug");

	m_headSphereMesh = new Engine::Graphics::Assets::Debug::cSphere(Engine::Graphics::Structures::sColor8{ 255, 255, 0, 255 });
	m_bodySphereMesh = new Engine::Graphics::Assets::Debug::cSphere(Engine::Graphics::Structures::sColor8{ 255, 255, 0, 255 });

	m_hAxisMesh = new Engine::Graphics::Assets::Debug::cLine(Engine::Math::cVector::zero, m_transform.position);
	m_vAxisMesh = new Engine::Graphics::Assets::Debug::cLine(Engine::Math::cVector::zero, m_transform.position);

	m_hAxisMesh->SetColor(0, Engine::Graphics::Structures::sColor8{ 255, 0, 0, 255 });
	m_hAxisMesh->SetColor(1, Engine::Graphics::Structures::sColor8{ 255, 0, 0, 255 });

	m_vAxisMesh->SetColor(0, Engine::Graphics::Structures::sColor8{ 0, 255, 0, 255 });
	m_vAxisMesh->SetColor(1, Engine::Graphics::Structures::sColor8{ 0, 255, 0, 255 });

	m_hAxis = new Engine::Shared::cGameObject(m_hAxisMesh, debugMat);
	m_vAxis = new Engine::Shared::cGameObject(m_vAxisMesh, debugMat);

	m_headSphere = new Engine::Shared::cGameObject(m_headSphereMesh, debugMat);
	m_bodySphere = new Engine::Shared::cGameObject(m_bodySphereMesh, debugMat);

	m_headSphere->m_transform.position = m_transform.position + Engine::Math::cVector(0, -35.0f, 0);
	m_bodySphere->m_transform.position = m_transform.position + Engine::Math::cVector(0, -40.0f, 0);

	m_headSphere->m_transform.scale *= 2.5f;
	m_bodySphere->m_transform.scale *= 5.0f;
}

Game::MyGame::sPlayer::sPlayer(const std::string& path) : sPlayer()
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
			Engine::Math::cVector position;

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
			Engine::Math::cVector axis;

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

			m_transform.orientation = Engine::Math::cQuaternion(Engine::Math::ConvertDegreesToRadians(angle), axis);
		}
		// Scale
		{
			Engine::Math::cVector scale;

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
		float maxSpeed;
		bool applyGravity;
		Engine::Math::cVector gravity;

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

		lua_pushstring(io_luaState, "maxSpeed");
		lua_gettable(io_luaState, -2);
		maxSpeed = ((float)lua_tonumber(io_luaState, -1));
		lua_pop(io_luaState, 1);
		lua_pop(io_luaState, 1);

		m_rigidbody.m_maxSpeed = maxSpeed;
	}
}

Game::MyGame::sPlayer::~sPlayer() {}

bool Game::MyGame::sPlayer::CheckHorizontalCollisions()
{
	Engine::Math::cVector p = Engine::Math::cVector(m_hAxisMesh->m_vertices[0].position.x, m_hAxisMesh->m_vertices[0].position.y, m_hAxisMesh->m_vertices[0].position.z);
	Engine::Math::cVector q = Engine::Math::cVector(m_hAxisMesh->m_vertices[1].position.x, m_hAxisMesh->m_vertices[1].position.y, m_hAxisMesh->m_vertices[1].position.z);

	Engine::Math::cVector collisionPoint;
	Engine::Math::cVector norm;

	if (Engine::Shared::Physics::Raycast(this, true, p, q, norm, collisionPoint))
	{
		m_rigidbody.velocity.x += norm.x;
		m_rigidbody.velocity.z += norm.z;

		return true;
	}
	return false;
}

bool Game::MyGame::sPlayer::CheckInclinedCollisions()
{
	Engine::Math::cVector p = Engine::Math::cVector(m_vAxisMesh->m_vertices[0].position.x, m_vAxisMesh->m_vertices[0].position.y, m_vAxisMesh->m_vertices[0].position.z);
	Engine::Math::cVector q = Engine::Math::cVector(m_vAxisMesh->m_vertices[1].position.x, m_vAxisMesh->m_vertices[1].position.y, m_vAxisMesh->m_vertices[1].position.z);

	Engine::Math::cVector collisionPoint;
	Engine::Math::cVector norm;

	if (Engine::Shared::Physics::Raycast(this, true, p, q, norm, collisionPoint))
	{
		norm = norm.CreateNormalized();

		float dot = Engine::Math::Dot(norm, m_camera->m_transform.forward);

		if (dot == 0)
		{
			Engine::Shared::Physics::ApplyLinearForce(this, m_camera->m_transform.up, Engine::Time::DeltaTime());
		}

		if (dot < 0)
		{
			if (Engine::UserInput::IsKeyPressed('W'))
			{
				m_rigidbody.velocity += m_camera->m_transform.forward + norm;
			}
			if (Engine::UserInput::IsKeyPressed('S'))
			{
				m_rigidbody.velocity -= m_camera->m_transform.forward + norm;
			}
		}

		if (dot > 0)
		{
			if (Engine::UserInput::IsKeyPressed('W'))
			{
				m_rigidbody.velocity += m_camera->m_transform.forward - norm;
			}
			if (Engine::UserInput::IsKeyPressed('S'))
			{
				m_rigidbody.velocity -= m_camera->m_transform.forward - norm;
			}
		}

		return true;
	}
	return false;
}

bool Game::MyGame::sPlayer::CheckVerticalCollisions()
{
	Engine::Math::cVector p = Engine::Math::cVector(m_vAxisMesh->m_vertices[0].position.x, m_vAxisMesh->m_vertices[0].position.y, m_vAxisMesh->m_vertices[0].position.z);
	Engine::Math::cVector q = Engine::Math::cVector(m_vAxisMesh->m_vertices[1].position.x, m_vAxisMesh->m_vertices[1].position.y, m_vAxisMesh->m_vertices[1].position.z);

	Engine::Math::cVector collisionPoint;
	Engine::Math::cVector norm;

	return Engine::Shared::Physics::Raycast(this, false, p, q, norm, collisionPoint);
}

bool Game::MyGame::sPlayer::Update()
{
	m_camera->m_active = m_active;

	m_transform.position += m_rigidbody.velocity;
	m_headSphere->m_transform.position = m_transform.position + Engine::Math::cVector(0, -35.0f, 0);
	m_bodySphere->m_transform.position = m_transform.position + Engine::Math::cVector(0, -40.0f, 0);
	m_camera->m_transform.position = m_headSphere->m_transform.position + Engine::Math::cVector(0, 0.0625f, 0);

	m_hAxisMesh->SetPosition(0, m_camera->m_transform.position);

	m_vAxisMesh->SetPosition(0, m_camera->m_transform.position);
	m_vAxisMesh->SetPosition(1, m_camera->m_transform.position - 50.0f * m_camera->m_transform.up);

	if (Engine::UserInput::IsKeyPressed('W'))
	{
		m_hAxisMesh->SetPosition(1, m_camera->m_transform.position + 12.5f * m_camera->m_transform.forward);
	}

	if (Engine::UserInput::IsKeyPressed('S'))
	{
		m_hAxisMesh->SetPosition(1, m_camera->m_transform.position - 12.5f * m_camera->m_transform.forward);
	}

	if (Engine::UserInput::IsKeyPressed('A'))
	{
		m_hAxisMesh->SetPosition(1, m_camera->m_transform.position - 12.5f * m_camera->m_transform.right);
	}

	if (Engine::UserInput::IsKeyPressed('D'))
	{
		m_hAxisMesh->SetPosition(1, m_camera->m_transform.position + 12.5f * m_camera->m_transform.right);
	}

	if (Engine::UserInput::IsKeyPressed(VK_SPACE))
	{
		m_vAxisMesh->SetPosition(1, m_camera->m_transform.position);
	}

	return true;
}

bool Game::MyGame::sPlayer::Render()
{
	Engine::Graphics::SubmitGameObject(m_hAxis);
	Engine::Graphics::SubmitGameObject(m_vAxis);
	Engine::Graphics::SubmitGameObject(m_headSphere);
	Engine::Graphics::SubmitGameObject(m_bodySphere);
	return true;
}
