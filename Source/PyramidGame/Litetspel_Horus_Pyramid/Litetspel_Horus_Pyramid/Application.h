#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "ViewLayer.h"
#include "Input.h"
#include"Timer.h"
#include <Dbt.h>
#include "GameState.h"
#include "MenuState.h"
#include"WinState.h"

class Application
{
private:
	Application();
	Timer m_timer;
	float m_deltaTime;

	//DirectX Audio
	HDEVNOTIFY m_hNewAudio;
	bool m_resetAudio;
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;

	std::stack<iGameState*> m_gameStateStack;
	bool m_shouldQuit;
	volatile bool m_doneLoadingModels;

	void audioUpdate();

public:
	static Application& getInstance()
	{
		static Application instance;
		return instance;
	}
	~Application();

	void stateChange();
	Application(Application const&) = delete;
	void operator=(Application const&) = delete;

	bool initApplication(const HINSTANCE hInstance, const LPWSTR lpCmdLine, const HWND hWnd, const int showCmd);
	void createWin32Window(const HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window);
	bool loadGameOptions(const std::string fileName);

	void applicationLoop();
	void pushNewState(const states state);
	void GameStateChecks();
	void newAudioDevice() { m_resetAudio = true; OutputDebugString(L"New Audio Device!"); }

	Input m_input;
	HWND m_window;
	GameOptions m_gameOptions;
	std::unique_ptr< ViewLayer > m_viewLayerPtr;
};

#endif // !_APPLICATION_H_