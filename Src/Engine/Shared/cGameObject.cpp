#include "cGameObject.h"
#include "../Graphics/Assets/cMesh.h"
#include "../Time/Time.h"
#include "../UserInput/UserInput.h"

namespace
{
	const float SPEED = 0.25f;
}

Engine::Shared::cGameObject::cGameObject() {}

Engine::Shared::cGameObject::cGameObject(Graphics::Assets::cMesh *i_mesh, Graphics::Assets::cEffect *i_effect)
{
	if (i_mesh)
	{
		m_mesh = i_mesh;
	}
	if (i_effect)
	{
		m_effect = i_effect;
	}
}

Engine::Shared::cGameObject::~cGameObject() {}
