#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4.h"
#include "InputSystem.h"
#include "EngineTime.h"


AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	constant cc;
	cc.m_time = EngineTime::getDeltaTime();

	/*m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	m_delta_scale += m_delta_time / 0.55f;*/

	Matrix4 temp;


	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f,1.5f, 0), m_delta_pos));

	//cc.m_world *= temp;

	/*cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;*/

	cc.m_world.setIdentity();

	
	
#if OLD_CAMERA
	Matrix4 world_cam;
	world_cam.setIdentity();

	
	temp.setIdentity();
	temp.setRotationX(rotX);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(rotY);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.1f);
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.1f);
	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	/*cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left)/300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top)/300.0f,
		-4.0f,
		4.0f
	);*/

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
#endif
	cc.m_view = m_Camera->getView();
	cc.m_proj = m_Camera->getProj();

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	// INPUT SYSTEM
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	// GRAPHICS ENGINE
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	m_swap_chain->init(this->m_hwnd, m_windowWidth, m_windowHeight);

	
	// SCENE CAMERA
	m_Camera = new Camera(&m_windowWidth, &m_windowHeight);


	vertex vertex_list[] =
	{
		//X - Y - Z
		{Vector3D(-0.5f,-0.5f,-0.5f), Vector3D(-0.5f,-0.5f,-0.5f),   Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f), Vector3D(-0.5f,0.5f,-0.5f),   Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f), Vector3D(0.5f,0.5f,-0.5f),  Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f), Vector3D(0.5f,-0.5f,-0.5f),    Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f), Vector3D(0.5f,-0.5f,0.5f),   Vector3D(0,1,0), Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f), Vector3D(0.5f,0.5f,0.5f),   Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f), Vector3D(-0.5f,0.5f,0.5f),  Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f), Vector3D(-0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);


	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};


	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);




	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));




	m_testCube = new Cube();
	m_testCube->initialize();
	m_testCube->move(Vector3D(-2, 0, 0));
	m_testCube->rotate(Vector3D(150, 0, 0));
}

void AppWindow::updateQuadPosition()
{
	m_delta_time = EngineTime::getDeltaTime();

	constant cc;
	cc.m_time = EngineTime::getDeltaTime();//::GetTickCount();


	/*m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;*/


	Matrix4 temp;

	//m_delta_scale += m_delta_time / 0.55f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f,1.5f, 0), m_delta_pos));

	//cc.m_world *= temp;
	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(scaleCube, scaleCube, scaleCube));

	temp.setIdentity();
	temp.setRotationZ(0);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(rotY);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(rotX);
	cc.m_world *= temp;

	/*cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);*/

	cc.m_view = m_Camera->getView();
	cc.m_proj = m_Camera->getProj();

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();


	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(m_windowWidth, m_windowHeight);



	m_Camera->update();
	update();
	updateQuadPosition();



	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);


	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	m_testCube->updateMatrix(m_Camera->getView(), m_Camera->getProj());
	m_testCube->draw();

	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	delete m_testCube;

	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		rotX += 3.14f*m_delta_time;
	}
	else if (key == 'S')
	{
		rotX -= 3.14f*m_delta_time;
	}
	else if (key == 'A')
	{
		rotY += 3.14f*m_delta_time;
	}
	else if (key == 'D')
	{
		rotY -= 3.14f*m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	InputSystem::get()->setCursorPosition(Point((m_windowWidth / 2), (m_windowHeight / 2)));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	scaleCube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	scaleCube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	scaleCube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	scaleCube = 1.0f;
}