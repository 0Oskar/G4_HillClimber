#include"pch.h"
#include"MenuState.h"

MenuState::MenuState()
{
	m_device = nullptr;
	m_dContext = nullptr;
	m_cameraMovementComponentPtr = nullptr;
	m_audioComponent = nullptr;
	m_doneLoadingModelsPtr = nullptr;
}

MenuState::~MenuState()
{
	if (m_audioComponent != nullptr)
	{
		m_audioComponent->stopSound(0);
		delete m_audioComponent;
		m_audioComponent = nullptr;
	}

	if (m_cameraMovementComponentPtr != nullptr)
	{
		delete m_cameraMovementComponentPtr;
	}

}

void MenuState::setupLight()
{
	m_perFrameData.skyLightColor = { 1.f, 1.f, 1.f};
	m_perFrameData.skyLightIntensity = 1.f;
	m_perFrameData.skyLightDirection = { 1.f, 1.f, 1.f };
}

void MenuState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine, volatile bool* doneLoadingModels)
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Pyramid OBB
	DirectX::XMFLOAT3 center(0.f, 62.f, 80.f);
	DirectX::XMFLOAT3 extents(200.f, 250.f, 1.f);
	constexpr float rotationX = XMConvertToRadians(45.f);
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotationX, 0.f, 0.f);
	DirectX::XMFLOAT4 orientation;
	DirectX::XMStoreFloat4(&orientation, quaternion);
	pyramid = DirectX::BoundingOrientedBox(
		center,
		extents,
		orientation
	);

	m_device = device;
	m_dContext = dContext;
	m_cameraMovementComponentPtr = new MovementComponent();
	m_audioComponent = new AudioComponent();
	m_gameOptions = options;

	m_audioComponent->init(audioEngine, m_cameraMovementComponentPtr, 1);
	m_audioComponent->loadSound(m_menuSound);

	m_camera.followMoveComp(m_cameraMovementComponentPtr);
	m_camera.initialize(options.mouseSensitivity, options.fov, (float)options.width / (float)options.height, 0.1f, 10000.f);
	
	m_doneLoadingModelsPtr = doneLoadingModels;
	loadModels();

	m_pyramidRoom.initialize(m_device, m_dContext, &m_models, &m_wvpCBuffers, nullptr, XMVectorSet(0, 0, 0, 1), audioEngine, nullptr, options);
	m_pyramidRoom.init(&pyramid);
	m_perFrameData = m_pyramidRoom.getPerFrameData();

	m_cameraMovementComponentPtr->position = { 0, 100, 0 };

	// UI
	m_resourceHandler = &ResourceHandler::get();
	m_titelImage = m_resourceHandler->getTexture((TEXTURE_PATH + wstring(L"HorusPyramid_TitlePNG.png")).c_str());
	ID3D11Resource* titelImageTexture = 0;
	m_titelImage->GetResource(&titelImageTexture);
	ID3D11Texture2D* titelImageTexture2D = 0;
	titelImageTexture->QueryInterface<ID3D11Texture2D>(&titelImageTexture2D);
	D3D11_TEXTURE2D_DESC titelImageDesc;
	titelImageTexture2D->GetDesc(&titelImageDesc);
	int titelImageX = (m_gameOptions.width / 2) - (titelImageDesc.Width / 2);
	int titelImageY = (m_gameOptions.height / 3) - (titelImageDesc.Height / 2);

	m_titelImagePosition = DirectX::XMFLOAT2((float)titelImageX, (float)titelImageY);

	m_infoImage = m_resourceHandler->getTexture((TEXTURE_PATH + wstring(L"HorusPyramid_StartPNG.png")).c_str());
	ID3D11Resource* infoImageTexture = 0;
	m_infoImage->GetResource(&infoImageTexture);
	ID3D11Texture2D* infoImageTexture2D = 0;
	titelImageTexture->QueryInterface<ID3D11Texture2D>(&infoImageTexture2D);
	D3D11_TEXTURE2D_DESC infoImageDesc;
	infoImageTexture2D->GetDesc(&infoImageDesc);
	int infoImageX = (m_gameOptions.width / 2) - (infoImageDesc.Width*3 / 10);
	int infoImageY = (m_gameOptions.height*8 / 10);

	m_infoImagePosition = DirectX::XMFLOAT2((float)infoImageX, (float)infoImageY);
}


void MenuState::update(float dt)
{
	bool allLoadingThreadsDone = true;
	for (uint32_t i = 0; i < THREAD_COUNT; i++)
	{
		future_status status = m_asyncModelsFuture[i].wait_for(0ms);
		if (m_asyncModelsFuture[i].wait_for(0ms) != future_status::ready)
		{
			allLoadingThreadsDone = false;
		}
	}
	*m_doneLoadingModelsPtr = allLoadingThreadsDone;
	Room* tempRoomPtr = &m_pyramidRoom;
	bool activeRoomChanged = false;
	//if (*m_doneLoadingModelsPtr)
	//{
		m_pyramidRoom.update(dt, &m_camera, tempRoomPtr, activeRoomChanged);
	//}

	float angle;
	m_angle += 10 * dt;

	angle = XMConvertToRadians(m_angle);
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		VS_WVPW_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = m_camera.getViewMatrix() * m_camera.getProjectionMatrix();
		wvpData.wvp = m_gameObjects.at(i)->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = m_gameObjects[i]->getWorldMatrix();

		m_wvpCBuffers[m_gameObjects[i]->getWvpCBufferIndex()].upd(&wvpData);
		if (m_gameObjects[i]->m_removeMe)
		{
			m_gameObjects[i]->setVisibility(false);
			m_gameObjects[i]->setIfCollidable(false);
		}
	}

	XMVECTOR newPos = XMVectorSet(cos(angle) * m_radiusConst, XMVectorGetY(m_moveAroundPosConst), sin(angle) * m_radiusConst, 1);
	m_cameraMovementComponentPtr->position = newPos;

	XMVECTOR cameraToPyramid;
	cameraToPyramid = m_cameraMovementComponentPtr->position - m_lookAtPosConst;
	cameraToPyramid = XMVector3Normalize(cameraToPyramid);
	float targetRotation = (float)atan2((double)(cameraToPyramid.m128_f32[0]), (double)(cameraToPyramid.m128_f32[2])) + XM_PI;

	m_cameraMovementComponentPtr->rotation = XMVectorSet(0.0f, targetRotation, 0.0f, 0.0f);

	m_camera.updateView();
}


states MenuState::handleInput(Keyboard* keyboard, Mouse* mousePtr, float dt)
{
	states changeStateTo = states::NONE;
	if (!keyboard->empty())
	{
		KeyboardEvent keyboardEvent = keyboard->readKey();
		if (keyboardEvent.getEvent() == Event::Pressed)
		{
			if (keyboardEvent.getKey() == VK_ESCAPE)
				changeStateTo = states::POP;
			else if (keyboardEvent.getKey() == VK_RETURN) // ENTER
			{
				if (*m_doneLoadingModelsPtr)
				{
					changeStateTo = states::GAME;
				}
			}
		}
	}

	return changeStateTo;
}
void MenuState::afterChange()
{

}

void MenuState::loadModels()
{
	// Material
	MaterialData mat;
	mat.ambient = { 0.2f, 0.2f, 0.2f};
	mat.globalAmbientContribution = 1.f;

	//-1 Error Model
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInit("cube.obj", mat, L"error_tex.png");

	//0 Desert Ground
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 0.f;
	addNewModelInit("desertGround.obj", mat, L"sandTexture.png");

	//1 Pyramid
	mat.diffuse = { 0.9f, 0.7f, 0.3f, 1.f };
	mat.ambient = { 0.9f * 0.2f, 0.7f * 0.2f, 0.3f * 0.2f};
	mat.globalAmbientContribution = 0.f;
	addNewModelInit("BasePyramidNew.bff", mat, L"pyramidTexture.png");

	//2 HookHead model
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.ambient = { 0.2f, 0.2f, 0.2f};
	mat.globalAmbientContribution = 1.f;
	addNewModelInitAsync("Hook_Bird.bff", mat, L"ColorTexture.png");

	//3 platform model
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 0.3f;
	addNewModelInit("platform.obj", mat, L"platformTextureCracks1.png");

	//4 HookHand
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 1.f;
	addNewModelInitAsync("Gauntlet_Base.bff", mat, L"ColorTexture.png");

	//5 Chain Link
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Hook_ChainLink.bff", mat, L"ColorTexture.png");

	//6 Lever Room aka Kevins room
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("LeverRoom2.obj", mat, L"ColorTexture.png");

	//7 Lever
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Lever.obj", mat, L"ColorTexture.png");

	//8. PuzzleRoom (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("EdvinPuzzleRoom.bff", mat, L"ColorTexture.png");

	//9. Button (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Button.bff", mat, L"Hyroglajf_0.png");

	//10 Portal
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 0.5f;
	addNewModelInit("PortalGate.bff", mat, L"ColorTexture.png");

	//11. Brick_1 (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 1.f;
	addNewModelInitAsync("Brick_1.bff", mat, L"Hyroglajf_0.png");

	//12. Brick_2 (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Brick_2.bff", mat, L"Hyroglajf_1.png");

	//13. Brick_3 (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Brick_3.bff", mat, L"Hyroglajf_2.png");

	//14. Brick_4 (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Brick_4.bff", mat, L"Hyroglajf_3.png");

	//15. Brick_5 (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Brick_5.bff", mat, L"Hyroglajf_4.png");

	//16. Leaver Base (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("LeverBase.bff", mat, L"ColorTexture.png");

	//17. Pedistal (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Pedistal.bff", mat, L"ColorTexture.png");

	//18. Cover (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Cover.bff", mat, L"ColorTexture.png");

	//19. checkpoint
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 0.5f;
	addNewModelInit("checkpointPlatform.bff", mat, L"ColorTexture.png");

	//20. Leaver Handle (Edvin)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 1.f;
	addNewModelInitAsync("LeverHandle.bff", mat, L"ColorTexture.png");

	//21. PuzzleRoom(Tristan)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("TristansNewPuzzleRoom.bff", mat, L"ColorTexture.png");

	//22. Pyramid Portal
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 0.5f;
	addNewModelInit("PuzzleRoomGateExtended.bff", mat, L"ColorTexture.png");

	//23. Scorpion
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 1.f;
	addNewModelInitAsync("scorpionDone.obj", mat, L"ColorTexture.png");

	//24. dartWallObj
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("dartTrapWall.obj", mat, L"ColorTexture.png");

	//25. dartWalldarts
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("dartTrapWallDart.obj", mat, L"ColorTexture.png");

	//26. expandingBridge
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("expandingBridge.obj", mat, L"ColorTexture.png");

	//27. Lever Grip (Tristan)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("TristansLeverGrip.bff", mat, L"ColorTexture.png");
	 
	//28. finalRoom
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("endRoom.obj", mat, L"ColorTexture.png");

	//29. swingingAxe
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("swingingAxe.obj", mat, L"ColorTexture.png");
	
	//30. Left Wing
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Gauntlet_LeftWing.bff", mat, L"ColorTexture.png");

	//31. Right Wing
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Gauntlet_RightWing.bff", mat, L"ColorTexture.png");

	//32. Hook Highlight Gem
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Gauntlet_GreenGem.bff", mat, L"ColorTexture.png");

	//33. Diamond
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Diamond.bff", mat, L"ColorTexture.png");

	//33. Bell 2
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInitAsync("Bell2.bff", mat, L"ColorTexture.png");

	//34. PalmTree
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInit("PalmTree.obj", mat, L"ColorTexture.png");

	//35. Hawk (36 temporarily)
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInit("Falcon2.obj", mat, L"ColorTexture.png");

	// Room (Viktor)
	{
		//36
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vRoom_New2.bff", mat, L"ColorTexture.png");

		//37
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vGate_New.bff", mat, L"ColorTexture.png");

		//38
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vLever_New.bff", mat, L"ColorTexture.png");

		//39
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vGem1_New.bff", mat, L"ColorTexture.png");

		//40
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vGem2_New.bff", mat, L"ColorTexture.png");

		//41
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vGem3_New.bff", mat, L"ColorTexture.png");

		//42
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vGem4_New.bff", mat, L"ColorTexture.png");

		//43
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vGem5_New.bff", mat, L"ColorTexture.png");

		//44
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vPedistal_New.bff", mat, L"ColorTexture.png");

		//45
		mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
		addNewModelInitAsync("vRamp_New.bff", mat, L"ColorTexture.png");
	}
	
	//46. Cactus
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	addNewModelInit("cactus2.obj", mat, L"ColorTexture.png");

	//47. Pillars
	mat.diffuse = { 1.f, 1.f, 1.f, 1.f };
	mat.globalAmbientContribution = 0.7f;
	addNewModelInit("Entrence.obj", mat, L"ColorTexture.png");

	//48. Clouds
	mat.diffuse = { 1.f, 1.f, 1.f, 0.6f };
	mat.globalAmbientContribution = 0.f;
	addNewModelInit("Clouds.obj", mat, L"ColorTexture.png");
	
	//49. Clouds
	mat.diffuse = { 0.5f, 0.5f, 0.5f, 1.f };
	mat.globalAmbientContribution = 1.f;
	addNewModelInit("ddh.obj", mat, L"DefaultWhite.jpg");

	// Start async loading threads
	OutputDebugStringA("Threaded model loading begin\n");
	uint32_t startIndex = 0;
	uint32_t perThreadCount = (uint32_t)m_asyncModelNames.size() / THREAD_COUNT;
	for (uint32_t i = 0; i < THREAD_COUNT; i++)
	{
		uint32_t endIndex = startIndex + perThreadCount;
		m_asyncModelsFuture[i] = async(launch::async, MenuState::loadModelsThreadSafe, i, &m_models, &m_asyncModelNames, startIndex, endIndex);
		startIndex = endIndex + 1;
	}
}


void MenuState::onEntry() 
{
	m_audioComponent->playSound(m_menuSound, 0, true, 1.0f, 0.5f);
}

void MenuState::onLeave()
{
	m_audioComponent->stopSound(0);
}

void MenuState::onPop()
{
	iGameState::onPop();

	if(m_cameraMovementComponentPtr != nullptr)
		delete m_cameraMovementComponentPtr;

	if (m_audioComponent != nullptr)
		delete m_audioComponent;
}

void MenuState::addNewModelInitAsync(const char* modelFilePath, const MaterialData material, const wchar_t* texturePath)
{
	m_models[modelFilePath].initForAsyncLoad(m_device, m_dContext, modelFilePath, material, texturePath);
	m_asyncModelNames.push_back(modelFilePath);
}

void MenuState::addNewModelInit(const char* modelFilePath, const MaterialData material, const wchar_t* texturePath)
{
	m_models[modelFilePath].loadModel(m_device, m_dContext, modelFilePath, material, texturePath);
}

void MenuState::loadModelsThreadSafe(uint32_t threadIndex, std::unordered_map<std::string, Model>* modelsListPtr, std::vector<const char*>* modelNameList, uint32_t from, uint32_t to)
{
	std::string message = "Thread " + to_string(threadIndex) + " Start!\n";
	OutputDebugStringA(message.c_str());

	std::unordered_map<std::string, Model>& modelMap = *modelsListPtr;
	for (uint32_t i = from; i <= to; i++)
	{
		modelMap[modelNameList->at(i)].loadModelAsync();
	}
	message = "Thread " + to_string(threadIndex) + " Done!\n";
	OutputDebugStringA(message.c_str());
}

std::unordered_map<std::string, Model>* MenuState::getModelsPtr()
{
	return &m_models;
}

std::vector<GameObject*>* MenuState::getGameObjectsPtr()
{
	return m_pyramidRoom.getGameObjectsPtr();
}

std::vector<BoundingBox>* MenuState::getActiveRoomBoundingBoxesPtr()
{
	return nullptr;
}

std::vector<BoundingOrientedBox>* MenuState::getActiveRoomOrientedBoundingBoxesPtr()
{
	return nullptr;
}

PS_PER_FRAME_BUFFER* MenuState::getPerFrameData()
{
	return &m_perFrameData;
}

std::vector<ConstBuffer<VS_WVPW_CONSTANT_BUFFER>>* MenuState::getWvpCBuffersPtr() 
{
	return &m_wvpCBuffers;
}

Camera* MenuState::getCamera()
{
	return &m_camera;
}

void MenuState::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{
	spriteBatchPtr->Draw(m_titelImage, m_titelImagePosition);

	if (m_doneLoadingModelsPtr && *m_doneLoadingModelsPtr)
		spriteBatchPtr->Draw(m_infoImage, m_infoImagePosition);
}