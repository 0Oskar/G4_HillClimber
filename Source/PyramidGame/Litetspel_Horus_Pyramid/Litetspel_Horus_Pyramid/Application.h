#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "GameState.h"
#include "ViewLayer.h"
#include "Input.h"
#include"Timer.h"

class Application
{
private:
	Application();
	Timer m_timer;
public:
	static Application& getInstance()
	{
		static Application instance;
		return instance;
	}

	Input m_input;
	HWND m_window;
	GameState m_gameState;
	GameOptions m_gameOptions;
	std::unique_ptr< ViewLayer > m_viewLayerPtr;

	Application(Application const&) = delete;
	void operator=(Application const&) = delete;

	bool initApplication(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int showCmd);
	void createWin32Window(HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window);
	bool loadGameOptions(std::string fileName);

	void applicationLoop();
};

#endif // !_APPLICATION_H_