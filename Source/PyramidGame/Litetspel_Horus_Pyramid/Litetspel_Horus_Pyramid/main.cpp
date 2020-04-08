#include"pch.h"
#include"Application.h"

Application* g_App;


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	bool initOK = false;
	g_App = &Application::getInstance();
	initOK = g_App->initApplication(hInstance, lpCmdLine, g_App->m_window, g_App->m_gameOptions.width, g_App->m_gameOptions.height, nShowCmd);

	if (initOK)
	{

	OutputDebugStringA("Window Created!\n");
	ShowWindow(g_App->m_window, nShowCmd);

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
				g_App->viewLayerPtr->render();
				//Call engine member function here.
			}
		}
	}

	return 0;
};

