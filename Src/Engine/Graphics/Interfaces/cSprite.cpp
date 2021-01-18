#include "cSprite.h"

Engine::Graphics::Interfaces::cSprite::cSprite(
	const Structures::sRectangle& i_screenPosition,
	const Structures::sRectangle& i_texturePosition,
	const Structures::sColor8& i_color,
	float i_scale) :
	m_screenPosition(i_screenPosition),
	m_texturePosition(i_texturePosition),
	m_color(i_color),
	m_scale(i_scale)
{
	Initialize();
}


Engine::Graphics::Interfaces::cSprite::~cSprite()
{
	CleanUp();
}


