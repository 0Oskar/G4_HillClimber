#include"pch.h"
#include"Application.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern Application* g_App;

Application::Application()
{
	this->m_gameOptions.height = 600;
	this->m_gameOptions.width = 600;
	this->m_gameOptions.fov = 40;
	this->m_deltaTime = 0.f;
}

bool Application::initApplication(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int nShowCmd)
{
	if (hWnd != NULL) return true; // Should not be ran twice due to setup of mouse device.
	const wchar_t WINDOWTILE[] = L"Litetspel Horus Pyramid";

	SetCursor(NULL);
	//ShowCursor(FALSE);
	//TODO: Check if we have sufficient resources

	this->loadGameOptions("GameOptions.xml");

	this->createWin32Window(hInstance, WINDOWTILE, hWnd);//Create Window
	OutputDebugStringA("Window Created!\n");

	this->m_window = hWnd;

	this->m_viewLayerPtr = std::make_unique<ViewLayer>();
	this->m_viewLayerPtr->initialize(this->m_window, &this->m_gameOptions);

	this->m_gameState.initlialize(this->m_viewLayerPtr->getDevice(), this->m_viewLayerPtr->getContextDevice(), this->m_gameOptions);

	this->m_viewLayerPtr->setViewMatrix(this->m_gameState.getViewMatrix());
	this->m_viewLayerPtr->setProjectionMatrix(this->m_gameState.getProjectionMatrix());
	this->m_viewLayerPtr->setModelsFromState(this->m_gameState.getModelsPtr());
	this->m_viewLayerPtr->setgameObjectsFromState(this->m_gameState.getGameObjectsPtr());
	this->m_viewLayerPtr->setWvpCBufferFromState(this->m_gameState.getWvpCBuffersPtr());
	this->m_timer.start();

	RAWINPUTDEVICE rawIDevice;
	rawIDevice.usUsagePage = 0x01;
	rawIDevice.usUsage = 0x02;
	rawIDevice.dwFlags = 0;
	rawIDevice.hwndTarget = NULL;

	if (RegisterRawInputDevices(&rawIDevice, 1, sizeof(rawIDevice)) == FALSE)
		return false;

	ShowWindow(this->m_window, nShowCmd);

	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	g_App->m_input.handleMessages(hwnd, uMsg, wParam, lParam);
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Application::createWin32Window(HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window)
{
	const wchar_t CLASS_NAME[] = L"Litetspel_Horus_Pyramid";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.
	_d3d11Window = CreateWindowEx(
		0,                          // Optional window styles.
		CLASS_NAME,                 // Window class
		windowTitle,				// Window text
		WS_OVERLAPPEDWINDOW,        // Window style
		CW_USEDEFAULT,				// Position, X
		CW_USEDEFAULT,				// Position, Y
		this->m_gameOptions.width,	// Width
		this->m_gameOptions.height,	// Height
		NULL,						// Parent window    
		NULL,						// Menu
		hInstance,					// Instance handle
		NULL						// Additional application data
	);
	assert(_d3d11Window);
}

bool Application::loadGameOptions(std::string fileName)
{
	std::map<std::string, std::string> optionsMap;
	pugi::xml_document optionFile;
	pugi::xml_parse_result result = optionFile.load_file(fileName.c_str());
	if (result)
	{
		pugi::xml_node node = optionFile.child("GameOptions");

		for (pugi::xml_node options = node.first_child(); options; options = options.next_sibling())
		{
			optionsMap[options.name()] = options.text().as_string();
		}

		//Here you can load any options you add into GameOptions.xml
		this->m_gameOptions.height = std::stoi(optionsMap.at("Height"));
		this->m_gameOptions.width = std::stoi(optionsMap.at("Width"));
		this->m_gameOptions.fov = std::stof(optionsMap.at("FOV"));
	}

	return result;
}

void Application::applicationLoop()
{
	MSG msg = { };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // Message loop
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // Render/Logic Loop
		{
			// Delta Time
			this->m_deltaTime = (float)m_timer.timeElapsed();
			m_timer.restart();

			// Draw Collision Primitives
			if (this->m_input.getKeyboard()->isKeyPressed('T'))
				this->m_viewLayerPtr->toggleDrawPrimitives(true);

			if (this->m_input.getKeyboard()->isKeyPressed('G'))
				this->m_viewLayerPtr->toggleDrawPrimitives(false);

			// Update Layers
			this->m_gameState.update(this->m_input.getKeyboard(), this->m_input.getMouseEvent(), this->m_input.getMouse(), this->m_deltaTime);
			this->m_input.readBuffers();
			this->m_viewLayerPtr->update(this->m_deltaTime);
			this->m_viewLayerPtr->render();
		}
	}
}