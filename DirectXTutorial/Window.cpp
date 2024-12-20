#include "Window.h"

#include "EngineTime.h"
#include "Debugger.h"

#include "UIManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;


	switch (msg)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_SETFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}




Window::Window()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"WindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc))
		Debugger::Error("[Window] Create");

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"WindowClass", L"GDENG03_Raycast",
		WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 840, 600,
		NULL, NULL, NULL, NULL);

	if (!m_hwnd)
		Debugger::Error("[Window] Create");


	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	m_is_run = true;
	Debugger::Success("[Window] Created");
}

Window::~Window()
{
}

bool Window::broadcast()
{
    MSG msg;

	if (!this->m_is_init)
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_is_init = true;
	}
	EngineTime::tick();

    // Update only occurs when the time matches the expecred ms per frame
    if (EngineTime::getDeltaTime() >= timePerFrame) {
        this->onUpdate();

        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // Updates the lastFrame to now        
        EngineTime::LogFrameStart();
        // Refreshes the deltaTime
        EngineTime::ResetDeltaTime();
    }
    Sleep(1);

    return true;
}

bool Window::Run()
{
	if (m_is_run) {
		broadcast();
	}
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::onCreate()
{
	CalcWindowRect();

	UIManager::initialize(this->m_hwnd);
}

void Window::onDestroy()
{
	m_is_run = false;
	Debugger::Warning("[Window] Destroyed");
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
	// VALUE USED BY CAMERA FOR SCREEN SIZE
	CalcWindowRect();
}



void Window::onUpdate()
{
}

void Window::CalcWindowRect()
{
    m_windowWidth = (this->getClientWindowRect().right - this->getClientWindowRect().left);
    m_windowHeight = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
}