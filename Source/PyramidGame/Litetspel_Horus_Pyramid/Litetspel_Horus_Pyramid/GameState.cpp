#include "pch.h"
#include "GameState.h"

using namespace DirectX;

GameState::GameState() 
{
	this->m_device = nullptr;
	this->m_dContext = nullptr;
	this->m_chainGObjects = new std::vector<GameObject*>();
	this->m_activeRoom = nullptr;
	this->m_majorChange = false;
	this->m_gameTime = Timer(true);
	this->m_crossHairSRV = nullptr;
	this->m_crosshairPosition = DirectX::XMFLOAT2();
	this->m_resourceHandler = &ResourceHandler::get();
	this->m_timerString = "";
}

GameState::~GameState() {}

DirectX::XMMATRIX* GameState::getViewMatrix() const
{
	return this->m_camera.getViewMatrixPtr();
}

DirectX::XMMATRIX* GameState::getProjectionMatrix() const
{
	return this->m_camera.getProjectionMatrixPtr();
}

std::vector<Model>* GameState::getModelsPtr()
{
	return this->m_modelsPtr;
}

std::vector<GameObject*>* GameState::getGameObjectsPtr()
{
	return &this->m_gameObjects;
}

std::vector<GameObject*>* GameState::getActiveRoomGameObjectsPtr()
{
	if (this->m_activeRoom != nullptr)
		return this->m_activeRoom->getGameObjectsPtr();
	else
		return nullptr;
}

std::vector<BoundingBox>* GameState::getActiveRoomBoundingBoxsPtr()
{
	if (this->m_activeRoom != nullptr)
		return this->m_activeRoom->getBoundingBoxPtr();
	else
		return nullptr;
}

std::vector<BoundingOrientedBox>* GameState::getActiveRoomOrientedBoundingBoxPtr()
{
	if (this->m_activeRoom != nullptr)
		return this->m_activeRoom->getOrientedBoundingBoxPtr();
	else
		return nullptr;
}

std::vector<BoundingBox>* GameState::getActiveRoomTriggerBox()
{
	if (this->m_activeRoom != nullptr)
		return this->m_activeRoom->getTriggerBoxes();
	else
		return nullptr;
}

std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* GameState::getWvpCBuffersPtr()
{
	return &this->m_wvpCBuffers;
}

constantBufferData* GameState::getConstantBufferData()
{
	return &this->m_constantbufferData;
}

PS_DIR_BUFFER GameState::getActiveRoomDirectionalLight()
{
	if (this->m_activeRoom != nullptr)
	{
		return this->m_activeRoom->getDirectionalLight();
	}
	else
		return PS_DIR_BUFFER();
}

PS_FOG_BUFFER GameState::getActiveRoomFogData()
{
	if (this->m_activeRoom != nullptr)
	{
		return this->m_activeRoom->getFogData();
	}
	else
		return PS_FOG_BUFFER();
}

PS_LIGHT_BUFFER GameState::getActiveRoomLightData()
{
	if (this->m_activeRoom != nullptr)
		return this->m_activeRoom->getLightData();
	else
		return PS_LIGHT_BUFFER();
}

void GameState::addGameObjectToWorld(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1))
{
	this->m_gameObjects.emplace_back(new GameObject());
	GameObject* gObject = this->m_gameObjects.back();
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers.back().init(m_device, m_dContext);
	int bufferIndex = (int)m_wvpCBuffers.size() - 1;
	if (dynamic)
	{
		gObject->initializeDynamic(colide, false, mdlIndx, bufferIndex, weight, acceleration, deceleration, mdl);
	}
	else
	{
		gObject->initializeStatic(colide, mdlIndx, bufferIndex, mdl);
	}

	gObject->setScale(scale3D);
	gObject->setPosition(position);

	if (colide)
	{
		this->m_player.addAABB(gObject->getAABBPtr());
		

	}
	if (boundingBoxSize.x == 0 && boundingBoxSize.y == 0 && boundingBoxSize.z == 0)
	{

	}
	else
		gObject->setBoundingBox(boundingBoxSize);
}

void GameState::addPlatformToWorld(int mdlIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox)
{
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers.back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(new Platform());
	dynamic_cast<Platform*>(this->m_gameObjects.back())->init(true, mdlIndex, (int)m_wvpCBuffers.size() - 1, pyramid, mdl);

	this->m_gameObjects.back()->setPosition(position);
	this->m_gameObjects.back()->setBoundingBox(platformBoundingBox);
	this->m_player.addAABB(this->m_gameObjects.back()->getAABBPtr());
}

void GameState::addLeverToWorld(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB)
{
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers.back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(new Lever());
	dynamic_cast<Lever*>(this->m_gameObjects.back())->init(false, mdlIndex, (int)m_wvpCBuffers.size() - 1, mdl);
	this->m_gameObjects.back()->setPosition(position);
	this->m_gameObjects.back()->setBoundingBox(leverBB);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rotation;
}

void GameState::addPortalToWorld(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room)
{
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers.back().init(m_device, m_dContext);
	int bufferIndex = (int)m_wvpCBuffers.size() - 1;

	this->m_gameObjects.emplace_back(new Portal());
	if (room != -1)
		dynamic_cast<Portal*>(this->m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers.size() - 1, mdl, this->m_rooms[room]->getEntrancePosition(), &this->m_player, room);
	else
		dynamic_cast<Portal*>(this->m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers.size() - 1, mdl, position, &this->m_player, room);


	this->m_gameObjects.back()->setScale(scale3D);
	this->m_gameObjects.back()->setPosition(position);
	this->m_gameObjects.back()->setBoundingBox(boundingBoxSize);
	//this->m_player.addAABB(this->m_gameObjects.back()->getAABBPtr());
}

void GameState::looseALife(bool looseLife)
{
	if (looseLife == true)
	{

	}
}



void GameState::roomChangeInit()
{
	platformBB.clear();
	this->m_gameObjects.resize(this->nrOfGameObjects);
	this->m_player.clearAABB();
	//Gedddwt active room platforms to send to hookHand.
	for (size_t i = 0; this->m_activeRoom && i < this->m_activeRoom->getGameObjectsPtr()->size(); i++)
	{
		
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_activeRoom->getGameObjectsPtr()->at(i));
		if (castToPlatform != nullptr)
		{
			platformBB.emplace_back(castToPlatform->getAABBPtr());
		}

	}
	PyramidRoom* pyramidRoomPtr = dynamic_cast<PyramidRoom*>(this->m_rooms[0]);
	for (size_t i = 0; pyramidRoomPtr && i < pyramidRoomPtr->getBBForHook().size(); i++)
	{
		this->platformBB.emplace_back(pyramidRoomPtr->getBBForHook().at(i));
	}
	this->m_player.updateHookHandBB(platformBB);
	this->m_activeRoom->updatePlayerBB();
	this->m_constantbufferData.dirBuffer = m_activeRoom->getDirectionalLight();
	this->m_constantbufferData.fogBuffer = m_activeRoom->getFogData();
	this->m_constantbufferData.lightBuffer = m_activeRoom->getLightData();

	for (size_t i = 0; i < this->m_activeRoom->getGameObjectsPtr()->size(); i++)
	{
		this->m_gameObjects.emplace_back(this->m_activeRoom->getGameObjectsPtr()->at(i));
	}

}

Timer* GameState::getGameTimerPtr()
{
	return &this->m_gameTime;
}

void GameState::afterChange()
{
	this->roomChangeInit();
}

void GameState::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{
	spriteBatchPtr->Draw(this->m_crossHairSRV, this->m_crosshairPosition);
	spriteFontPtr->DrawString(spriteBatchPtr, this->m_timerString.c_str(), DirectX::XMFLOAT2(10.f, 10.f), this->m_gameTime.isActive() ? DirectX::Colors::White : DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, std::to_string(this->m_gameTime.timeElapsed()).c_str(), DirectX::XMFLOAT2(70.f, 10.f), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
}

void GameState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{
	GameObject* hook = nullptr;
	GameObject* hookHand = nullptr;
	GameObject* hookGem = nullptr;
	GameObject* hookHandLeftWing = nullptr;
	GameObject* hookHandRightWing = nullptr;
	this->m_gameOptions = options;


	this->m_device = device;
	this->m_dContext = dContext;
	//this->loadModels(); //Loads all models

	//-UI START -

	// Timer
	this->m_timerString = "Time: ";

	// Crosshair
	this->m_crossHairSRV = this->m_resourceHandler->getTexture(L"Textures/crosshair.png");
	ID3D11Resource* crosshairTexture = 0;
	this->m_crossHairSRV->GetResource(&crosshairTexture);
	ID3D11Texture2D* crosshairTexture2D = 0;
	crosshairTexture->QueryInterface<ID3D11Texture2D>(&crosshairTexture2D);
	D3D11_TEXTURE2D_DESC desc;
	crosshairTexture2D->GetDesc(&desc);
	int crosshairX = (this->m_gameOptions.width / 2) - (desc.Width / 2);
	int crosshairY = (this->m_gameOptions.height / 2) - (desc.Height / 2);
	this->m_crosshairPosition = DirectX::XMFLOAT2((float)crosshairX, (float)crosshairY);

	//-UI END-

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

	this->m_pyramidOBB = DirectX::BoundingOrientedBox(
		center,
		extents,
		orientation
	);
	this->m_player.addPyramidOBB(&this->m_pyramidOBB);


	// Hook Head 
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 2, &m_modelsPtr->at(2), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(.5f, .5f, .5f), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hook = this->m_gameObjects.back();
  
	// Hook Hand
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 4, &m_modelsPtr->at(4), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHand = this->m_gameObjects.back();

	// Hook Gem
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 32, &m_modelsPtr->at(32), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookGem = this->m_gameObjects.back();

	// Hook Left Wing
	vec = DirectX::XMVectorSet(9.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 30, &m_modelsPtr->at(30), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHandLeftWing = this->m_gameObjects.back();

	// Hook Right Wing
	vec = DirectX::XMVectorSet(11.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 31, &m_modelsPtr->at(31), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHandRightWing = this->m_gameObjects.back();

	// Hook Chain link
    DirectX::XMFLOAT3 vecF3 = hook->getMoveCompPtr()->getPositionF3();
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		vec = DirectX::XMVectorSet(vecF3.x, vecF3.y, vecF3.z - 5.f -((float)i * 0.6f), 1.f);
		this->addGameObjectToWorld(true, false, 1, 5, &m_modelsPtr->at(5), vec, XMVectorSet(.9f, .9f, .9f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
		this->m_chainGObjects->push_back(this->m_gameObjects.back());
	}

	//Possible hook gameobjects
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_gameObjects.at(i));
		if (castToPlatform != nullptr)
		{
			platformBB.emplace_back(castToPlatform->getAABBPtr());
		}
	}

	// Player
	this->m_player.initialize(-1, -1, 60.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), hook, hookHand, hookGem, hookHandLeftWing, hookHandRightWing, this->m_chainGObjects, audioEngine, platformBB);
    this->m_player.setSpawnPosition(DirectX::XMVectorSet(0.f, 10.f, -1.f, 1.f));
	this->m_player.respawn();
	
	//Room creation
	//Pyramid Room - [0]
	this->m_rooms.emplace_back(new PyramidRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<PyramidRoom*>(this->m_rooms.back())->init(&m_pyramidOBB);
	m_activeRoom = m_rooms.back();

	//Template Room [1] //Up for grabs
	this->m_rooms.emplace_back(new TemplateRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<TemplateRoom*>(this->m_rooms.back())->init();

	//Kevin Room [2]
	this->m_rooms.emplace_back(new KevinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(100, 2, 100, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<KevinsRoom*>(this->m_rooms.back())->init();

	//Edvin Room [3]
	this->m_rooms.emplace_back(new EdvinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(-200, 0, 200, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<EdvinsRoom*>(this->m_rooms.back())->init();

	// Tristan Room [4]
	this->m_rooms.emplace_back(new TristansRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -200, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<TristansRoom*>(this->m_rooms.back())->init();
	


	// final Room [5]
	this->m_rooms.emplace_back(new finalRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -200, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<finalRoom*>(this->m_rooms.back())->init();


	//Otaget rum [4] -
	/*this->m_rooms.emplace_back(new NamnRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -100, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<NamnRoom*>(this->m_rooms.back())->init();*/

	this->m_player.getphysicsCompPtr()->setVelocity({ 0, 0, 0 });
	this->m_player.setPosition(this->m_activeRoom->getEntrancePosition());
	this->m_player.getphysicsCompPtr()->setVelocity({ 0, 0, 0 });

	//Get active room platforms to send to hookHand.
	for (size_t i = 0; this->m_activeRoom && i < this->m_activeRoom->getGameObjectsPtr()->size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_activeRoom->getGameObjectsPtr()->at(i));
		if (castToPlatform != nullptr)
		{
			platformBB.emplace_back(castToPlatform->getAABBPtr());
		}
	}
	
	PyramidRoom* pyramidRoomPtr = dynamic_cast<PyramidRoom*>(this->m_rooms[0]);
	for (size_t i = 0; pyramidRoomPtr && i < pyramidRoomPtr->getBBForHook().size(); i++)
	{
		this->platformBB.emplace_back(pyramidRoomPtr->getBBForHook().at(i));
	}
	this->m_player.updateHookHandBB(platformBB);

	//Initialize audio component for platforms and add theire boundingboxes to playerBoundingBoxes
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_gameObjects.at(i));
		if (castToPlatform != nullptr)
		{
			castToPlatform->setPlayerBoundingBox(this->m_player.getAABBPtr());
			castToPlatform->initAudioComponent(audioEngine, m_player.getMoveCompPtr());
		}
	}

	// Camera
	this->m_camera.followMoveComp(this->m_player.getMoveCompPtr());
	this->m_camera.initialize(
		options.mouseSensitivity,
		options.fov,
		(float)options.width / (float)options.height,
		0.1f,
		1000.f
	);

	//Add rooms vector to rooms then add portals
	for (int i = 0; i < this->m_rooms.size(); i++)
	{
		this->m_rooms.at(i)->addRooms(&this->m_rooms);
		this->m_rooms.at(i)->portals();
	}

	this->m_activeRoom->updatePlayerBB();
	this->m_gameTime.start();
	this->nrOfGameObjects = (int)this->m_gameObjects.size();
	this->roomChangeInit();
}

void GameState::update(float dt)
{
	
	// Player
	this->m_player.update(dt);

	// Game Objects from gameState
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
	  Portal* portalPtr = dynamic_cast<Portal*>(this->m_gameObjects[i]);

		if (portalPtr != nullptr)
		{
			portalPtr->update();

			if (portalPtr->shouldChangeActiveRoom())
			{
				m_activeRoom = m_rooms.at(portalPtr->getRoomID());
				portalPtr->resetActiveRoomVariable();
				this->m_majorChange = true;
				this->m_activeRoom->onEntrance();
			}
		}
		else
			this->m_gameObjects[i]->update(dt);

		// World View Projection Matrix Contruction
		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = this->m_camera.getViewMatrix() * this->m_camera.getProjectionMatrix();
		wvpData.wvp = this->m_gameObjects[i]->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = this->m_gameObjects[i]->getWorldMatrix();

		this->m_wvpCBuffers[this->m_gameObjects[i]->getWvpCBufferIndex()].upd(&wvpData);
		if (this->m_gameObjects[i]->m_removeMe)
		{
			this->m_gameObjects[i]->setVisibility(false);
			this->m_gameObjects[i]->setIfCollidable(false);
		}
	}

	if(m_activeRoom != nullptr)
		this->m_activeRoom->update(dt, &m_camera, this->m_activeRoom, m_majorChange);

}

states GameState::handleInput(Keyboard* keyboard, Mouse* mousePtr, float dt)
{
	states changeStateTo = states::NONE;
	// Camera
	while (!mousePtr->empty())
	{
		MouseEvent mEvent = mousePtr->readEvent();
		if (mEvent.getEvent() == Event::MouseRAW_MOVE)
			this->m_camera.update(mEvent, dt);
		else if (mEvent.getEvent() == Event::MouseLPressed)
		{
			this->m_player.shoot();
		}
		else if (mEvent.getEvent() == Event::MouseRPressed)
		{
			this->m_player.retract();
		}
	}
	while (!keyboard->empty())
	{
		KeyboardEvent keyboardEvent = keyboard->readKey();
		if (keyboardEvent.getEvent() == Event::Pressed)
		{
			if (keyboardEvent.getKey() == (char)27)
				changeStateTo = states::POP;
			
		}
	}

	// Controls
		if (keyboard->isKeyPressed('W'))
			this->m_player.movePlayer(Direction::FORWARD, dt);

		if (keyboard->isKeyPressed('S'))
			this->m_player.movePlayer(Direction::BACKWARD, dt);

		if (keyboard->isKeyPressed('A'))
			this->m_player.movePlayer(Direction::LEFT, dt);

		if (keyboard->isKeyPressed('D'))
			this->m_player.movePlayer(Direction::RIGHT, dt);

		if (keyboard->isKeyPressed(' ')) // Space
			this->m_player.jump(dt);

		if (keyboard->isKeyPressed((unsigned char)16)) // Shift
			this->m_player.flyDown(dt);

		if (keyboard->isKeyPressed('E'))
		{
			this->m_player.setUse(true);
		}
		else
		{
			this->m_player.setUse(false);
		}

		// QA Toggle
		if (keyboard->isKeyPressed('P'))
		{
			this->m_player.setQAMode(false);
			StatusTextHandler::get().sendText("QA Mode OFF!", 0.5);
		}
		if (keyboard->isKeyPressed('O'))
		{
			this->m_player.setQAMode(true);
			StatusTextHandler::get().sendText("QA Mode ON!", 0.5);
		}

		// For Debugging purposes
		if (keyboard->isKeyPressed('R'))
		{
			this->m_player.getMoveCompPtr()->position = DirectX::XMVectorSet(0.f, 6.f, -1.f, 1.f);
			this->m_player.resetVelocity(); // Reset Velocity
		}
	return changeStateTo;
}

//void GameState::updateCustomViewLayerVariables(ViewLayer* viewLayer)
//{
//	viewLayer->setgameObjectsFromActiveRoom(this->getActiveRoomGameObjectsPtr());
//	viewLayer->setBoundingBoxesFromActiveRoom(this->getActiveRoomBoundingBoxsPtr());
//	viewLayer->setOrientedBoundingBoxesFromActiveRoom(this->getActiveRoomOrientedBoundingBoxPtr());
//	viewLayer->setTriggerBoxFromActiveRoom(this->getActiveRoomTriggerBox());
//	viewLayer->setGameTimePtr(this->getGameTimerPtr());
//}

XMFLOAT3 GameState::getCameraPos()
{
	return this->m_player.getMoveCompPtr()->getPositionF3();
}
