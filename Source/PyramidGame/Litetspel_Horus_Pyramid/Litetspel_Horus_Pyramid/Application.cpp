#include"pch.h"
#include"Application.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


Application::Application()
{
	this->m_gameOptions.height = 600;
	this->m_gameOptions.width = 600;
	
}

bool Application::initApplication(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight, int nShowCmd)
{
	const wchar_t WINDOWTILE[] = L"Litetspel Horus Pyramid";

	SetCursor(NULL);
	//TODO: Check if we have sufficient resources

	if (hWnd == NULL)
	{
		this->createWin32Window(hInstance, WINDOWTILE, hWnd);//Create Window

	}
	else
	{
		//Set window
	}
	this->m_window = hWnd;

	this->viewLayerPtr = new ViewLayer(screenWidth, screenHeight);
	this->viewLayerPtr->initialize(this->m_window);
	

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
		this->m_gameOptions.width,				// Width
		this->m_gameOptions.height,				// Height
		NULL,						// Parent window    
		NULL,						// Menu
		hInstance,					// Instance handle
		NULL						// Additional application data
	);
	assert(_d3d11Window);
}
