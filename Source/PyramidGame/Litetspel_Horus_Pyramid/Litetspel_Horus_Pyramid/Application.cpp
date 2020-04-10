#include"pch.h"
#include"Application.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Application::Application()
{
	this->m_gameOptions.height = 600;
	this->m_gameOptions.width = 600;
	this->m_gameOptions.fov = 60.f;
}

bool Application::initApplication(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int nShowCmd)
{
	const wchar_t WINDOWTILE[] = L"Litetspel Horus Pyramid";

	SetCursor(NULL);
	//TODO: Check if we have sufficient resources

	loadGameOptions("GameOptions.xml");
	if (hWnd == NULL)
		this->createWin32Window(hInstance, WINDOWTILE, hWnd); // Create Window
	else
	{
		//Set window
	}
	this->m_window = hWnd;

	this->m_viewLayerPtr = std::make_unique<ViewLayer>();
	this->m_viewLayerPtr->initialize(this->m_window, &this->m_gameOptions);

	ShowWindow(this->m_window, nShowCmd);
	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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
			this->m_viewLayerPtr->render();
			//Call engine member function here.
		}
	}
}