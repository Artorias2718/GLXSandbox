// Header Files
//=============

#include "cMyGame.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Graphics/Assets/cMesh.h"
#include "../../Engine/Graphics/Assets/cMaterial.h"
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

	Engine::Graphics::Assets::cMaterial* red;
	Engine::Graphics::Assets::cMaterial* green;
	Engine::Graphics::Assets::cMaterial* blue;
	Engine::Graphics::Assets::cMaterial* yellow;

	Engine::Shared::cGameObject* cube;
	Engine::Shared::cGameObject* icosphere;
	Engine::Shared::cGameObject* plane;
	Engine::Shared::cGameObject* suzanne;
	Engine::Shared::cCamera* camera;

	bool Move(Engine::Shared::cGameObject* i_object);
	bool Rotate(Engine::Shared::cGameObject* i_object, float i_angle, const Engine::Math::cVector& i_axis);
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
	red = new Engine::Graphics::Assets::cMaterial("red");
	green = new Engine::Graphics::Assets::cMaterial("green");
	blue = new Engine::Graphics::Assets::cMaterial("blue");
	yellow = new Engine::Graphics::Assets::cMaterial("yellow");

	cube = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("cube"), red);
	icosphere = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("icosphere"), green);
	plane = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("plane"), blue);
	suzanne = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("suzanne"), yellow);
	camera = new Engine::Shared::cCamera("flycamera");

	cube->m_transform.position += -5.0f * Engine::Math::cVector::right + 1.05f * Engine::Math::cVector::up;
	icosphere->m_transform.position += 5.0f * Engine::Math::cVector::right + 1.05f * Engine::Math::cVector::up;
	plane->m_transform.scale *= 10.0f;
	suzanne->m_transform.position += 1.25f * Engine::Math::cVector::up;

	return true;
}

bool Game::MyGame::cMyGame::Update()
{
	Engine::Graphics::SubmitGameObject(cube);
	Engine::Graphics::SubmitGameObject(icosphere);
	Engine::Graphics::SubmitGameObject(plane);
	Engine::Graphics::SubmitGameObject(suzanne);

	Move(suzanne);
	Rotate(suzanne, -50.0f, Engine::Math::cVector::up);

	Move(camera);

	if (Engine::Shared::MouseParams::mouseMoved)
	{
		Engine::Shared::MouseParams::mouseMoved = false;
		Rotate(camera, 0, Engine::Math::cVector::zero);
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

	bool Rotate(Engine::Shared::cGameObject* i_object, float i_angle, const Engine::Math::cVector& i_axis)
	{
		if (dynamic_cast<Engine::Shared::cCamera*>(i_object))
		{
			Engine::Shared::cCamera* i_camera = dynamic_cast<Engine::Shared::cCamera*>(i_object);

			Engine::Math::cQuaternion rotation(Engine::Math::ConvertDegreesToRadians(i_camera->m_rotationSpeed), Engine::Shared::MouseParams::verticalAxis);
			i_camera->m_transform.orientation = i_camera->m_transform.orientation * rotation;
			i_camera->m_transform.orientation.Normalize();

			i_camera->m_transform.right = rotation * i_camera->m_transform.right;
			i_camera->m_transform.up = rotation * i_camera->m_transform.up;
			i_camera->m_transform.forward = rotation * i_camera->m_transform.forward;
		}
		else
		{
			const float angle = Engine::Math::ConvertDegreesToRadians(i_angle) * Engine::Time::DeltaTime();
			Engine::Math::cQuaternion rotation(angle, i_axis);
			i_object->m_transform.orientation = i_object->m_transform.orientation * rotation;
		}
		return true;
	}

}
