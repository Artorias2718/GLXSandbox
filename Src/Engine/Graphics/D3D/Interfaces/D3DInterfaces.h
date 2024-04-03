#ifndef ENGINE_GRAPHICS_INTERFACES_D3DINTERFACES
#define ENGINE_GRAPHICS_INTERFACES_D3DINTERFACES

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11SamplerState;
struct ID3D11InputLayout;

namespace Engine
{
	namespace Graphics
	{
		namespace Interfaces
		{
			namespace D3DInterfaces
			{
				extern ID3D11Device* s_direct3dDevice;
				extern IDXGISwapChain* s_swapChain;
				extern ID3D11DeviceContext* s_direct3dImmediateContext;
				extern ID3D11RenderTargetView* s_renderTargetView;
				extern ID3D11DepthStencilView* s_depthStencilView;
				extern ID3D11SamplerState* s_samplerState;
				extern ID3D11InputLayout* s_inputLayout;
			}
		}
	}
}

#endif // !ENGINE_GRAPHICS_INTERFACES_D3DINTERFACES