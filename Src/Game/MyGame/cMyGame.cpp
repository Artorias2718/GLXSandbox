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
#include "../../Engine/Graphics/Structures/sVertex.h"
#include "../../Engine/Graphics/Structures/sIndexSet.h"
#include "../../Engine/Shared/sCollisionTriangle.h"
#include "GameObjects/sPlayer.h"
#include "GameObjects/sPlayerController.h"

// Interface
//==========

namespace
{
	const float SPEED = 8.0f;

	Engine::Graphics::Assets::cMaterial* boxesMat;
	Engine::Graphics::Assets::cMaterial* cementMat;
	Engine::Graphics::Assets::cMaterial* collisionMat;
	Engine::Graphics::Assets::cMaterial* groundMat;
	Engine::Graphics::Assets::cMaterial* metalMat;
	Engine::Graphics::Assets::cMaterial* railingMat;
	Engine::Graphics::Assets::cMaterial* debugMat;

	Engine::Shared::cGameObject* boxes;
	Engine::Shared::cGameObject* cement;
	Engine::Shared::cGameObject* collision;
	Engine::Shared::cGameObject* ground;
	Engine::Shared::cGameObject* metal;
	Engine::Shared::cGameObject* railing;

	Game::MyGame::sPlayer* player;
	Game::MyGame::sPlayerController* playerController;

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
	collisionMat = new Engine::Graphics::Assets::cMaterial("collision");
	groundMat = new Engine::Graphics::Assets::cMaterial("ground");
	metalMat = new Engine::Graphics::Assets::cMaterial("metal");
	railingMat = new Engine::Graphics::Assets::cMaterial("railing");
	debugMat = new Engine::Graphics::Assets::cMaterial("debug");

	boxes = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("boxes"), boxesMat);
	cement = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("cement"), cementMat);
	collision = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("collision"), collisionMat);
	ground = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("ground"), groundMat);
	metal = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("metal"), metalMat);
	railing = new Engine::Shared::cGameObject(new Engine::Graphics::Assets::cMesh("railing"), railingMat);

	player = new sPlayer("player");
	playerController = new sPlayerController(player);

	for (uint32_t triIdx = 0; triIdx < collision->m_mesh->m_indexSetCount; ++triIdx)
	{
		Engine::Math::cVector a, b, c;
		if (collision->m_mesh->m_isShort)
		{
			a = Engine::Math::cVector(
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].a].position.x,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].a].position.y,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].a].position.z
			);

			b = Engine::Math::cVector(
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].b].position.x,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].b].position.y,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].b].position.z
			);

			c = Engine::Math::cVector(
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].c].position.x,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].c].position.y,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet16[triIdx].c].position.z
			);
		}
		else
		{
			a = Engine::Math::cVector(
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].a].position.x,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].a].position.y,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].a].position.z
			);

			b = Engine::Math::cVector(
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].b].position.x,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].b].position.y,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].b].position.z
			);

			c = Engine::Math::cVector(
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].c].position.x,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].c].position.y,
				collision->m_mesh->m_vertices[collision->m_mesh->m_indexSet32[triIdx].c].position.z
			);
		}


#if defined D3D_API 
		Engine::Shared::sCollisionTriangle* collisionTriangle = new Engine::Shared::sCollisionTriangle(a, c, b);
#elif defined OGL_API 
		Engine::Shared::sCollisionTriangle* collisionTriangle = new Engine::Shared::sCollisionTriangle(a, b, c);
#endif
		collisionTriangle->ComputeNormal();
		player->m_rigidbody.m_collisionData.push_back(collisionTriangle);
	}

	camera = new Engine::Shared::cCamera("flycamera");

	return true;
}

bool Game::MyGame::cMyGame::Update()
{
	Engine::Graphics::SubmitGameObject(boxes);
	Engine::Graphics::SubmitGameObject(cement);
#if defined _DEBUG
	Engine::Graphics::SubmitGameObject(collision);
#endif
	Engine::Graphics::SubmitGameObject(ground);
	Engine::Graphics::SubmitGameObject(metal);
	Engine::Graphics::SubmitGameObject(railing);
	
	//Move(suzanne);
	//Rotate(suzanne, -50.0f, Engine::Math::cVector::up);

	if (Engine::UserInput::IsKeyPressed('F'))
	{
		camera->m_active = !camera->m_active;
		player->m_active = !player->m_active;
	}

	if (camera->m_active)
	{
		Engine::Graphics::SubmitGameObject(camera);
		Move(camera);
		
		if (Engine::Shared::MouseParams::mouseMoved)
		{
			Engine::Shared::MouseParams::mouseMoved = false;
			Rotate(camera, 1.0f, Engine::Math::cVector(0.0f, 1.0f, 0.0f));
		}
		player->Render();
	}
	else if(player->m_active)
	{
		camera->m_transform.position = player->m_camera->m_transform.position;
		camera->m_transform.orientation = player->m_camera->m_transform.orientation;
		Engine::Graphics::SubmitGameObject(player->m_camera);
		playerController->Move();
	}
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

