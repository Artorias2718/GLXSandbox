// Header Files
//=============

#include "Graphics.h"

#include "../Asserts/Asserts.h"
#include "../Logging/Logging.h"

std::vector<Engine::Graphics::Assets::cMesh*> Engine::Graphics::meshObjects;

bool Engine::Graphics::SubmitGameObject(Engine::Graphics::Assets::cMesh* i_object)
{
	if (i_object)
	{
		meshObjects.push_back(i_object);
		return true;
	}
	else
	{
		Engine::Logging::OutputError("Mesh is uninitialized!");
		return false;
	}
}

