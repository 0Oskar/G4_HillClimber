#include "pch.h"
#include "GameState.h"

using namespace DirectX;

GameState::GameState() 
{
	this->m_device = nullptr;
	this->m_dContext = nullptr;
	this->m_chainGObjects = new std::vector<GameObject*>();
	this->m_activeRoom = nullptr;
	this->m_activeRoomChanged = false;
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
	return &this->m_models;
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


std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* GameState::getWvpCBuffersPtr()
{
	return &this->m_wvpCBuffers;
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
	dynamic_cast<Portal*>(this->m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers.size() - 1, mdl, teleportLocation, &this->m_player, room);


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

void GameState::loadModels()
{
	// Material
	MaterialData mat;

	//0 - Desert Ground
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[0].loadVertexFromOBJ(this->m_device, this->m_dContext, L"Models/desertGround.obj", mat, L"Textures/sandTexture.png");

	//1 - Pyramid
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.65164f, 0.60648f, 0.22648f, 1.0f);
	this->m_models[1].loadVertexFromOBJ(m_device, m_dContext, L"Models/PyramidNewModel.obj", mat, L"Textures/pyramidTexture.png");

	//2- HookHead model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[2].loadVertexFromOBJ(m_device, m_dContext, L"Models/BirdHookModel.obj", mat, L"Textures/BirdTexture.png");

	//3- platform model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[3].loadVertexFromOBJ(m_device, m_dContext, L"Models/platform.obj", mat, L"Textures/platformTextureCracks1.png");

	//4- HookHand
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[4].loadVertexFromOBJ(m_device, m_dContext, L"Models/HookModel.obj", mat, L"Textures/HookTexture.png");

	//5- Chain Link
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[5].loadVertexFromOBJ(m_device, m_dContext, L"Models/ChainLink.obj", mat, L"Textures/BirdTexture.png");

	//6- Lever Room aka Kevins room
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[6].loadVertexFromOBJ(m_device, m_dContext, L"Models/LeverRoom.obj", mat, L"Textures/ColorTexture.png");

	//7 - Lever
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[7].loadVertexFromOBJ(m_device, m_dContext, L"Models/Lever.obj", mat, L"Textures/platformTextureCracks1.png");

	//8. PuzzleRoom (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[8].initializeModelBff(m_device, m_dContext, "EdvinPuzzleRoom.bff", mat, L"Textures/ColorTexture.png"); //load model

	//9. Button (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[9].initializeModelBff(m_device, m_dContext, "Button.bff", mat, L"Textures/BirdHyroglajf.png"); //load model

	//10 - Portal
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[10].loadVertexFromOBJ(m_device, m_dContext, L"Models/PuzzleRoomGate.obj", mat, L"Textures/PuzzleDoor_Texture2.png");

	//11. Brick_1 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[11].initializeModelBff(m_device, m_dContext, "Brick_1.bff", mat, L"Textures/BirdHyroglajf.png"); //load model

	//12. Brick_2 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[12].initializeModelBff(m_device, m_dContext, "Brick_2.bff", mat, L"Textures/BirdHyroglajf.png"); //load model

	//13. Brick_3 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[13].initializeModelBff(m_device, m_dContext, "Brick_3.bff", mat, L"Textures/BirdHyroglajf.png"); //load model

	//14. Brick_4 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[14].initializeModelBff(m_device, m_dContext, "Brick_4.bff", mat, L"Textures/BirdHyroglajf.png"); //load model


	//15. Brick_5 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[15].initializeModelBff(m_device, m_dContext, "Brick_5.bff", mat, L"Textures/BirdHyroglajf.png"); //load model

	//16. Scorpion
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[16].loadVertexFromOBJ(m_device, m_dContext, L"Models/scorpionDone.obj", mat, L"Textures/ColorTexture.png");

	//17. dartWallObj
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[17].loadVertexFromOBJ(m_device, m_dContext, L"Models/dartTrapWall.obj", mat, L"Textures/ColorTexture.png");

	//18. dartWalldarts
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[18].loadVertexFromOBJ(m_device, m_dContext, L"Models/dartTrapWallDart.obj", mat, L"Textures/ColorTexture.png");

	//19. expandingBridge
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[19].loadVertexFromOBJ(m_device, m_dContext, L"Models/expandingBridge.obj", mat, L"Textures/ColorTexture.png");

	}

void GameState::roomChangeInit()
{
	platformBB.clear();
	//Get active room platforms to send to hookHand.
	for (size_t i = 0; this->m_activeRoom && i < this->m_activeRoom->getGameObjectsPtr()->size(); i++)
	{
		
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_activeRoom->getGameObjectsPtr()->at(i));
		if (castToPlatform != nullptr)
		{
			platformBB.emplace_back(castToPlatform->getAABBPtr());
		}
	}

	this->m_player.updateHookHandBB(platformBB);

}

void GameState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{
	GameObject* hook = nullptr;
	GameObject* hookHand = nullptr;
	this->m_device = device;
	this->m_dContext = dContext;
	this->loadModels(); //Loads all models

	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	
	//Pyramid OBB
	DirectX::XMFLOAT3 center(0.f, 52.f, 80.f);
	DirectX::XMFLOAT3 extents(80.f, 105.f, 1.f);
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(0.9f, 0.f, 0.f);
	DirectX::XMFLOAT4 orientation;
	DirectX::XMStoreFloat4(&orientation, quaternion);

	this->m_pyramidOBB = DirectX::BoundingOrientedBox(
		center,
		extents,
		orientation
	);
	this->m_player.addPyramidOBB(&this->m_pyramidOBB);


	// HookHead 
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 2, &m_models[2], vec, NormalScale, DirectX::XMFLOAT3(.5f, .5f, .5f), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hook = this->m_gameObjects.back();
  
	//HookHand
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 4, &m_models[4], vec, DirectX::XMVectorSet(.7f, .7f, .7f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHand = this->m_gameObjects.back();

	//Chain link
    DirectX::XMFLOAT3 vecF3 = hook->getMoveCompPtr()->getPositionF3();
	vec = DirectX::XMVectorSet(vecF3.x, vecF3.y, vecF3.z - 5.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 5, &m_models[5], vec, NormalScale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->m_chainGObjects->push_back(this->m_gameObjects.back());

	//Chain link stuff
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		vec = DirectX::XMVectorSet(vecF3.x, vecF3.y, vecF3.z - 5.f -((float)i * 0.6f), 1.f);
	  this->addGameObjectToWorld(true, false, 1, 5, &m_models[5], vec, NormalScale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
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
	this->m_player.initialize(-1, -1, 60.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), hook, hookHand, this->m_chainGObjects, audioEngine, platformBB);
	
	//Room creation
	//Pyramid Room - [0]
	this->m_rooms.emplace_back(new PyramidRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1), audioEngine);
	dynamic_cast<PyramidRoom*>(this->m_rooms.back())->init(&m_pyramidOBB);

	//Template Room [1]
	this->m_rooms.emplace_back(new TemplateRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1), audioEngine);
	dynamic_cast<TemplateRoom*>(this->m_rooms.back())->init();

	//Kevin Room [2]
	this->m_rooms.emplace_back(new KevinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(100, 0, 100, 1), audioEngine); //2 innan i y
	dynamic_cast<KevinsRoom*>(this->m_rooms.back())->init();
	m_activeRoom = m_rooms.back();

	//Edvins Room [3]
	this->m_rooms.emplace_back(new EdvinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -100, 1), audioEngine);
	dynamic_cast<EdvinsRoom*>(this->m_rooms.back())->init();
	

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
}

void GameState::update(Keyboard* keyboard, MouseEvent mouseEvent, Mouse* mousePtr, float dt)
{
	// Player
	this->m_player.update(keyboard, mousePtr, dt);

	// Camera
	this->m_camera.update(mouseEvent, dt);

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
				this->m_activeRoomChanged = true;
			}
		}
		else
		{
			this->m_gameObjects[i]->update(dt);
		}

		// World View Projection Matrix Contruction
		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = this->m_camera.getViewMatrix() * this->m_camera.getProjectionMatrix();
		wvpData.wvp = this->m_gameObjects[i]->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = this->m_gameObjects[i]->getWorldMatrix();

		this->m_wvpCBuffers[this->m_gameObjects[i]->getWvpCBufferIndex()].upd(&wvpData);
		if (m_gameObjects[i]->m_removeMe)
		{
			delete m_gameObjects[i];
			m_gameObjects[i] = nullptr;
			m_gameObjects.erase(m_gameObjects.begin() + i);
		}
	}

	if(m_activeRoom != nullptr)
		this->m_activeRoom->update(dt, &m_camera, this->m_activeRoom, m_activeRoomChanged);
}