#pragma once
#include "Primitive.h"

class Quad : public Primitive
{
public:
	Quad();
	Quad(Vector3D color);
	Quad(Quad& copy, RECT rc);
	~Quad();


	void createVertexBuffer(void* shader_byte_code, UINT size_byte_shader);

};