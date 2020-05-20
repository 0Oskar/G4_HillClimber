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
	mat.diffuse = DirectX::XMFLOAT4(0.9f, 0.7f, 0.3f, 1.0f);
	this->m_models[1].loadVertexFromOBJ(m_device, m_dContext, L"Models/FinalPyramid.obj", mat, L"Textures/pyramidTexture.png");

	//2- HookHead model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[2].loadVertexFromOBJ(m_device, m_dContext, L"Models/FinalBird.obj", mat, L"Textures/ColorTexture.png");

	//3- platform model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[3].loadVertexFromOBJ(m_device, m_dContext, L"Models/platform.obj", mat, L"Textures/platformTextureCracks1.png");

	//4- HookHand
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[4].loadVertexFromOBJ(m_device, m_dContext, L"Models/FinalHook.obj", mat, L"Textures/ColorTexture.png");

	//5- Chain Link
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[5].loadVertexFromOBJ(m_device, m_dContext, L"Models/FinalChainLink.obj", mat, L"Textures/ColorTexture.png");

	//6- Lever Room aka Kevins room
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[6].loadVertexFromOBJ(m_device, m_dContext, L"Models/LeverRoom2.obj", mat, L"Textures/ColorTexture.png");

	//7 - Lever
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[7].loadVertexFromOBJ(m_device, m_dContext, L"Models/Lever.obj", mat, L"Textures/ColorTexture.png");

	//8. PuzzleRoom (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[8].initializeModelBff(m_device, m_dContext, "EdvinPuzzleRoom.bff", mat, L"Textures/ColorTexture.png"); //load model

	//9. Button (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[9].initializeModelBff(m_device, m_dContext, "Button.bff", mat, L"Textures/Hyroglajf_0.png"); //load model

	//10 - Portal
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[10].initializeModelBff(m_device, m_dContext, "PortalGate.bff", mat, L"Textures/ColorTexture.png");

	//11. Brick_1 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[11].initializeModelBff(m_device, m_dContext, "Brick_1.bff", mat, L"Textures/Hyroglajf_0.png"); //load model

	//12. Brick_2 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[12].initializeModelBff(m_device, m_dContext, "Brick_2.bff", mat, L"Textures/Hyroglajf_1.png"); //load model

	//13. Brick_3 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[13].initializeModelBff(m_device, m_dContext, "Brick_3.bff", mat, L"Textures/Hyroglajf_2.png"); //load model

	//14. Brick_4 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[14].initializeModelBff(m_device, m_dContext, "Brick_4.bff", mat, L"Textures/Hyroglajf_3.png"); //load model

	//15. Brick_5 (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[15].initializeModelBff(m_device, m_dContext, "Brick_5.bff", mat, L"Textures/Hyroglajf_4.png"); //load model
  
	//16. Leaver Base (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[16].initializeModelBff(m_device, m_dContext, "LeverBase.bff", mat, L"Textures/ColorTexture.png"); //load model

	//17. Pedistal (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[17].initializeModelBff(m_device, m_dContext, "Pedistal.bff", mat, L"Textures/ColorTexture.png"); //load model

	//18. Cover (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[18].initializeModelBff(m_device, m_dContext, "Cover.bff", mat, L"Textures/ColorTexture.png"); //load model

	//19. checkpoint
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[19].initializeModelBff(m_device, m_dContext, "checkpointPlatform.bff", mat, L"Textures/ColorTexture.png");

	//20. Leaver Handle (Edvin)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[20].initializeModelBff(m_device, m_dContext, "LeverHandle.bff", mat, L"Textures/ColorTexture.png"); //load model

	//21. PuzzleRoom(Tristan)
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[21].initializeModelBff(m_device, m_dContext, "TristansPuzzleRoom.bff", mat, L"Textures/ColorTexture.png"); //load model

	//22. Pyramid Portal
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	this->m_models[22].initializeModelBff(m_device, m_dContext, "PuzzleRoomGateExtended.bff", mat, L"Textures/ColorTexture.png"); //load model
  
    //23. Scorpion
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[23].loadVertexFromOBJ(m_device, m_dContext, L"Models/scorpionDone.obj", mat, L"Textures/ColorTexture.png");

	//24. dartWallObj
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[24].loadVertexFromOBJ(m_device, m_dContext, L"Models/dartTrapWall.obj", mat, L"Textures/ColorTexture.png");

	//25. dartWalldarts
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[25].loadVertexFromOBJ(m_device, m_dContext, L"Models/dartTrapWallDart.obj", mat, L"Textures/ColorTexture.png");

	//26. expandingBridge
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[26].loadVertexFromOBJ(m_device, m_dContext, L"Models/expandingBridge.obj", mat, L"Textures/ColorTexture.png");

	//LastNumber. Room (Viktor)
	int nrOfCurrentLModels = 26;

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vRoom.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGate.bff", mat, L"Textures/ColorTexture.png");  

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vLeaver.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem1.bff", mat, L"Textures/ColorTexture.png");

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem2.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem3.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem4.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem5.bff", mat, L"Textures/ColorTexture.png");

	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vPedistal.bff", mat, L"Textures/ColorTexture.png");
}


void GameState::roomChangeInit()
{
	platformBB.clear();
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

}

Timer* GameState::getGameTimerPtr()
{
	return &this->m_gameTime;
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
	DirectX::XMFLOAT3 center(0.f, 62.f, 80.f);
	DirectX::XMFLOAT3 extents(200.f, 250.f, 1.f);
	float rotationX = XMConvertToRadians(45.f);
	LPCWSTR test = std::to_wstring(rotationX).c_str();
	OutputDebugStringW(test);
	OutputDebugStringW(L"\n");
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotationX, 0.f, 0.f);
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
    this->m_player.setSpawnPosition(DirectX::XMVectorSet(0.f, 10.f, -1.f, 1.f));
	this->m_player.respawn();
	
	//Room creation
	//Pyramid Room - [0]
	this->m_rooms.emplace_back(new PyramidRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<PyramidRoom*>(this->m_rooms.back())->init(&m_pyramidOBB);
	m_activeRoom = m_rooms.back();

	//Template Room [1] Viktor
	this->m_rooms.emplace_back(new FindGemsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 300, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<FindGemsRoom*>(this->m_rooms.back())->init();

	//Kevin Room [2]
	this->m_rooms.emplace_back(new KevinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(100, 2, 100, 1), audioEngine, &this->m_gameTime);

	dynamic_cast<KevinsRoom*>(this->m_rooms.back())->init();
	//m_activeRoom = m_rooms.back();

	//Edvin Room [3]
	this->m_rooms.emplace_back(new EdvinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(-200, 0, 200, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<EdvinsRoom*>(this->m_rooms.back())->init();

	// Tristan Room [4]
	this->m_rooms.emplace_back(new TristansRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -200, 1), audioEngine, &this->m_gameTime);
	dynamic_cast<TristansRoom*>(this->m_rooms.back())->init();

	//Otaget rum [4] -
	/*this->m_rooms.emplace_back(new NamnRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -100, 1), audioEngine, &this->m_gameTime);
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

	this->addPortalToWorld({ 0, 0, 0, 0 }, 10, &m_models[10], { -40, 4, -5, 1 }, { 1, 1, 1, 1 }, { 2, 2, 1 }, 0); //Pyramid "Room"
	this->addPortalToWorld({ 0, 0, 0, 0 }, 10, &m_models[10], { -30, 4, -5, 1 }, { 1, 1, 1, 1 }, { 2, 2, 1 }, 1); // FindGemsRoom "Viktor"
	this->addPortalToWorld({ 0, 0, 0, 0 }, 10, &m_models[10], { -20, 4, -5, 1 }, { 1, 1, 1, 1 }, { 2, 2, 1 }, 2); // Kevins room
	this->addPortalToWorld({ 0, 0, 0, 0 }, 10, & m_models[10], { -10, 4, -5, 1 }, { 1, 1, 1, 1 }, { 2, 2, 1 }, 3); //Edvins room
	this->addPortalToWorld({ 0, 0, 0, 0 }, 10, &m_models[10], { 30, 4, -5, 1 }, { 1, 1, 1, 1 }, { 2, 2, 1 }, 4); //Tristans Room

	this->m_gameTime.start();
}

void GameState::update(Keyboard* keyboard, MouseEvent mouseEvent, Mouse* mousePtr, float dt)
{
	while (!mousePtr->empty())
	{
		MouseEvent mEvent = mousePtr->readEvent();
		if (mEvent.getEvent() == Event::MouseRAW_MOVE)
		{
			this->m_camera.update(mEvent, dt);
		}
	}

	// Player
	this->m_player.update(keyboard, mousePtr, dt);

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
		this->m_activeRoom->update(dt, &m_camera, this->m_activeRoom, m_activeRoomChanged);

}