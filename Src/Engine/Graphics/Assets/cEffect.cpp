#include "cEffect.h"
#include "../../Asserts/Asserts.h"

Engine::Graphics::Assets::cEffect::cEffect(const std::string& i_effect) :
	m_effect(i_effect),
#if defined D3D_API 
	m_vertexShader(nullptr), m_fragmentShader(nullptr)
#elif defined OGL_API 
	m_programId(0)
#endif
{
	LoadShaders();
}

Engine::Graphics::Assets::cEffect::~cEffect()
{
}