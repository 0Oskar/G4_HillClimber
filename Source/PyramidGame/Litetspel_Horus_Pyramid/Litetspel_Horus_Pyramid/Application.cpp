#include"pch.h"
#include"Application.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern Application* g_App;

Application::Application()
{
	m_gameOptions.height = 600;
	m_gameOptions.width = 600;
	m_gameOptions.fov = 40;
	m_gameOptions.mouseSensitivity = 0.1f;
	m_gameOptions.name = "undefined";
	m_deltaTime = 0.f;
	m_resetAudio = false;
	m_shouldQuit = false;
	m_doneLoadingModels = false;
}

Application::~Application()
{
		
}

void Application::stateChange()
{
	iGameState* gameState = m_gameStateStack.top();
	m_viewLayerPtr->setViewMatrix(gameState->getViewMatrix());
	m_viewLayerPtr->setProjectionMatrix(gameState->getProjectionMatrix());
	m_viewLayerPtr->setModelsFromState(gameState->getModelsPtr());
	m_viewLayerPtr->setgameObjectsFromState(gameState->getGameObjectsPtr());
	m_viewLayerPtr->setBoundingBoxesFromActiveRoom(gameState->getActiveRoomBoundingBoxesPtr());
	m_viewLayerPtr->setOrientedBoundingBoxesFromActiveRoom(gameState->getActiveRoomOrientedBoundingBoxesPtr());
	m_viewLayerPtr->setWvpCBufferFromState(gameState->getWvpCBuffersPtr());
	m_viewLayerPtr->setConstantBuffersFromGameState(gameState->getConstantBufferData());
	gameState->onEntry();
	//gameState->updateCustomViewLayerVariables(m_viewLayerPtr.get());
}

bool Application::initApplication(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int nShowCmd)
{
	if (hWnd != NULL) return true; // Should not be ran twice due to setup of mouse device.
	const wchar_t WINDOWTILE[] = L"Litetspel Horus Pyramid";

	SetCursor(NULL);
	//ShowCursor(FALSE);
	//TODO: Check if we have sufficient resources

	loadGameOptions("GameOptions.xml");

	createWin32Window(hInstance, WINDOWTILE, hWnd);//Create Window
	OutputDebugStringA("Window Created!\n");

	m_window = hWnd;

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

	m_viewLayerPtr = std::make_unique<ViewLayer>();
	m_viewLayerPtr->initialize(m_window, &m_gameOptions);
	
	m_gameStateStack.push(new MenuState());
	m_gameStateStack.top()->initlialize(m_viewLayerPtr->getDevice(), m_viewLayerPtr->getContextDevice(), m_gameOptions, m_audioEngine, &m_doneLoadingModels);
	stateChange();

	
	m_timer.start();

	RAWINPUTDEVICE rawIDevice;
	rawIDevice.usUsagePage = 0x01;
	rawIDevice.usUsage = 0x02;
	rawIDevice.dwFlags = 0;
	rawIDevice.hwndTarget = NULL;
	if (RegisterRawInputDevices(&rawIDevice, 1, sizeof(rawIDevice)) == FALSE)
		return false;

	ShowWindow(m_window, nShowCmd);

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
		m_gameOptions.width,	// Width
		m_gameOptions.height,	// Height
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

	m_hNewAudio = RegisterDeviceNotification(_d3d11Window, &filter,
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
		m_gameOptions.height = std::stoi(optionsMap.at("Height"));
		m_gameOptions.width = std::stoi(optionsMap.at("Width"));
		m_gameOptions.fov = std::stof(optionsMap.at("FOV"));
		m_gameOptions.mouseSensitivity = std::stof(optionsMap.at("mouseSensitivity"));
		m_gameOptions.name = optionsMap.at("name");
	}

	return result;
}

void Application::applicationLoop()
{
	MSG msg = { };
	states fetchedState;
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
			m_deltaTime = (float)m_timer.timeElapsed();
			m_timer.restart();

			// Draw Collision Primitives
			if (m_input.getKeyboard()->isKeyPressed('T'))
				m_viewLayerPtr->toggleDrawPrimitives(true);

			if (m_input.getKeyboard()->isKeyPressed('G'))
				m_viewLayerPtr->toggleDrawPrimitives(false);

			GameStateChecks();

			// Update Layers
			fetchedState = m_gameStateStack.top()->handleInput(m_input.getKeyboard(), m_input.getMouse(), m_deltaTime);
			if (fetchedState == states::RELOAD_SHADERS)
				m_viewLayerPtr->reloadShaders();
			else if (fetchedState != states::NONE)
				pushNewState(fetchedState);

			if (!m_shouldQuit)
			{
				m_gameStateStack.top()->update(m_deltaTime);
				if (m_doneLoadingModels)
				{
					audioUpdate();

				}
				m_viewLayerPtr->update(m_deltaTime, m_gameStateStack.top()->getCameraPos());
				m_viewLayerPtr->render(m_gameStateStack.top());
			}
		}
	}
}

void Application::pushNewState(states state)
{
	int gameTime = 0;
	//If we enter this function we always push or pop something
	bool newState = false;
	std::unordered_map<std::string, Model>* mdlPointer = m_gameStateStack.top()->getModelsPtr();
	m_gameStateStack.top()->onLeave();
	switch (state)
	{
	case states::NONE:
		break;
	case states::MENU:
		break;
	case states::GAME:
		m_gameStateStack.push(new GameState());
		newState = true;
		break;
	case states::PAUSE:
		break;
	case states::POP:
		delete m_gameStateStack.top();
		m_gameStateStack.pop();
		break;
	case states::WON:
		gameTime = (int)dynamic_cast<GameState*>(m_gameStateStack.top())->getGameTimerPtr()->timeElapsed();
		m_gameStateStack.push(new WinState);
		dynamic_cast<WinState*>(m_gameStateStack.top())->setGameTime(gameTime);
		newState = true;
		break;
	default:
		break;
	}

	if (newState)
	{
		m_gameStateStack.top()->setModelPointer(mdlPointer);
		m_gameStateStack.top()->initlialize(m_viewLayerPtr->getDevice(), m_viewLayerPtr->getContextDevice(), m_gameOptions, m_audioEngine, &m_doneLoadingModels);
	}
	if (m_gameStateStack.size() <= 0)
	{
		PostQuitMessage(-1);
		m_shouldQuit = true;
	}
	else
	{
		stateChange();
	}
}

void Application::GameStateChecks()
{
	iGameState* gameState = m_gameStateStack.top();
	if (gameState->m_majorChange)
	{
		stateChange();
		gameState->m_majorChange = false;
		gameState->afterChange();
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
		m_resetAudio = false;
	}
	else if (!m_audioEngine->Update())
	{
		if (m_audioEngine->IsCriticalError())
		{
			OutputDebugString(L"AudioError: Audio device was lost.");
		}
	}
}