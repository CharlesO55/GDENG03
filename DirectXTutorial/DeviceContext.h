#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	~DeviceContext();

	bool release();

	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void setViewportSize(UINT width, UINT height);


private:
	ID3D11DeviceContext* m_device_context;
};
