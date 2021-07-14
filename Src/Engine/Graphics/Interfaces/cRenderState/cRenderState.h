/*
Render state is the fixed-function state
that can be configured on the GPU
*/

#ifndef ENGINE_GRAPHICS_INTERFACES_CRENDERSTATE_H
#define ENGINE_GRAPHICS_INTERFACES_CRENDERSTATE_H

// Header Files
//=============

#include "../../Configuration.h"

#include <cstdint>

// Forward Declarations
//=====================

#if defined ( D3D_API )
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
#endif


namespace Engine
{
	namespace Graphics
	{
		namespace Interfaces
		{
			class cRenderState
			{
				// Interface
				//==========

			public:

				// Render
				//-------

				void Bind() const;

				// Access
				//-------

				bool IsAlphaTransparencyEnabled() const;
				bool IsDepthBufferingEnabled() const;
				bool IsBackfaceCullingEnabled() const;
				bool IsWireframeRenderingEnabled() const;

				uint8_t GetRenderStateBits() const;

				// Initialization / Clean Up
				//--------------------------

				// The input parameter is a concatenation of RenderState::eRenderState bits
				// which define which render states should be enabled
				bool Initialize(const uint8_t i_renderStateBits);
				bool CleanUp();

				cRenderState();
				~cRenderState();

				// Data
				//=====

			private:

#if defined( D3D_API )
				ID3D11BlendState* m_blendState;
				ID3D11DepthStencilState* m_depthStencilState;
				ID3D11RasterizerState* m_rasterizerState;
#endif
				uint8_t m_bits;

				// Implementation
				//===============

				// Initialization / Clean Up
				//--------------------------

				bool InitializeFromBits();
			};
		}
	}
}

//#include "cRenderState.inl"

#endif // !ENGINE_GRAPHICS_INTERFACES_CRENDERSTATE_H
