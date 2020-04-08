#ifndef UNICODE
#define UNICODE
#endif

// Layer Includes
#include "ViewLayer.h"

// Window
HWND window;
HRESULT hr;

// Abstaction layers
ViewLayer view;

// Window Functions
void createWin32Window(HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Main
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	// Window Setup
	const wchar_t WINDOWTILE[] = L"Litetspel Horus Pyramid";
	createWin32Window(hInstance, WINDOWTILE, window);

	// Initialize Engine
	view.initialize(window);

	// Show Window
	ShowWindow(window, nShowCmd);
	OutputDebugStringA("Window Created!\n");

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
			view.render();
		}
	}

	return 0;
}

void createWin32Window(HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window)
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
		view.getWindowWidth(),		// Width
		view.getWindowHeight(),		// Height
		NULL,						// Parent window    
		NULL,						// Menu
		hInstance,					// Instance handle
		NULL						// Additional application data
		);
	assert(_d3d11Window);
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