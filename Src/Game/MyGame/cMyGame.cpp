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

// Interface
//==========

namespace
{
	const float SPEED = 8.0f;

	Engine::Graphics::Assets::cMaterial* cabooseMat;
	Engine::Graphics::Assets::cMaterial* sargeMat;

	Engine::Graphics::Assets::cMaterial* boxesMat;
	Engine::Graphics::Assets::cMaterial* cementMat;
	Engine::Graphics::Assets::cMaterial* checkpointBasesMat;
	Engine::Graphics::Assets::cMaterial* groundMat;
	Engine::Graphics::Assets::cMaterial* metalMat;
	Engine::Graphics::Assets::cMaterial* railingMat;
	Engine::Graphics::Assets::cMaterial* debugMat;

	Engine::Graphics::Assets::cMaterial* kaibaMat;

	Engine::Shared::cGameObject* caboose;
	Engine::Shared::cGameObject* sarge;

	Engine::Shared::cGameObject* boxes;
	Engine::Shared::cGameObject* cement;
	Engine::Shared::cGameObject* checkpointBases;
	Engine::Shared::cGameObject* ground;
	Engine::Shared::cGameObject* metal;
	Engine::Shared::cGameObject* railing;

	Engine::Shared::cGameObject* debugLine1;
	Engine::Shared::cGameObject* debugLine2;
	Engine::Shared::cGameObject* debugBox1;
	Engine::Shared::cGameObject* debugBox2;
	Engine::Shared::cGameObject* debugSphere1;
	Engine::Shared::cGameObject* debugSphere2;
	Engine::Shared::cGameObject* debugCapsule1;
	Engine::Shared::cGameObject* debugCapsule2;

	Engine::Shared::cGameObject* kaiba1;
	Engine::Shared::cGameObject* kaiba2;

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
	cabooseMat = new Engine::Graphics::Assets::cMaterial("caboose");
	sargeMat = new Engine::Graphics::Assets::cMaterial("sarge");

	boxesMat = new Engine::Graphics::Assets::cMaterial("boxes");
	cementMat = new Engine::Graphics::Assets::cMaterial("cement");
	checkpointBasesMat = new Engine::Graphics::Assets::cMaterial("checkpointbases");
	groundMat = new Engine::Graphics::Assets::cMaterial("ground");
	metalMat = new Engine::Graphics::Assets::cMaterial("metal");
	railingMat = new Engine::Graphics::Assets::cMaterial("railing");
	debugMat = new Engine::Graphics::Assets::cMaterial("debug");

	kaibaMat = new Engine::Graphics::Assets::cMaterial("kaiba");

	caboose = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("caboose"), cabooseMat);
	sarge = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("sarge"), sargeMat);

	boxes = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("boxes"), boxesMat);
	cement = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("cement"), cementMat);
	checkpointBases = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("checkpointbases"), checkpointBasesMat);
	ground = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("ground"), groundMat);
	metal = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("metal"), metalMat);
	railing = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("railing"), railingMat);

	// Debug
	{
		// Lines
		{
			debugLine1 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cLine(glm::vec3(-10, -20, 40), glm::vec3(10, 20, -40), { 255, 0, 0, 255 }), debugMat);
			debugLine2 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cLine(glm::vec3(40, -20, 10), glm::vec3(-40, 20, 10), { 0, 0, 255, 255 }), debugMat);
		}
		// Boxes
		{
			debugBox1 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cBox({ 255, 255, 0, 255 }), debugMat);
			debugBox2 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cBox({ 255, 0, 255, 255 }), debugMat);
		
			debugBox1->m_transform.position = glm::vec3(-20, 0, 0);
			debugBox2->m_transform.position = glm::vec3(20, 0, 0);
		
			debugBox1->m_transform.orientation = debugBox1->m_transform.orientation * glm::angleAxis(glm::radians(90.0f), -Engine::Math::forward);
			debugBox2->m_transform.orientation = debugBox2->m_transform.orientation * glm::angleAxis(glm::radians(90.0f), Engine::Math::forward);
		
			debugBox1->m_transform.scale = glm::vec3(20, 15, 10);
			debugBox2->m_transform.scale = glm::vec3(20, 15, 10);
		}
		// Spheres 
		{
			debugSphere1 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cSphere({ 250, 95, 0, 255 }), debugMat);
			debugSphere2 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cSphere({ 0, 255, 0, 255 }), debugMat);
		
			debugSphere1->m_transform.position = glm::vec3(0, -15, 0);
			debugSphere2->m_transform.position = glm::vec3(0, 15, 0);
		
			debugSphere1->m_transform.scale = 5.0f * glm::one<glm::vec3>();
			debugSphere2->m_transform.scale = 5.0f * glm::one<glm::vec3>();
		}
		// Capsules 
		{
			debugCapsule1 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cCapsule(5, { 255, 128, 0, 255 }), debugMat);
			debugCapsule2 = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::Debug::cCapsule(5, { 128, 0, 255, 255 }), debugMat);
		
			debugCapsule1->m_transform.position = glm::vec3(-10, 20, 0);
			debugCapsule2->m_transform.position = glm::vec3(10, 5, 0);
		
			debugCapsule1->m_transform.orientation = glm::angleAxis(glm::radians(45.0f), Engine::Math::forward);
			debugCapsule2->m_transform.orientation = glm::angleAxis(glm::radians(45.0f), -Engine::Math::forward);
		}
	}

	kaiba1 = new Engine::Shared::cGameObject(kaibaMat, glm::vec2(-675.0f, -675.0f), glm::vec2(-0.5f, 0.5f));
	kaiba2 = new Engine::Shared::cGameObject(kaibaMat, glm::vec2(675.0f, -675.0f), glm::vec2(0.5f, 0.5f));

	camera = new Engine::Shared::cCamera("flycamera");

	return true;
}

bool Game::MyGame::cMyGame::Update()
{
	Engine::Graphics::SubmitGameObject(caboose);
	Engine::Graphics::SubmitGameObject(sarge);

	Engine::Graphics::SubmitGameObject(boxes);
	Engine::Graphics::SubmitGameObject(cement);
	Engine::Graphics::SubmitGameObject(checkpointBases);
	Engine::Graphics::SubmitGameObject(ground);
	Engine::Graphics::SubmitGameObject(metal);
	Engine::Graphics::SubmitGameObject(railing);

	Engine::Graphics::SubmitGameObject(debugLine1);
	Engine::Graphics::SubmitGameObject(debugLine2);
	Engine::Graphics::SubmitGameObject(debugBox1);
	Engine::Graphics::SubmitGameObject(debugBox2);
	Engine::Graphics::SubmitGameObject(debugSphere1);
	Engine::Graphics::SubmitGameObject(debugSphere2);
	Engine::Graphics::SubmitGameObject(debugCapsule1);
	Engine::Graphics::SubmitGameObject(debugCapsule2);

	Move(caboose);
	Rotate(sarge, -50.0f, Engine::Math::up);

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

			i_camera->m_transform.right = i_camera->m_transform.right * rotation;
			i_camera->m_transform.up = i_camera->m_transform.up * rotation;
			i_camera->m_transform.forward = i_camera->m_transform.forward * rotation;
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