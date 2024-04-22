// Header Files
//=============

#include "cGame.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Graphics/Assets/cMaterial.h"
#include "../../Engine/Graphics/Assets/cMesh.h"
#include "../../Engine/Shared/cCamera.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Math/Functions.h"
#include "../../Engine/Shared/MouseParams.h"
#include "../../Engine/Math/glm/gtc/quaternion.hpp"

// Interface
//==========

namespace
{
	const float OFFSET = 8.0f;
	float angle = 0.0f;

	Engine::Graphics::Assets::cMaterial* crateMat;
	Engine::Graphics::Assets::cMaterial* lavaMat;
	Engine::Graphics::Assets::cMaterial* monkeyMat;
	
	Engine::Shared::cGameObject* crate;
	Engine::Shared::cGameObject* lava;
	Engine::Shared::cGameObject* monkey;
	Engine::Shared::cCamera* camera;

	bool Move(Engine::Shared::cGameObject* i_object);
	bool Rotate(Engine::Shared::cGameObject* i_object);
}

// Initialization / Clean Up
//--------------------------

Game::MyGame::cGame::~cGame()
{
	CleanUp();
}

// Inherited Implementation
//=========================

// Initialization / Clean Up
//--------------------------



bool Game::MyGame::cGame::Initialize()
{
	crateMat = new Engine::Graphics::Assets::cMaterial("mesh/crate");
	lavaMat = new Engine::Graphics::Assets::cMaterial("mesh/lava");
	monkeyMat = new Engine::Graphics::Assets::cMaterial("mesh/monkey");
	
	crate = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("cube"), crateMat);
	lava = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("plane"), lavaMat);
	monkey = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("suzanne"), monkeyMat);
	camera = new Engine::Shared::cCamera("flycamera");

	lava->m_transform.scale *= 5.0f;
	monkey->m_transform.position += 0.25f * Engine::Math::up;

	return true;
}

bool Game::MyGame::cGame::Update()
{
	Engine::Graphics::SubmitGameObject(crate);
	//Engine::Graphics::SubmitGameObject(lava);
	//Engine::Graphics::SubmitGameObject(monkey);
	Engine::Graphics::SubmitGameObject(camera);

	if (Engine::UserInput::IsKeyPressed(VK_RSHIFT))
	{
		monkey->Move(OFFSET * Engine::Time::DeltaTime());
	}
	else
	{
		camera->Move(OFFSET * Engine::Time::DeltaTime());
	}

	if (Engine::Shared::MouseParams::mouseMoved)
	{
		Engine::Shared::MouseParams::mouseMoved = false;
		angle = glm::radians(camera->m_rotationSpeed);
		camera->Rotate(glm::angleAxis(angle, Engine::Shared::MouseParams::verticalAxis));
	}

	angle = glm::radians(64.0f) * Engine::Time::DeltaTime();
	monkey->Rotate(angle, monkey->m_transform.up);
	return true;
}

bool Game::MyGame::cGame::CleanUp()
{
	return true;
}

namespace
{
	bool Move(Engine::Shared::cGameObject* i_object)
	{
		float offset = 0;
		if (dynamic_cast<Engine::Shared::cCamera*>(i_object))
		{
			Engine::Shared::cCamera* i_camera = dynamic_cast<Engine::Shared::cCamera*>(i_object);
			offset = i_camera->m_movementSpeed;
			i_camera->Move(offset);
		}
		if (Engine::UserInput::IsKeyPressed(VK_RSHIFT))
		{
			offset = OFFSET;
			i_object->Move(offset);
		}
		return true;
	}

	bool Rotate(Engine::Shared::cGameObject* i_object)
	{
		float offset = 0;
		if (dynamic_cast<Engine::Shared::cCamera*>(i_object))
		{
			Engine::Shared::cCamera* i_camera = dynamic_cast<Engine::Shared::cCamera*>(i_object);
			offset = i_camera->m_rotationSpeed;
			i_object->Rotate(offset, Engine::Shared::MouseParams::verticalAxis);
		}
		else
		{
			offset = OFFSET;
			i_object->Rotate(offset, i_object->m_transform.up);
		}
		return true;
	}
}