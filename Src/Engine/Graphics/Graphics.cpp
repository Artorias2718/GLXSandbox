// Header Files
//=============

#include "Graphics.h"

#include "../Asserts/Asserts.h"
#include "../Logging/Logging.h"

#include "Interfaces/cConstantBuffer.h"
#include "Structures/sFrame.h"
#include "Structures/sDrawCall.h"

#include "../Shared/cGameObject.h"

std::vector<Engine::Shared::cGameObject*> Engine::Graphics::meshes;
Engine::Graphics::Structures::sFrame Engine::Graphics::frameData;
Engine::Graphics::Structures::sDrawCall Engine::Graphics::drawCallData;

Engine::Graphics::Interfaces::cConstantBuffer* Engine::Graphics::s_frameBuffer = nullptr;
Engine::Graphics::Interfaces::cConstantBuffer* Engine::Graphics::s_drawCallBuffer = nullptr;


bool Engine::Graphics::SubmitGameObject(Engine::Shared::cGameObject* i_object)
{
	if (i_object->m_mesh)
	{
		meshes.push_back(i_object);
		return true;
	}
	else
	{
		Engine::Logging::OutputError("Object mesh is uninitialized!");
		return false;
	}
}
