#include"pch.h"
#include"Application.h"

Application* g_App;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	HRESULT hr = CoInitialize(NULL);
	
	bool initOK = false;
	g_App = &Application::getInstance();
	initOK = g_App->initApplication(hInstance, lpCmdLine, g_App->m_window, nShowCmd);

	if (initOK)
	{
		OutputDebugStringA("Window Created!\n");
		g_App->applicationLoop();
	}

	return 0;
};

