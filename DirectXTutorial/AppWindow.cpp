#include "AppWindow.h"

#include "VertexData.h"
#include "EngineTime.h"

AppWindow::AppWindow()
{ 
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right-rc.left, rc.bottom-rc.top);


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	
	
	createObjects(shader_byte_code, size_shader, rc);

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();


	// CONSTANT BUFFER
	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}




void AppWindow::createObjects(void* shader_byte_code, size_t size_shader, RECT rc) {
	// ALTER THE VERTEX struct and VERTEXBUFFER AS WELL IF CONTENTS CHANGE
	/*vertex triangle_list[] = {
		{Vector3D(-1.f, -1.f, 0.0f),	Vector3D(0,1,0),	Vector3D(0,1,0)},
		{Vector3D(-0.5f, 0.f, 0.0f),	Vector3D(0,1,0),	Vector3D(0,1,0)},
		{Vector3D(0.f, -1.f, 0.0f),		Vector3D(0,1,0),	Vector3D(0,1,0)}
	};*/
	


	vertex quad_list[] = {
		//	POS0	,	POS1	,	COLOR0	,	COLOR1
		{ Vector3D(-0.1f, -0.1f, 0.0f)	, Vector3D(-0.5f, -0.5f, 0.0f)	,	Vector3D(0,1,0), Vector3D(0, 0, 0) }, // BL
		{ Vector3D(0.3f,0.5f,0.0f)		, Vector3D(-0.9f,0.f,0.0f)		,   Vector3D(1,1,0), Vector3D(1,1,0) },		// TL
		{ Vector3D(0.8f,-.5f,0.0f)		, Vector3D(0.5f,-0.5f,0.0f)		,   Vector3D(1,0,0),  Vector3D(0,0,1) },	// BR
		{ Vector3D(0.99f,0.4f,0.0f)		, Vector3D(0.2f,0.1f,0.0f)		,   Vector3D(0,0,1), Vector3D(1,1,1) }		// TR
	};

	// Create a new Primitive since the current Quad removed animations
	Primitive* animateRect = new Primitive(quad_list);
	animateRect->createVertexBuffer(shader_byte_code, size_shader, 4);
	animateRect->createConstantBuffer(rc);
	animateRect->transform(Vector3D(0.5f,.5f, 0), Vector3D(0.5f,.5f,.5f), Vector3D(0,0,0));
	m_objects.push_back(animateRect);

	
	vertex folding_rect[] = {
		//	POS0	,	POS1	,	COLOR0	,	COLOR1
		{ Vector3D(-0.1f, -0.1f, 0.0f)	, Vector3D(-0.5f, -0.5f, 0.0f)	,	Vector3D(1,1,0), Vector3D(0, 1, 1) }, // BL
		{ Vector3D(0.3f,0.5f,0.0f)		, Vector3D(-0.9f,0.f,0.0f)		,   Vector3D(1,1,1), Vector3D(1,0,0) },		// TL
		{ Vector3D(0.8f,-.5f,0.0f)		, Vector3D(0.5f,-0.5f,0.0f)		,   Vector3D(1,0,0),  Vector3D(0,0,1) },	// BR
		{ Vector3D(0.99f,0.4f,0.0f)		, Vector3D(-0.5f, -0.5f,0.0f)		,   Vector3D(0,0,0), Vector3D(1,0,1) }		// TR
	};

	Primitive* foldingRect = new Primitive(folding_rect);
	foldingRect->createVertexBuffer(shader_byte_code, size_shader, 4);
	foldingRect->createConstantBuffer(rc);
	foldingRect->transform(Vector3D(-0.5f, -.5f, 0), Vector3D(0.5f, .5f, .5f), Vector3D(0, 0, 0));
	m_objects.push_back(foldingRect);

	/*
	* [1] Creates a parent and child.
	* [2] Attaches child to parent.
	* [3] Function returns the parent.
	* [Optional] We can transform the parent and the child will follow.
	* [Optional] We can transform the child and the parent will not be affected.
	* [4] Add ONLY the parent to the objects list.
	*
	Quad* tilted = createParentAndChild(shader_byte_code, size_shader, rc);
	tilted->transform(Vector3D(-0.5, 0.5, 0), Vector3D(0.5, 0.5, 1), Vector3D(0, 0, 45));
	m_objects.push_back(tilted);

	Quad* normal = createParentAndChild(shader_byte_code, size_shader, rc);
	normal->transform(Vector3D(0.5, 0.5, 0), Vector3D(0.5, 0.5, 1), Vector3D(0, 0, 0));
	m_objects.push_back(normal);

	Quad* strecthed = createParentAndChild(shader_byte_code, size_shader, rc);
	strecthed->transform(Vector3D(0, -0.5, 0), Vector3D(2, 0.5, 1), Vector3D(0, 0, 0));
	m_objects.push_back(strecthed);

	// OVERRIDE CHILD TRANSFORMS SAMPLE
	strecthed->getChild()->transform(Vector3D(0, 0, 0), Vector3D(0.1, 0.1, 1), Vector3D(0, 0, 0));*/
}


Quad* AppWindow::createParentAndChild(void* shader_byte_code, size_t size_shader, RECT rc)
{
	Quad* parent = new Quad();
	parent->createVertexBuffer(shader_byte_code, size_shader);
	parent->createConstantBuffer(rc);
	parent->transform(Vector3D(0, -0.5f, 0), Vector3D(2, 0.5, 1), Vector3D(0, 0, 0)); 

	Quad* child = new Quad(Vector3D(1, 0, 0));	//color red child
	child->createVertexBuffer(shader_byte_code, size_shader);
	child->createConstantBuffer(rc);
	child->transform(Vector3D(0, 0, 0), Vector3D(1, 1, 1), Vector3D(0, 0, 45)); //tilt child transform to be distinguishable
	parent->addChild(child, true);

	return parent;
}


void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(
		m_swap_chain, 0.5,0.5f,0.5f,1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right-rc.left, rc.bottom-rc.top);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//updateQuadPosition();

	double deltaTime = EngineTime::getDeltaTime();
	for (int i = 0; i < m_objects.size(); i++) {
		this->m_objects[i]->update(deltaTime);
		//Ignore updating children for now...
		this->m_objects[i]->draw(m_vs, m_ps);
		this->m_objects[i]->drawChildren(m_vs, m_ps);
	}

	m_swap_chain->present(false);

	// UPDATE TIME
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

Quad* AppWindow::createNewSquare(Quad *original) {
	RECT rc = this->getClientWindowRect();
	Quad* newQuad = new Quad(*original, rc);

	return newQuad;
}



void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_objects.clear();
	m_objects.shrink_to_fit();

	m_swap_chain->release();
	m_vs->release();
	m_ps->release();

	GraphicsEngine::get()->release();
}

/* [DEPRECATED]
*	Our constant buffers have been moved inside Primitive classes for passing transform updates.
*	Use this buffer only for passing global cc data.
*/
void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount64();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	Matrix4 temp;

	m_delta_scale += m_delta_time / 0.15f;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));

	cc.m_world *= temp;


	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}