#include "cGameObject.h"
#include "../Graphics/Assets/cMesh.h"
#include "../Time/Time.h"
#include "../UserInput/UserInput.h"

namespace
{
	const float SPEED = 0.25f;
}

Engine::Shared::cGameObject::cGameObject() {}

Engine::Shared::cGameObject::cGameObject(Engine::Graphics::Assets::cMesh *i_mesh)
{
	if (i_mesh)
	{
		m_mesh = i_mesh;
	}
}

Engine::Shared::cGameObject::~cGameObject() {}
