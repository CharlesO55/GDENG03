#pragma once
#include "VertexData.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class VertexShader;
class PixelShader;


class Primitive
{
protected:
	vertex* m_vertexList;
	VertexBuffer* m_vb = nullptr;
	IndexBuffer* m_ib = nullptr;

	constant m_cc;
	ConstantBuffer* m_cb = nullptr;

	Matrix4 m_transform;

	Primitive* m_child = nullptr;
	bool m_keepTransform = false;


	float updateSpeed = 1;
	float updateDir = 1;
	float totalTime = 0;

public:
	Primitive();
	Primitive(vertex* vertices);
	void createVertexBuffer(void* shader_byte_code, UINT size_byte_shader, UINT vertex_count);
	void createIndexBuffer();
	void createConstantBuffer(RECT rc);
	~Primitive();

	void addChild(Primitive* child, bool keepTransform);
	void transform(Vector3D translate, Vector3D scale, Vector3D rotate);

	Primitive* getChild();

public:
	void update(double deltaTime);

	void startDraw(VertexShader* vs, PixelShader* ps, constant* global_cc = nullptr);
	void drawChildren(VertexShader* vs, PixelShader* ps);


protected:
	virtual void draw(VertexShader* vs, PixelShader* ps, constant* global_cc);
};