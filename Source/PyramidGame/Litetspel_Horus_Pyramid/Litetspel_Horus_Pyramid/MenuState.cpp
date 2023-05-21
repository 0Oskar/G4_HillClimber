#include"pch.h"
#include"MenuState.h"

MenuState::MenuState()
{
	this->m_device = nullptr;
	this->m_dContext = nullptr;
	this->m_cameraMovementComponentPtr = nullptr;
	this->m_audioComponent = nullptr;
}

MenuState::~MenuState()
{
	if (this->m_audioComponent != nullptr)
	{
		this->m_audioComponent->stopSound(0);
		delete this->m_audioComponent;
		this->m_audioComponent = nullptr;
	}

	if (this->m_cameraMovementComponentPtr != nullptr)
	{
		delete this->m_cameraMovementComponentPtr;
	}

}

void MenuState::setupLight()
{
	this->m_constantbufferData.dirBuffer.lightColor = { 1, 1, 1, 1 };
	this->m_constantbufferData.dirBuffer.lightDirection = { 1, 1, 1, 1 };

}

void MenuState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{


	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	

	//Pyramid OBB
	DirectX::XMFLOAT3 center(0.f, 62.f, 80.f);
	DirectX::XMFLOAT3 extents(200.f, 250.f, 1.f);
	float rotationX = XMConvertToRadians(45.f);
	LPCWSTR test = std::to_wstring(rotationX).c_str();
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotationX, 0.f, 0.f);
	DirectX::XMFLOAT4 orientation;
	DirectX::XMStoreFloat4(&orientation, quaternion);
	this->pyramid = DirectX::BoundingOrientedBox(
		center,
		extents,
		orientation
	);


	this->m_device = device;
	this->m_dContext = dContext;
	this->m_cameraMovementComponentPtr = new MovementComponent();
	this->m_audioComponent = new AudioComponent();
	this->m_gameOptions = options;

	this->m_audioComponent->init(audioEngine, m_cameraMovementComponentPtr, 1);
	this->m_audioComponent->loadSound(menuSound);

	this->m_camera.followMoveComp(m_cameraMovementComponentPtr);
	this->m_camera.initialize(options.mouseSensitivity, options.fov, (float)options.width / (float)options.height, 0.1f, 1000.f);
	
	this->loadModels();

	m_pyramidRoom.initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, nullptr, XMVectorSet(0, 0, 0, 1), audioEngine, nullptr, options);
	m_pyramidRoom.init(&pyramid);
	this->m_constantbufferData.dirBuffer = this->m_pyramidRoom.getDirectionalLight();
	this->m_constantbufferData.fogBuffer = this->m_pyramidRoom.getFogData();
	this->m_constantbufferData.lightBuffer = this->m_pyramidRoom.getLightData();

	this->m_cameraMovementComponentPtr->position = { 0, 100, 0 };

	// UI
	this->m_resourceHandler = &ResourceHandler::get();	
	this->m_titelImage = this->m_resourceHandler->getTexture(L"Textures/HorusPyramid_TitlePNG.png");
	ID3D11Resource* titelImageTexture = 0;
	this->m_titelImage->GetResource(&titelImageTexture);
	ID3D11Texture2D* titelImageTexture2D = 0;
	titelImageTexture->QueryInterface<ID3D11Texture2D>(&titelImageTexture2D);
	D3D11_TEXTURE2D_DESC titelImageDesc;
	titelImageTexture2D->GetDesc(&titelImageDesc);
	int titelImageX = (this->m_gameOptions.width / 2) - (titelImageDesc.Width / 2);
	int titelImageY = (this->m_gameOptions.height / 3) - (titelImageDesc.Height / 2);

	this->m_titelImagePosition = DirectX::XMFLOAT2((float)titelImageX, (float)titelImageY);

	this->m_infoImage = this->m_resourceHandler->getTexture(L"Textures/HorusPyramid_StartPNG.png");
	ID3D11Resource* infoImageTexture = 0;
	this->m_infoImage->GetResource(&infoImageTexture);
	ID3D11Texture2D* infoImageTexture2D = 0;
	titelImageTexture->QueryInterface<ID3D11Texture2D>(&infoImageTexture2D);
	D3D11_TEXTURE2D_DESC infoImageDesc;
	infoImageTexture2D->GetDesc(&infoImageDesc);
	int infoImageX = (this->m_gameOptions.width / 2) - (infoImageDesc.Width*3 / 10);
	int infoImageY = (this->m_gameOptions.height*8 / 10);

	this->m_infoImagePosition = DirectX::XMFLOAT2((float)infoImageX, (float)infoImageY);
}


void MenuState::update(float dt)
{
	bool boolean = false;
	float angle;
	Room* tempRoomPtr = &this->m_pyramidRoom;
	m_angle += 10 * dt;

	angle = XMConvertToRadians(m_angle);
	this->m_pyramidRoom.update(dt, &this->m_camera, tempRoomPtr, boolean);
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = this->m_camera.getViewMatrix() * this->m_camera.getProjectionMatrix();
		wvpData.wvp = this->m_gameObjects.at(i)->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = this->m_gameObjects[i]->getWorldMatrix();

		this->m_wvpCBuffers[this->m_gameObjects[i]->getWvpCBufferIndex()].upd(&wvpData);
		if (this->m_gameObjects[i]->m_removeMe)
		{
			this->m_gameObjects[i]->setVisibility(false);
			this->m_gameObjects[i]->setIfCollidable(false);
		}
	}


	

	XMVECTOR newPos = XMVectorSet(cos(angle) * m_radiusConst, XMVectorGetY(m_moveAroundPosConst), sin(angle) * m_radiusConst, 1);
	this->m_cameraMovementComponentPtr->position = newPos;

	XMVECTOR cameraToPyramid;
	cameraToPyramid = this->m_cameraMovementComponentPtr->position - m_lookAtPosConst;
	cameraToPyramid = XMVector3Normalize(cameraToPyramid);
	float targetRotation = (float)atan2((double)(cameraToPyramid.m128_f32[0]), (double)(cameraToPyramid.m128_f32[2])) + XM_PI;

	
	this->m_cameraMovementComponentPtr->rotation = XMVectorSet(0.0f, targetRotation, 0.0f, 0.0f);
	

	this->m_camera.updateView();
}


states MenuState::handleInput(Keyboard* keyboard, Mouse* mousePtr, float dt)
{
	states changeStateTo = states::NONE;
	if (!keyboard->empty())
	{
		KeyboardEvent keyboardEvent = keyboard->readKey();
		if (keyboardEvent.getEvent() == Event::Pressed)
		{
			if (keyboardEvent.getKey() == (char)27)
				changeStateTo = states::POP;
			else if(keyboardEvent.getKey() == (char)13)
				changeStateTo = states::GAME;
		}
	}

	return changeStateTo;
}
void MenuState::afterChange()
{

}

void MenuState::loadModels()
{
	m_models.resize(50);
	uint32_t modelIndex = 0;

	// Material
	MaterialData mat;
	mat.ambient = { 0.2f, 0.2f, 0.2f, 1.f };

	//0 - Desert Ground
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "desertGround.obj", mat, L"Textures/sandTexture.png");

	//1 - Pyramid
	mat.diffuse = DirectX::XMFLOAT4(0.9f, 0.7f, 0.3f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "BasePyramidNew.bff", mat, L"Textures/pyramidTexture.png");

	//2- HookHead model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Hook_Bird.bff", mat, L"Textures/ColorTexture.png");

	//3- platform model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "platform.obj", mat, L"Textures/platformTextureCracks1.png");

	//4- HookHand
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Gauntlet_Base.bff", mat, L"Textures/ColorTexture.png");

	//5- Chain Link
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Hook_ChainLink.bff", mat, L"Textures/ColorTexture.png");

	//6- Lever Room aka Kevins room
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "LeverRoom2.obj", mat, L"Textures/ColorTexture.png");

	//7 - Lever
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "Lever.obj", mat, L"Textures/ColorTexture.png");

	//8. PuzzleRoom (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "EdvinPuzzleRoom.bff", mat, L"Textures/ColorTexture.png");

	//9. Button (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Button.bff", mat, L"Textures/Hyroglajf_0.png");

	//10 - Portal
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "PortalGate.bff", mat, L"Textures/ColorTexture.png");

	//11. Brick_1 (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Brick_1.bff", mat, L"Textures/Hyroglajf_0.png");

	//12. Brick_2 (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Brick_2.bff", mat, L"Textures/Hyroglajf_1.png");

	//13. Brick_3 (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Brick_3.bff", mat, L"Textures/Hyroglajf_2.png");

	//14. Brick_4 (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Brick_4.bff", mat, L"Textures/Hyroglajf_3.png");

	//15. Brick_5 (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Brick_5.bff", mat, L"Textures/Hyroglajf_4.png");

	//16. Leaver Base (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "LeverBase.bff", mat, L"Textures/ColorTexture.png");

	//17. Pedistal (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Pedistal.bff", mat, L"Textures/ColorTexture.png");

	//18. Cover (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Cover.bff", mat, L"Textures/ColorTexture.png");

	//19. checkpoint
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "checkpointPlatform.bff", mat, L"Textures/ColorTexture.png");

	//20. Leaver Handle (Edvin)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "LeverHandle.bff", mat, L"Textures/ColorTexture.png");

	//21. PuzzleRoom(Tristan)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "TristansNewPuzzleRoom.bff", mat, L"Textures/ColorTexture.png");

	//22. Pyramid Portal
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "PuzzleRoomGateExtended.bff", mat, L"Textures/ColorTexture.png");

	//23. Scorpion
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "scorpionDone.obj", mat, L"Textures/ColorTexture.png");

	//24. dartWallObj
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "dartTrapWall.obj", mat, L"Textures/ColorTexture.png");

	//25. dartWalldarts
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "dartTrapWallDart.obj", mat, L"Textures/ColorTexture.png");

	//26. expandingBridge
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "expandingBridge.obj", mat, L"Textures/ColorTexture.png");

	//27. Lever Grip (Tristan)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "TristansLeverGrip.bff", mat, L"Textures/ColorTexture.png");

	//28. finalRoom
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "endRoom.obj", mat, L"Textures/ColorTexture.png");

	//29. swingingAxe
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "swingingAxe.obj", mat, L"Textures/ColorTexture.png");

	//30. Left Wing
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Gauntlet_LeftWing.bff", mat, L"Textures/ColorTexture.png");

	//31. Right Wing
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Gauntlet_RightWing.bff", mat, L"Textures/ColorTexture.png");

	//32. Hook Highlight Gem
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Gauntlet_GreenGem.bff", mat, L"Textures/ColorTexture.png");

	//33. Diamond
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Diamond.bff", mat, L"Textures/ColorTexture.png");

	//33. Bell 2
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "Bell2.bff", mat, L"Textures/ColorTexture.png");

	//34. PalmTree
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "PalmTree.obj", mat, L"Textures/ColorTexture.png");

	//35. Hawk (36 temporarily)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "Falcon2.obj", mat, L"Textures/ColorTexture.png");

	// Room (Viktor)
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vRoom_New2.bff", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vGate_New.bff", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vLever_New.bff", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vGem1_New.bff", mat, L"Textures/ColorTexture.png");
	
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vGem2_New.bff", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vGem3_New.bff", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vGem4_New.bff", mat, L"Textures/ColorTexture.png");
	
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vGem5_New.bff", mat, L"Textures/ColorTexture.png");
	
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vPedistal_New.bff", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[modelIndex++].initializeModelBff(m_device, m_dContext, "vRamp_New.bff", mat, L"Textures/ColorTexture.png");

	// Not Viktor
	
	//47. Cactus
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "cactus2.obj", mat, L"Textures/ColorTexture.png");

	//48. Pillar
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "Entrence.obj", mat, L"Textures/ColorTexture.png");

	//49. Clouds
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[modelIndex++].loadVertexFromOBJ(m_device, m_dContext, "Clouds.obj", mat, L"Textures/ColorTexture.png");
}


void MenuState::onEntry() 
{
	this->m_audioComponent->playSound(menuSound, 0, true, 1.0f, 0.5f);
}

void MenuState::onLeave()
{
	this->m_audioComponent->stopSound(0);
}

void MenuState::onPop()
{
	iGameState::onPop();

	if(this->m_cameraMovementComponentPtr != nullptr)
		delete m_cameraMovementComponentPtr;

	if (m_audioComponent != nullptr)
		delete m_audioComponent;
}

std::vector<Model>* MenuState::getModelsPtr()
{
	return &this->m_models;
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

constantBufferData* MenuState::getConstantBufferData()
{
	return &this->m_constantbufferData;
}

std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* MenuState::getWvpCBuffersPtr() 
{
	return &this->m_wvpCBuffers;
}

DirectX::XMMATRIX* MenuState::getViewMatrix() const
{
	return this->m_camera.getViewMatrixPtr();
}

DirectX::XMMATRIX* MenuState::getProjectionMatrix() const
{
	return this->m_camera.getProjectionMatrixPtr();
}


XMFLOAT3 MenuState::getCameraPos() const
{
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, this->m_cameraMovementComponentPtr->position);
	return  camPos;
}

void MenuState::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{
	//std::string helpText = "Press -Enter- key to continiue.";
	//std::string titleName = "HORUS PYRAMID";

	spriteBatchPtr->Draw(this->m_titelImage, this->m_titelImagePosition);
	spriteBatchPtr->Draw(this->m_infoImage, this->m_infoImagePosition);
	
	//spriteFontPtr->DrawString(spriteBatchPtr, helpText.c_str(), DirectX::XMFLOAT2((float)this->m_gameOptions.width / 2, (float)this->m_gameOptions.height / 2), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(100.f, 0.f));
	//spriteFontPtr->DrawString(spriteBatchPtr, titleName.c_str(), DirectX::XMFLOAT2((float)this->m_gameOptions.width / 2, (float)this->m_gameOptions.height / 10), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
}