#include "cGameObject.h"
#include "../Graphics/Assets/cMesh.h"
#include "../Time/Time.h"
#include "../UserInput/UserInput.h"
#include "../Math/glm/gtc/quaternion.hpp"
#include "MouseParams.h"

namespace
{
	const float SPEED = 0.25f;
}

Engine::Shared::cGameObject::cGameObject() {}

Engine::Shared::cGameObject::cGameObject(Engine::Graphics::Assets::cMesh *i_mesh, Graphics::Assets::cMaterial *i_material)
{
	if (i_mesh)
	{
		m_mesh = i_mesh;
	}

	if(i_material)
	{
		m_material = i_material;
	}
}

Engine::Shared::cGameObject::~cGameObject() {}

bool Engine::Shared::cGameObject::Move(float offset)
{
	if (Engine::UserInput::IsKeyPressed('A'))
	{
		m_transform.position -= offset * m_transform.right;
	}
	if (Engine::UserInput::IsKeyPressed('D'))
	{
		m_transform.position += offset * m_transform.right;
	}
	
	if (Engine::UserInput::IsKeyPressed('W'))
	{
		m_transform.position += offset * m_transform.up;
	}
	if (Engine::UserInput::IsKeyPressed('S'))
	{
		m_transform.position -= offset * m_transform.up;
	}
	
	if (Engine::UserInput::IsKeyPressed('Q'))
	{
		m_transform.position -= offset * m_transform.forward;
	}
	if (Engine::UserInput::IsKeyPressed('E'))
	{
		m_transform.position += offset * m_transform.forward;
	}

	return true;
}

bool Engine::Shared::cGameObject::Rotate(float offset, glm::vec3& axis)
{
	glm::quat rotation = glm::angleAxis(offset, axis);
	m_transform.orientation = glm::normalize(m_transform.orientation * rotation);

	return true;
}