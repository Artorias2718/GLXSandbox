// Header Files
//=============

#include "cRenderState.h"

// Interface
//==========

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Interfaces::cRenderState::Initialize(const uint8_t i_renderStateBits)
{
	m_bits = i_renderStateBits;
	return InitializeFromBits();
}

Engine::Graphics::Interfaces::cRenderState::cRenderState()
	:
#if defined( D3D_API )
	m_blendState(NULL), m_depthStencilState(NULL), m_rasterizerState(NULL),
#endif
	// A reasonable default could be set here in the constructor,
	// but choosing a recognizable impossible value can help in debugging
	// (if a RenderState ever has these bits it shows that it wasn't initialized)
	m_bits(0xff)
{

}

Engine::Graphics::Interfaces::cRenderState::~cRenderState()
{
	//CleanUp();
}


