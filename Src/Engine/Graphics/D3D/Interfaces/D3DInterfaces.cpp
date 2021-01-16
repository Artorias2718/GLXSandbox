#include "D3DInterfaces.h"

ID3D11Device* Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dDevice = nullptr;
IDXGISwapChain* Engine::Graphics::Interfaces::D3DInterfaces::s_swapChain = nullptr;
ID3D11DeviceContext* Engine::Graphics::Interfaces::D3DInterfaces::s_direct3dImmediateContext = nullptr;
ID3D11RenderTargetView* Engine::Graphics::Interfaces::D3DInterfaces::s_renderTargetView = nullptr;
ID3D11DepthStencilView* Engine::Graphics::Interfaces::D3DInterfaces::s_depthStencilView = nullptr;
ID3D11SamplerState* Engine::Graphics::Interfaces::D3DInterfaces::s_samplerState = nullptr;
ID3D11InputLayout* Engine::Graphics::Interfaces::D3DInterfaces::s_inputLayout = nullptr;

