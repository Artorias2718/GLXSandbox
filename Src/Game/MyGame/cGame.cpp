// Header Files
//=============

#include "cGame.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Graphics/Assets/cMesh.h"
#include "../../Engine/Shared/cGameObject.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"

// Interface
//==========

namespace
{
	const float SPEED = 0.25f;

	Engine::Shared::cGameObject* square;
	Engine::Shared::cGameObject* triangle;

	bool Move(Engine::Shared::cGameObject* i_object);
}

// Initialization / Clean Up
//--------------------------

Game::cGame::~cGame()
{
	CleanUp();
}

// Inherited Implementation
//=========================

// Initialization / Clean Up
//--------------------------

bool Game::cGame::Initialize()
{
	square = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("square"));
	triangle = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("triangle"));

	square->m_position.x -= 0.25f;
	triangle->m_position.x += 0.25f;

	return true;
}

bool Game::cGame::Update()
{
	Engine::Graphics::SubmitGameObject(square);
	Engine::Graphics::SubmitGameObject(triangle);

	Move(square);

	return true;
}

bool Game::cGame::CleanUp()
{
	return true;
}

namespace
{
	bool Move(Engine::Shared::cGameObject* i_object)
	{
		if (Engine::UserInput::IsKeyPressed('A'))
		{
			i_object->m_position.x -= SPEED * Engine::Time::DeltaTime();
		}
		if (Engine::UserInput::IsKeyPressed('D'))
		{
			i_object->m_position.x += SPEED * Engine::Time::DeltaTime();
		}
		if (Engine::UserInput::IsKeyPressed('W'))
		{
			i_object->m_position.y += SPEED * Engine::Time::DeltaTime();
		}
		if (Engine::UserInput::IsKeyPressed('S'))
		{
			i_object->m_position.y -= SPEED * Engine::Time::DeltaTime();
		}

		return true;
	}
}
