#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

#include <string>

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

	bool present(bool vsync);

private:
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_rtv = nullptr;
	ID3D11DepthStencilView* m_dsv = nullptr;
	RenderSystem* m_system = nullptr;

	std::string HResultToString(HRESULT hr);

private:
	friend class DeviceContext;
};
