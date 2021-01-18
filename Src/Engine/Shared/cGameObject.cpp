#include "cGameObject.h"
#include "../Graphics/Assets/cMesh.h"
#include "../Time/Time.h"
#include "../UserInput/UserInput.h"

namespace
{
	const float SPEED = 0.25f;
}

Engine::Shared::cGameObject::cGameObject() {}

Engine::Shared::cGameObject::cGameObject(Graphics::Assets::cMesh *i_mesh, Graphics::Assets::cMaterial  *i_material)
{
	if (i_mesh)
	{
		m_mesh = i_mesh;
	}

	if (i_material)
	{
		m_material = i_material;
	}
}

Engine::Shared::cGameObject::~cGameObject() {}
