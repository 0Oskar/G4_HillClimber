#include"pch.h"
#include"Application.h"
#include <Dbt.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern Application* g_App;

Application::Application()
{
	this->m_gameOptions.height = 600;
	this->m_gameOptions.width = 600;
	this->m_gameOptions.fov = 40;
	this->m_gameOptions.mouseSensitivity = 0.1f;
	this->m_deltaTime = 0.f;
	this->m_resetAudio = false;
}

Application::~Application()
{
	if (m_audioEngine) this->m_audioEngine->Suspend();
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

	DirectX::AUDIO_ENGINE_FLAGS audioFlag = DirectX::AudioEngine_EnvironmentalReverb
		| DirectX::AudioEngine_ReverbUseFilters;
#ifdef _DEBUG
	audioFlag = audioFlag | DirectX::AudioEngine_Debug;
#endif
	m_audioEngine = std::make_shared<DirectX::AudioEngine>(audioFlag);
	m_audioEngine->SetReverb(DirectX::Reverb_Plain);

	if (!m_audioEngine->IsAudioDevicePresent())
	{
		OutputDebugString(L"AudioError: No audio device found");
	}

	this->m_viewLayerPtr = std::make_unique<ViewLayer>();
	this->m_viewLayerPtr->initialize(this->m_window, &this->m_gameOptions);

	this->m_gameState.initlialize(this->m_viewLayerPtr->getDevice(), this->m_viewLayerPtr->getContextDevice(), this->m_gameOptions, this->m_audioEngine);

	this->m_viewLayerPtr->setViewMatrix(this->m_gameState.getViewMatrix());
	this->m_viewLayerPtr->setProjectionMatrix(this->m_gameState.getProjectionMatrix());
	this->m_viewLayerPtr->setModelsFromState(this->m_gameState.getModelsPtr());
	this->m_viewLayerPtr->setgameObjectsFromState(this->m_gameState.getGameObjectsPtr());
	this->m_viewLayerPtr->setgameObjectsFromActiveRoom(this->m_gameState.getActiveRoomGameObjectsPtr());
	this->m_viewLayerPtr->setBoundingBoxesFromActiveRoom(this->m_gameState.getActiveRoomBoundingBoxsPtr());
	this->m_viewLayerPtr->setGameTimePtr(this->m_gameState.getGameTimerPtr());
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
	case WM_DEVICECHANGE:
		if (wParam == DBT_DEVICEARRIVAL)
		{
			auto pDev = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
			if (pDev)
			{
				if (pDev->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
				{
					auto pInter = reinterpret_cast<
						const PDEV_BROADCAST_DEVICEINTERFACE>(pDev);
					if (pInter->dbcc_classguid == KSCATEGORY_AUDIO)
					{
						g_App->newAudioDevice();
					}
				}
			}
		}
		return 0;
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

	// Listen for new audio devices
	DEV_BROADCAST_DEVICEINTERFACE filter = {};
	filter.dbcc_size = sizeof(filter);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = KSCATEGORY_AUDIO;

	this->m_hNewAudio = RegisterDeviceNotification(_d3d11Window, &filter,
		DEVICE_NOTIFY_WINDOW_HANDLE);
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
		this->m_gameOptions.mouseSensitivity = std::stof(optionsMap.at("mouseSensitivity"));
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

			this->GameStateChecks();
			// Update Layers
			this->m_gameState.update(this->m_input.getKeyboard(), this->m_input.getMouseEvent(), this->m_input.getMouse(), this->m_deltaTime);
			
			this->m_input.readBuffers();
			this->audioUpdate();


			this->m_viewLayerPtr->update(this->m_deltaTime);
			this->m_viewLayerPtr->render();
		}
	}
}

void Application::GameStateChecks()
{
	if (this->m_gameState.m_activeRoomChanged)
	{
		this->m_viewLayerPtr->setgameObjectsFromActiveRoom(this->m_gameState.getActiveRoomGameObjectsPtr());
		this->m_viewLayerPtr->setBoundingBoxesFromActiveRoom(this->m_gameState.getActiveRoomBoundingBoxsPtr());
		this->m_gameState.m_activeRoomChanged = false;
		this->m_gameState.roomChangeInit();
	}
}

void Application::audioUpdate()
{
	if (m_hNewAudio)
		UnregisterDeviceNotification(m_hNewAudio);

	if (m_resetAudio)
	{
		if (m_audioEngine->Reset())
		{

		}
		this->m_resetAudio = false;
	}
	else if (!m_audioEngine->Update())
	{
		if (m_audioEngine->IsCriticalError())
		{
			OutputDebugString(L"AudioError: Audio device was lost.");
		}
	}
}