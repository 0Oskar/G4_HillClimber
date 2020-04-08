#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#ifndef UNICODE
#define UNICODE
#endif

// Standard
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <assert.h>
#include <memory>
#include <wrl/client.h>
#include "pugixml.hpp"
#include <iostream>


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
