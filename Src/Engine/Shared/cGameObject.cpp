#include "cGameObject.h"
#include "../Graphics/Assets/cMaterial.h"
#include "../Graphics/Assets/cMesh.h"
#include "../Graphics/Interfaces/cSprite.h"
#include "../Graphics/Assets/cTexture/cTexture.h"

#include "../Time/Time.h"
#include "../UserInput/UserInput.h"
#include "../UserSettings/UserSettings.h"
#if defined WINDOWS_API 
	#include "../Windows/Includes.h"
#endif

namespace
{
	const float SPEED = 0.25f;
}

Engine::Shared::cGameObject::cGameObject() {}

Engine::Shared::cGameObject::cGameObject(Graphics::Assets::cMesh *i_mesh, Graphics::Assets::cMaterial *i_material)
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

Engine::Shared::cGameObject::cGameObject(Engine::Graphics::Assets::cMaterial* i_material,
	const glm::vec2& i_position, const glm::vec2& i_scale)
{
	if (m_sprite == nullptr)
	{
		m_sprite = new Graphics::Interfaces::cSprite();
	}

	if (i_material)
	{
		m_material = i_material;
	}

	float textureWidth = m_material->m_texture->m_width;
	float textureHeight = m_material->m_texture->m_height;

	float screenWidth = static_cast<float>(UserSettings::GetResolutionWidth());
	float screenHeight = static_cast<float>(UserSettings::GetResolutionHeight());

	Graphics::Structures::sRectangleF screenPosition = m_sprite->m_screenPosition;

	glm::vec2 absScale(fabsf(i_scale.x), fabsf(i_scale.y));

	m_sprite->m_screenPosition.x = absScale.x * (i_position.x - textureWidth) / screenWidth;
	m_sprite->m_screenPosition.y = absScale.x * (i_position.x + textureWidth) / screenWidth;
	m_sprite->m_screenPosition.z = absScale.y * (i_position.y + textureHeight) / screenHeight;
	m_sprite->m_screenPosition.w = absScale.y * (i_position.y - textureHeight) / screenHeight;

	if (i_scale.x < 0)
	{
		m_sprite->m_texturePosition.x = 1.0f - m_sprite->m_texturePosition.x;
		m_sprite->m_texturePosition.y = 1.0f - m_sprite->m_texturePosition.x;
	}
	if (i_scale.y < 0)
	{
		m_sprite->m_texturePosition.z = 1.0f - m_sprite->m_texturePosition.z;
		m_sprite->m_texturePosition.w = 1.0f - m_sprite->m_texturePosition.w;
	}
}


Engine::Shared::cGameObject::~cGameObject() {}
