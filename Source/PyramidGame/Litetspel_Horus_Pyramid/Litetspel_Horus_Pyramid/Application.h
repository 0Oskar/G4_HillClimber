#ifndef _APPLICATION_H_
#define _APPLICATION_H_


#include "ViewLayer.h"

struct GameOptions
{
	int width;
	int height;

	
};

class Application
{
public:
	static Application& getInstance()
	{
		static Application instance;

		return instance;

	}
private:
	Application();
public:
	HWND m_window;
	GameOptions m_gameOptions;
	ViewLayer* viewLayerPtr;

	Application(Application const&) = delete;
	void operator=(Application const&) = delete;

	bool initApplication(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight, int showCmd);
	void createWin32Window(HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window);

	bool loadGameOptions(std::string fileName);


};


#endif // !_APPLICATION_H_
