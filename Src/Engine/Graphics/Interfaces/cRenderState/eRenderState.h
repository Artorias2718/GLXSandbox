#ifndef ENGINE_GRAPHICS_INTERFACES_ERENDERSTATE_H
#define ENGINE_GRAPHICS_INTERFACES_ERENDERSTATE_H

#include <cstdint>

namespace Engine
{
	namespace Graphics
	{
		namespace Interfaces
		{
			namespace RenderStates
			{
				// The values below are used as bit masks
				enum eRenderState
				{
					// Set to enable alpha transparency
					AlphaTransparency = 1 << 0,
					// Set to enable both depth testing and writing to the depth buffer
					DepthBuffering = 1 << 1,
					// Set to draw only front-facing triangles
					// (unset to disable culling)
					BackfaceCulling = 1 << 2,
					// Set to enable wireframe rendering
					WireframeRendering = 1 << 3
				};

				// Alpha Transparency
				bool IsAlphaTransparencyEnabled(const uint8_t i_renderStateBits);
				void EnableAlphaTransparency(uint8_t& io_renderStateBits);
				void DisableAlphaTransparency(uint8_t& io_renderStateBits);
				// Depth Buffering
				bool IsDepthBufferingEnabled(const uint8_t i_renderStateBits);
				void EnableDepthBuffering(uint8_t& io_renderStateBits);
				void DisableDepthBuffering(uint8_t& io_renderStateBits);
				// Cull back-facing triangles
				bool IsBackfaceCullingEnabled(const uint8_t i_renderStateBits);
				void EnableBackfaceCulling(uint8_t& io_renderStateBits);
				void DisableBackfaceCulling(uint8_t& io_renderStateBits);
				// Render wireframe triangles
				bool IsWireframeRenderingEnabled(const uint8_t i_renderStateBits);
				void EnableWireframeRendering(uint8_t& io_renderStateBits);
				void DisableWireframeRendering(uint8_t& io_renderStateBits);
			}
		}
	}
}

#endif // !ENGINE_GRAPHICS_INTERFACES_ERENDERSTATE_H
