// Header Files
//=============

#include "cMyGame.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Graphics/Assets/cMesh.h"
#include "../../Engine/Shared/cCamera.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Math/Functions.h"
#include "../../Engine/Shared/MouseParams.h"

// Interface
//==========

namespace
{
	const float SPEED = 8.0f;

	Engine::Shared::cGameObject* cube;
	Engine::Shared::cGameObject* plane;
	Engine::Shared::cCamera* camera;

	bool Move(Engine::Shared::cGameObject* i_object);
	bool Rotate(Engine::Shared::cGameObject* i_object, float i_angle, const glm::vec3& i_axis);
}

// Initialization / Clean Up
//--------------------------

Game::MyGame::cMyGame::~cMyGame()
{
	CleanUp();
}

// Inherited Implementation
//=========================

// Initialization / Clean Up
//--------------------------

bool Game::MyGame::cMyGame::Initialize()
{
	cube = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("cube"));
	plane = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("plane"));
	camera = new Engine::Shared::cCamera("flycamera");

	cube->m_transform.position += 0.25f * Engine::Math::up;
	plane->m_transform.scale *= 5.0f;

	return true;
}

bool Game::MyGame::cMyGame::Update()
{
	Engine::Graphics::SubmitGameObject(cube);
	Engine::Graphics::SubmitGameObject(plane);

	Move(cube);
	Rotate(cube, -50.0f, Engine::Math::up);

	Move(camera);

	if (Engine::Shared::MouseParams::mouseMoved)
	{
		Engine::Shared::MouseParams::mouseMoved = false;
		Rotate(camera, 0, Engine::Shared::MouseParams::verticalAxis);
	}
	Engine::Graphics::SubmitGameObject(camera);
	return true;
}

bool Game::MyGame::cMyGame::CleanUp()
{
	return true;
}

namespace
{
	bool Move(Engine::Shared::cGameObject* i_object)
	{
		if (dynamic_cast<Engine::Shared::cCamera*>(i_object))
		{
			Engine::Shared::cCamera* i_camera = dynamic_cast<Engine::Shared::cCamera*>(i_object);

			const float CAMERAOFFSET = i_camera->m_movementSpeed * Engine::Time::DeltaTime();

			if (!Engine::UserInput::IsKeyPressed(VK_RSHIFT))
			{
				if (Engine::UserInput::IsKeyPressed('A'))
				{
					i_object->m_transform.position -= CAMERAOFFSET * i_object->m_transform.right;
				}
				if (Engine::UserInput::IsKeyPressed('D'))
				{
					i_object->m_transform.position += CAMERAOFFSET * i_object->m_transform.right;
				}
				if (Engine::UserInput::IsKeyPressed('W'))
				{
					i_object->m_transform.position += CAMERAOFFSET * i_object->m_transform.up;
				}
				if (Engine::UserInput::IsKeyPressed('S'))
				{
					i_object->m_transform.position -= CAMERAOFFSET * i_object->m_transform.up;
				}
				if (Engine::UserInput::IsKeyPressed('Q'))
				{
					i_object->m_transform.position -= CAMERAOFFSET * i_object->m_transform.forward;
				}
				if (Engine::UserInput::IsKeyPressed('E'))
				{
					i_object->m_transform.position += CAMERAOFFSET * i_object->m_transform.forward;
				}
			}
		}
		else
		{
			const float OFFSET = SPEED * Engine::Time::DeltaTime();
			if (Engine::UserInput::IsKeyPressed(VK_RSHIFT))
			{
				if (Engine::UserInput::IsKeyPressed('A'))
				{
					i_object->m_transform.position -= OFFSET * i_object->m_transform.right;
				}
				if (Engine::UserInput::IsKeyPressed('D'))
				{
					i_object->m_transform.position += OFFSET * i_object->m_transform.right;
				}
				if (Engine::UserInput::IsKeyPressed('W'))
				{
					i_object->m_transform.position += OFFSET * i_object->m_transform.up;
				}
				if (Engine::UserInput::IsKeyPressed('S'))
				{
					i_object->m_transform.position -= OFFSET * i_object->m_transform.up;
				}
				if (Engine::UserInput::IsKeyPressed('Q'))
				{
					i_object->m_transform.position -= OFFSET * i_object->m_transform.forward;
				}
				if (Engine::UserInput::IsKeyPressed('E'))
				{
					i_object->m_transform.position += OFFSET * i_object->m_transform.forward;
				}
			}
		}

		return true;
	}

	bool Rotate(Engine::Shared::cGameObject* i_object, float i_angle, const glm::vec3& i_axis)
	{
		if (dynamic_cast<Engine::Shared::cCamera*>(i_object))
		{
			Engine::Shared::cCamera* i_camera = dynamic_cast<Engine::Shared::cCamera*>(i_object);

			glm::quat rotation(glm::angleAxis(glm::radians(i_camera->m_rotationSpeed), Engine::Shared::MouseParams::verticalAxis));
			i_camera->m_transform.orientation = i_camera->m_transform.orientation * rotation;
			i_camera->m_transform.orientation = glm::normalize(i_camera->m_transform.orientation);

			i_camera->m_transform.right = rotation * i_camera->m_transform.right;
			i_camera->m_transform.up = rotation * i_camera->m_transform.up;
			i_camera->m_transform.forward = rotation * i_camera->m_transform.forward;
		}
		else
		{
			const float angle = glm::radians(i_angle) * Engine::Time::DeltaTime();
			glm::quat rotation(glm::angleAxis(angle, i_axis));
			i_object->m_transform.orientation = i_object->m_transform.orientation * rotation;
		}
		return true;
	}

}
