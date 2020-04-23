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
	float m_deltaTime;

	//DirectX Audio
	HDEVNOTIFY m_hNewAudio;
	bool m_resetAudio;
	void audioUpdate();
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;

public:
	static Application& getInstance()
	{
		static Application instance;
		return instance;
	}
	~Application();

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
	void newAudioDevice() { this->m_resetAudio = true; OutputDebugString(L"New Audio Device!"); }
};

#endif // !_APPLICATION_H_