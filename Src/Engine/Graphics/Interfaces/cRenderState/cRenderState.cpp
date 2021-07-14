// Header Files
//=============

#include "cRenderState.h"
#include "eRenderState.h"

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

// Interface
//==========

// Alpha Transparency
bool Engine::Graphics::Interfaces::RenderStates::IsAlphaTransparencyEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & AlphaTransparency) != 0;
}
void Engine::Graphics::Interfaces::RenderStates::EnableAlphaTransparency(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= AlphaTransparency;
}
void Engine::Graphics::Interfaces::RenderStates::DisableAlphaTransparency(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= ~AlphaTransparency;
}

// Depth Buffering
bool Engine::Graphics::Interfaces::RenderStates::IsDepthBufferingEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & DepthBuffering) != 0;
}
void Engine::Graphics::Interfaces::RenderStates::EnableDepthBuffering(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= DepthBuffering;
}
void Engine::Graphics::Interfaces::RenderStates::DisableDepthBuffering(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= ~DepthBuffering;
}

// Draw Both Triangle Sides
bool Engine::Graphics::Interfaces::RenderStates::IsBackfaceCullingEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & BackfaceCulling) != 0;
}
void Engine::Graphics::Interfaces::RenderStates::EnableBackfaceCulling(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= BackfaceCulling;
}
void Engine::Graphics::Interfaces::RenderStates::DisableBackfaceCulling(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= BackfaceCulling;
}

bool Engine::Graphics::Interfaces::RenderStates::IsWireframeRenderingEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & WireframeRendering) != 0;
}

void Engine::Graphics::Interfaces::RenderStates::EnableWireframeRendering(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= WireframeRendering;
}

void Engine::Graphics::Interfaces::RenderStates::DisableWireframeRendering(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= WireframeRendering;
}

// Access
//-------

bool Engine::Graphics::Interfaces::cRenderState::IsAlphaTransparencyEnabled() const
{
	return Interfaces::RenderStates::IsAlphaTransparencyEnabled(m_bits);
}

bool Engine::Graphics::Interfaces::cRenderState::IsDepthBufferingEnabled() const
{
	return Interfaces::RenderStates::IsDepthBufferingEnabled(m_bits);
}

bool Engine::Graphics::Interfaces::cRenderState::IsBackfaceCullingEnabled() const
{
	return Interfaces::RenderStates::IsBackfaceCullingEnabled(m_bits);
}

bool Engine::Graphics::Interfaces::cRenderState::IsWireframeRenderingEnabled() const
{
	return Interfaces::RenderStates::IsWireframeRenderingEnabled(m_bits);
}

uint8_t Engine::Graphics::Interfaces::cRenderState::GetRenderStateBits() const
{
	return m_bits;
}
