#include "cPlayerController.h"
#include "cPlayer.h"

#include "../../../Engine/Math/Functions.h"
#include "../../../Engine/UserInput/UserInput.h"
#include "../../../Engine/Time/Time.h"
#include "../../../Engine/Graphics/Assets/Debug/cLine.h"
#include "../../../Engine/Shared/Physics.h"
#include "../../../Engine/Shared/sRigidbody.h"
#include "../../../Engine/Shared/MouseParams.h"
#include "../../../Engine/Shared/cCamera.h"
#include "../../../Engine/Windows/Includes.h"

Game::MyGame::cPlayerController::cPlayerController(cPlayer* i_player) : m_player(i_player) {}
Game::MyGame::cPlayerController::~cPlayerController() {}

bool Game::MyGame::cPlayerController::Move()
{
	float dt = Engine::Time::DeltaTime();

	if (Engine::Shared::MouseParams::mouseMoved)
	{
		Engine::Shared::MouseParams::mouseMoved = !Engine::Shared::MouseParams::mouseMoved;
		m_player->m_camera->Rotate();
	}

	if (Engine::UserInput::IsKeyPressed('W'))
	{
		m_player->m_rigidbody.velocity = m_player->m_rigidbody.m_speed * m_player->m_camera->m_transform.forward;

		m_player->CheckHorizontalCollisions();
		m_player->CheckInclinedCollisions();

		Engine::Shared::Physics::ApplyLinearForce(m_player, m_player->m_rigidbody.velocity, dt);
		m_player->Update();
	}

	if (Engine::UserInput::IsKeyPressed('S'))
	{
		m_player->m_rigidbody.velocity = -m_player->m_rigidbody.m_speed * m_player->m_camera->m_transform.forward;

		m_player->CheckHorizontalCollisions();
		m_player->CheckInclinedCollisions();

		Engine::Shared::Physics::ApplyLinearForce(m_player, m_player->m_rigidbody.velocity, dt);
		m_player->Update();
	}

	if (Engine::UserInput::IsKeyPressed('A'))
	{
		m_player->m_rigidbody.velocity = -m_player->m_rigidbody.m_speed * m_player->m_camera->m_transform.right;

		m_player->CheckHorizontalCollisions();
		m_player->CheckInclinedCollisions();

		Engine::Shared::Physics::ApplyLinearForce(m_player, m_player->m_rigidbody.velocity, dt);
		m_player->Update();
	}

	if (Engine::UserInput::IsKeyPressed('D'))
	{
		m_player->m_rigidbody.velocity = m_player->m_rigidbody.m_speed * m_player->m_camera->m_transform.right;

		m_player->CheckHorizontalCollisions();
		m_player->CheckInclinedCollisions();

		Engine::Shared::Physics::ApplyLinearForce(m_player, m_player->m_rigidbody.velocity, dt);
		m_player->Update();
	}

	if (Engine::UserInput::IsKeyPressed(VK_SPACE))
	{
		m_player->m_rigidbody.velocity = m_player->m_rigidbody.m_speed * m_player->m_camera->m_transform.up;
		Engine::Shared::Physics::ApplyLinearForce(m_player, m_player->m_rigidbody.velocity, dt);
		m_player->Update();
	}

	/*if (!Engine::UserInput::IsKeyPressed('W') && !Engine::UserInput::IsKeyPressed('S')
		&& !Engine::UserInput::IsKeyPressed('A') && !Engine::UserInput::IsKeyPressed('D')
		&& !Engine::UserInput::IsKeyPressed(VK_SPACE))
	{
		m_player->m_rigidbody.velocity = glm::zero<glm::vec3>();
		Engine::Shared::Physics::ApplyLinearForce(m_player, m_player->m_rigidbody.velocity, dt);
		m_player->Update();
	}*/

	if (m_player->m_rigidbody.m_applyGravity)
	{
		if (!m_player->CheckVerticalCollisions())
		{
			Engine::Shared::Physics::ApplyLinearForce(m_player, m_player->m_rigidbody.gravity, dt);
			m_player->Update();
		}
	}
	return true;
}
