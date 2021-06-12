// Header Files
//=============

#include "cMyGame.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Graphics/Assets/cMesh.h"
#include "../../Engine/Graphics/Assets/Debug/cLine.h"
#include "../../Engine/Graphics/Assets/Debug/cBox.h"
#include "../../Engine/Graphics/Assets/Debug/cSphere.h"
#include "../../Engine/Graphics/Assets/Debug/cCapsule.h"
#include "../../Engine/Graphics/Assets/cMaterial.h"
#include "../../Engine/Shared/cCamera.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Math/Functions.h"
#include "../../Engine/Shared/MouseParams.h"
#include "../../Engine/Graphics/Interfaces/cRenderState/cRenderState.h"
#include "../../Engine/Shared/cDebugMenu.h"

// Interface
//==========

namespace
{
	const float SPEED = 8.0f;

	Engine::Graphics::Assets::cMaterial* boxesMat;
	Engine::Graphics::Assets::cMaterial* cementMat;
	Engine::Graphics::Assets::cMaterial* groundMat;
	Engine::Graphics::Assets::cMaterial* metalMat;
	Engine::Graphics::Assets::cMaterial* railingMat;
	Engine::Graphics::Assets::cMaterial* robotMat;
	Engine::Graphics::Assets::cMaterial* debugMat;

	Engine::Shared::cGameObject* boxes;
	Engine::Shared::cGameObject* cement;
	Engine::Shared::cGameObject* ground;
	Engine::Shared::cGameObject* metal;
	Engine::Shared::cGameObject* railing;

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
	boxesMat = new Engine::Graphics::Assets::cMaterial("boxes");
	cementMat = new Engine::Graphics::Assets::cMaterial("cement");
	groundMat = new Engine::Graphics::Assets::cMaterial("ground");
	metalMat = new Engine::Graphics::Assets::cMaterial("metal");
	railingMat = new Engine::Graphics::Assets::cMaterial("railing");
	debugMat = new Engine::Graphics::Assets::cMaterial("debug");

	boxes = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("boxes"), boxesMat);
	cement = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("cement"), cementMat);
	ground = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("ground"), groundMat);
	metal = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("metal"), metalMat);
	railing = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("railing"), railingMat);

	camera = new Engine::Shared::cCamera("flycamera");

	return true;
}

bool Game::MyGame::cMyGame::Update()
{
	Engine::Graphics::SubmitGameObject(boxes);
	Engine::Graphics::SubmitGameObject(cement);
	Engine::Graphics::SubmitGameObject(ground);
	Engine::Graphics::SubmitGameObject(metal);
	Engine::Graphics::SubmitGameObject(railing);
	
	//Move(suzanne);
	//Rotate(suzanne, -50.0f, Engine::Math::cVector::up);

	if (!Engine::Shared::cDebugMenu::Instance().m_active)
	{
		Move(camera);

		if (Engine::Shared::MouseParams::mouseMoved)
		{
			Engine::Shared::MouseParams::mouseMoved = false;
			Rotate(camera, 0, Engine::Math::cVector::zero);
		}
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

