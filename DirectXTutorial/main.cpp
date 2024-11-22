#include "AppWindow.h"
#include "InputSystem.h"
#include "EngineTime.h"
#include "Debugger.h"

#include "UIManager.h"
#include "EngineState.h"

int main()
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
		EngineTime::create();
		EngineState::Init();
	}
	catch (...) { return -1; }
	{
		try
		{
			AppWindow app;
			while (app.Run()) {
			}
		}
		catch (...) {
			UIManager::Release();
			InputSystem::release();
			GraphicsEngine::release();
			EngineTime::release();
			return -1;
		}
	}


	UIManager::Release();
	InputSystem::release();
	GraphicsEngine::release();
	EngineTime::release();
	EngineState::Release();

	return 0;
}