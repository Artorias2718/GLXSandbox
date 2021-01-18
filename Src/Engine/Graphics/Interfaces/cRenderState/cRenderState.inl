#ifndef ENGINE_GRAPHICS_CRENDERSTATE_INL
#define ENGINE_GRAPHICS_CRENDERSTATE_INL

// Header Files
//=============

#include "eRenderState.h"
#include "cRenderState.h"

// Interface
//==========

// Alpha Transparency
inline bool Engine::Graphics::Interfaces::RenderStates::IsAlphaTransparencyEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & AlphaTransparency) != 0;
}
inline void Engine::Graphics::Interfaces::RenderStates::EnableAlphaTransparency(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= AlphaTransparency;
}
inline void Engine::Graphics::Interfaces::RenderStates::DisableAlphaTransparency(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= ~AlphaTransparency;
}

// Depth Buffering
inline bool Engine::Graphics::Interfaces::RenderStates::IsDepthBufferingEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & DepthBuffering) != 0;
}
inline void Engine::Graphics::Interfaces::RenderStates::EnableDepthBuffering(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= DepthBuffering;
}
inline void Engine::Graphics::Interfaces::RenderStates::DisableDepthBuffering(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= ~DepthBuffering;
}

// Draw Both Triangle Sides
inline bool Engine::Graphics::Interfaces::RenderStates::IsBackfaceCullingEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & BackfaceCulling) != 0;
}
inline void Engine::Graphics::Interfaces::RenderStates::EnableBackfaceCulling(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= BackfaceCulling;
}
inline void Engine::Graphics::Interfaces::RenderStates::DisableBackfaceCulling(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= BackfaceCulling;
}

inline bool Engine::Graphics::Interfaces::RenderStates::IsWireframeRenderingEnabled(const uint8_t i_renderStateBits)
{
	return (i_renderStateBits & WireframeRendering) != 0;
}

inline void Engine::Graphics::Interfaces::RenderStates::EnableWireframeRendering(uint8_t& io_renderStateBits)
{
	io_renderStateBits |= WireframeRendering;
}

inline void Engine::Graphics::Interfaces::RenderStates::DisableWireframeRendering(uint8_t& io_renderStateBits)
{
	io_renderStateBits &= WireframeRendering;
}

// Access
//-------

inline bool Engine::Graphics::Interfaces::cRenderState::IsAlphaTransparencyEnabled() const
{
	return Interfaces::RenderStates::IsAlphaTransparencyEnabled(m_bits);
}

inline bool Engine::Graphics::Interfaces::cRenderState::IsDepthBufferingEnabled() const
{
	return Interfaces::RenderStates::IsDepthBufferingEnabled(m_bits);
}

inline bool Engine::Graphics::Interfaces::cRenderState::IsBackfaceCullingEnabled() const
{
	return Interfaces::RenderStates::IsBackfaceCullingEnabled(m_bits);
}

inline bool Engine::Graphics::Interfaces::cRenderState::IsWireframeRenderingEnabled() const
{
	return Interfaces::RenderStates::IsWireframeRenderingEnabled(m_bits);
}

inline uint8_t Engine::Graphics::Interfaces::cRenderState::GetRenderStateBits() const
{
	return m_bits;
}

#endif	// !ENGINE_GRAPHICS_CRENDERSTATE_INL

