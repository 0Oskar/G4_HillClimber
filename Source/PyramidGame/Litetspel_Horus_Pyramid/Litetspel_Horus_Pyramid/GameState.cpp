#include "pch.h"
#include "GameState.h"

using namespace DirectX;
void submitHighScore(std::string fileToWriteTo, std::pair<std::string, int>* scoreToWrite, int nrOfScore);

GameState::GameState() 
{
	m_device = nullptr;
	m_dContext = nullptr;
	m_chainGObjects = new std::vector<GameObject*>();
	m_activeRoom = nullptr;
	m_majorChange = false;
	m_gameTime = Timer(true);
	m_crossHairSRV = nullptr;
	m_crosshairPosition = DirectX::XMFLOAT2();
	m_map = nullptr;
	m_mapPosition = DirectX::XMFLOAT2();
	m_mapPlayer = nullptr;
	m_mapPlayerPosition = DirectX::XMFLOAT2();
	m_resourceHandler = &ResourceHandler::get();
	m_timerString = "";
	m_gameOver = false;
}

GameState::~GameState() 
{
	for (int i = 0; i < m_nrOfGameObjects; i++)
	{
		if (m_gameObjects.at(i) != nullptr)
		{
			delete m_gameObjects.at(i);
			m_gameObjects.at(i) = nullptr;
		}
	}

	for (int i = 0; i < (int)m_rooms.size(); i++)
	{
		if (m_rooms.at(i) != nullptr)
		{
			delete m_rooms.at(i);
			m_rooms.at(i) = nullptr;
		}
	}
	m_rooms.clear();
	m_gameObjects.clear();
}

DirectX::XMMATRIX* GameState::getViewMatrix() const
{
	return m_camera.getViewMatrixPtr();
}

DirectX::XMMATRIX* GameState::getProjectionMatrix() const
{
	return m_camera.getProjectionMatrixPtr();
}

std::vector<Model>* GameState::getModelsPtr()
{
	return m_modelsPtr;
}

std::vector<GameObject*>* GameState::getGameObjectsPtr()
{
	return &m_gameObjects;
}

std::vector<GameObject*>* GameState::getActiveRoomGameObjectsPtr()
{
	if (m_activeRoom != nullptr)
		return m_activeRoom->getGameObjectsPtr();
	else
		return nullptr;
}

std::vector<BoundingBox>* GameState::getActiveRoomBoundingBoxesPtr()
{
	if (m_activeRoom != nullptr)
		return m_activeRoom->getBoundingBoxPtr();
	else
		return nullptr;
}

std::vector<BoundingOrientedBox>* GameState::getActiveRoomOrientedBoundingBoxesPtr()
{
	if (m_activeRoom != nullptr)
		return m_activeRoom->getOrientedBoundingBoxPtr();
	else
		return nullptr;
}

std::vector<BoundingBox>* GameState::getActiveRoomTriggerBox()
{
	if (m_activeRoom != nullptr)
		return m_activeRoom->getTriggerBoxes();
	else
		return nullptr;
}

std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* GameState::getWvpCBuffersPtr()
{
	return &m_wvpCBuffers;
}

constantBufferData* GameState::getConstantBufferData()
{
	return &m_constantbufferData;
}	

PS_DIR_BUFFER GameState::getActiveRoomDirectionalLight()
{
	if (m_activeRoom != nullptr)
	{
		return m_activeRoom->getDirectionalLight();
	}
	else
		return PS_DIR_BUFFER();
}

PS_FOG_BUFFER GameState::getActiveRoomFogData()
{
	if (m_activeRoom != nullptr)
	{
		return m_activeRoom->getFogData();
	}
	else
		return PS_FOG_BUFFER();
}

PS_LIGHT_BUFFER GameState::getActiveRoomLightData()
{
	if (m_activeRoom != nullptr)
		return m_activeRoom->getLightData();
	else
		return PS_LIGHT_BUFFER();
}

void GameState::addGameObjectToWorld(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1))
{
	m_gameObjects.emplace_back(new GameObject());
	GameObject* gObject = m_gameObjects.back();
	m_wvpCBuffers.emplace_back();
	m_wvpCBuffers.back().init(m_device, m_dContext);
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
		m_player.addAABB(gObject->getAABBPtr());


	}
	if (boundingBoxSize.x == 0 && boundingBoxSize.y == 0 && boundingBoxSize.z == 0)
	{

	}
	else
		gObject->setBoundingBox(boundingBoxSize);
}

// Thread safe version
static void addGameObjectToWorldStatic(Player* player, GameObject* gObject, unsigned wvpBufferIndex, bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1))
{
	if (dynamic)
	{
		gObject->initializeDynamic(colide, false, mdlIndx, wvpBufferIndex, weight, acceleration, deceleration, mdl);
	}
	else
	{
		gObject->initializeStatic(colide, mdlIndx, wvpBufferIndex, mdl);
	}

	gObject->setScale(scale3D);
	gObject->setPosition(position);

	if (colide)
		player->addAABB(gObject->getAABBPtr());
	if (boundingBoxSize.x != 0 && boundingBoxSize.y != 0 && boundingBoxSize.z != 0)
		gObject->setBoundingBox(boundingBoxSize);
}

void GameState::addPlatformToWorld(int mdlIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox)
{
	m_wvpCBuffers.emplace_back();
	m_wvpCBuffers.back().init(m_device, m_dContext);
	m_gameObjects.emplace_back(new Platform());
	dynamic_cast<Platform*>(m_gameObjects.back())->init(true, mdlIndex, (int)m_wvpCBuffers.size() - 1, pyramid, mdl);

	m_gameObjects.back()->setPosition(position);
	m_gameObjects.back()->setBoundingBox(platformBoundingBox);
	m_player.addAABB(m_gameObjects.back()->getAABBPtr());
}

void GameState::addLeverToWorld(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB)
{
	m_wvpCBuffers.emplace_back();
	m_wvpCBuffers.back().init(m_device, m_dContext);
	m_gameObjects.emplace_back(new Lever());
	dynamic_cast<Lever*>(m_gameObjects.back())->init(false, mdlIndex, (int)m_wvpCBuffers.size() - 1, mdl);
	m_gameObjects.back()->setPosition(position);
	m_gameObjects.back()->setBoundingBox(leverBB);
	m_gameObjects.back()->getMoveCompPtr()->rotation = rotation;
}

void GameState::addPortalToWorld(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room)
{
	m_wvpCBuffers.emplace_back();
	m_wvpCBuffers.back().init(m_device, m_dContext);
	int bufferIndex = (int)m_wvpCBuffers.size() - 1;

	m_gameObjects.emplace_back(new Portal());
	if (room != -1)
		dynamic_cast<Portal*>(m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers.size() - 1, mdl, m_rooms[room]->getEntrancePosition(), &m_player, room);
	else
		dynamic_cast<Portal*>(m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers.size() - 1, mdl, position, &m_player, room);


	m_gameObjects.back()->setScale(scale3D);
	m_gameObjects.back()->setPosition(position);
	m_gameObjects.back()->setBoundingBox(boundingBoxSize);
	//m_player.addAABB(m_gameObjects.back()->getAABBPtr());
}

void GameState::looseALife(bool looseLife)
{
	if (looseLife == true)
	{

	}
}

std::wstring GameState::getRoomUITexturePath()
{
	return dynamic_cast<FindGemsRoom*>(m_rooms.at(1))->getRoomUITexturePath();
}
/*
void GameState::loadModels()
{
	// Material
	MaterialData mat;

	//0 - Desert Ground
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[0].loadVertexFromOBJ(m_device, m_dContext, L"Models/desertGround.obj", mat, L"Textures/sandTexture.png");

	//1 - Pyramid
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.9f, 0.7f, 0.3f, 1.0f);

	m_models[1].initializeModelBff(m_device, m_dContext, "BasePyramidNew.bff", mat, L"Textures/pyramidTexture.png");

	//2- HookHead model
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[2].loadVertexFromOBJ(m_device, m_dContext, L"Models/FinalBird.obj", mat, L"Textures/ColorTexture.png");

	//3- platform model
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[3].loadVertexFromOBJ(m_device, m_dContext, L"Models/platform.obj", mat, L"Textures/platformTextureCracks1.png");

	//4- HookHand
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[4].loadVertexFromOBJ(m_device, m_dContext, L"Models/FinalHook.obj", mat, L"Textures/ColorTexture.png");

	//5- Chain Link
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[5].loadVertexFromOBJ(m_device, m_dContext, L"Models/FinalChainLink.obj", mat, L"Textures/ColorTexture.png");

	//6- Lever Room aka Kevins room
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[6].loadVertexFromOBJ(m_device, m_dContext, L"Models/LeverRoom2.obj", mat, L"Textures/ColorTexture.png");

	//7 - Lever
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[7].loadVertexFromOBJ(m_device, m_dContext, L"Models/Lever.obj", mat, L"Textures/ColorTexture.png");

	//8. PuzzleRoom (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[8].initializeModelBff(m_device, m_dContext, "EdvinPuzzleRoom.bff", mat, L"Textures/ColorTexture.png"); //load model

	//9. Button (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[9].initializeModelBff(m_device, m_dContext, "Button.bff", mat, L"Textures/Hyroglajf_0.png"); //load model

	//10 - Portal
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[10].initializeModelBff(m_device, m_dContext, "PortalGate.bff", mat, L"Textures/ColorTexture.png");

	//11. Brick_1 (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[11].initializeModelBff(m_device, m_dContext, "Brick_1.bff", mat, L"Textures/Hyroglajf_0.png"); //load model

	//12. Brick_2 (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[12].initializeModelBff(m_device, m_dContext, "Brick_2.bff", mat, L"Textures/Hyroglajf_1.png"); //load model

	//13. Brick_3 (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[13].initializeModelBff(m_device, m_dContext, "Brick_3.bff", mat, L"Textures/Hyroglajf_2.png"); //load model

	//14. Brick_4 (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[14].initializeModelBff(m_device, m_dContext, "Brick_4.bff", mat, L"Textures/Hyroglajf_3.png"); //load model

	//15. Brick_5 (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[15].initializeModelBff(m_device, m_dContext, "Brick_5.bff", mat, L"Textures/Hyroglajf_4.png"); //load model
  
	//16. Leaver Base (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[16].initializeModelBff(m_device, m_dContext, "LeverBase.bff", mat, L"Textures/ColorTexture.png"); //load model

	//17. Pedistal (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[17].initializeModelBff(m_device, m_dContext, "Pedistal.bff", mat, L"Textures/ColorTexture.png"); //load model

	//18. Cover (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[18].initializeModelBff(m_device, m_dContext, "Cover.bff", mat, L"Textures/ColorTexture.png"); //load model

	//19. checkpoint
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[19].initializeModelBff(m_device, m_dContext, "checkpointPlatform.bff", mat, L"Textures/ColorTexture.png");

	//20. Leaver Handle (Edvin)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[20].initializeModelBff(m_device, m_dContext, "LeverHandle.bff", mat, L"Textures/ColorTexture.png"); //load model

	//21. PuzzleRoom(Tristan)
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[21].initializeModelBff(m_device, m_dContext, "TristansPuzzleRoom.bff", mat, L"Textures/ColorTexture.png"); //load model

	//22. Pyramid Portal
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); //reset material
	m_models[22].initializeModelBff(m_device, m_dContext, "PuzzleRoomGateExtended.bff", mat, L"Textures/ColorTexture.png"); //load model
  
    //23. Scorpion
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[23].loadVertexFromOBJ(m_device, m_dContext, L"Models/scorpionDone.obj", mat, L"Textures/ColorTexture.png");

	//24. dartWallObj
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[24].loadVertexFromOBJ(m_device, m_dContext, L"Models/dartTrapWall.obj", mat, L"Textures/ColorTexture.png");

	//25. dartWalldarts
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[25].loadVertexFromOBJ(m_device, m_dContext, L"Models/dartTrapWallDart.obj", mat, L"Textures/ColorTexture.png");

	//26. expandingBridge
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[26].loadVertexFromOBJ(m_device, m_dContext, L"Models/expandingBridge.obj", mat, L"Textures/ColorTexture.png");

	//LastNumber. Room (Viktor)
	int nrOfCurrentLModels = 26;

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vRoom_New.bff", mat, L"Textures/ColorTexture.png"); 

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGate_New.bff", mat, L"Textures/ColorTexture.png");  

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vLever_New.bff", mat, L"Textures/ColorTexture.png"); 

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem1_New.bff", mat, L"Textures/ColorTexture.png");

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem2_New.bff", mat, L"Textures/ColorTexture.png"); 

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem3_New.bff", mat, L"Textures/ColorTexture.png"); 

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem4_New.bff", mat, L"Textures/ColorTexture.png"); 

	m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem5_New.bff", mat, L"Textures/ColorTexture.png");

	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vPedistal_New.bff", mat, L"Textures/ColorTexture.png");

	m_models.emplace_back();
    //START
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[35].loadVertexFromOBJ(m_device, m_dContext, L"Models/PalmTree.obj", mat, L"Textures/ColorTexture.png");


	//32. Hook Highlight Gem
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f); //reset material
	m_models[32].initializeModelBff(m_device, m_dContext, "Gauntlet_GreenGem.bff", mat, L"Textures/ColorTexture.png"); //load model
  
	//33. Diamond
	m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f); //reset material
	m_models[33].initializeModelBff(m_device, m_dContext, "Diamond.bff", mat, L"Textures/ColorTexture.png"); //load model

	//33. Bell

	//35. Clouds
	m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_models[36].loadVertexFromOBJ(m_device, m_dContext, L"Models/Clouds.obj", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vRamp_New.bff", mat, L"Textures/ColorTexture.png");
	//END
}
*/

void GameState::roomChangeInit()
{
	m_platformBB.clear();
	m_gameObjects.resize(m_nrOfGameObjects);
	m_player.clearAABB();
	//Get active room platforms to send to hookHand.
	for (size_t i = 0; m_activeRoom && i < m_activeRoom->getGameObjectsPtr()->size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(m_activeRoom->getGameObjectsPtr()->at(i));
		if (castToPlatform != nullptr)
		{
			m_platformBB.emplace_back(castToPlatform->getAABBPtr());
		}

	}
	PyramidRoom* pyramidRoomPtr = dynamic_cast<PyramidRoom*>(m_rooms[0]);
	for (size_t i = 0; pyramidRoomPtr && i < pyramidRoomPtr->getBBForHook().size(); i++)
	{
		m_platformBB.emplace_back(pyramidRoomPtr->getBBForHook().at(i));
	}
	m_player.updateHookHandBB(m_platformBB);
	m_activeRoom->updatePlayerBB();
	m_constantbufferData.dirBuffer = m_activeRoom->getDirectionalLight();
	m_constantbufferData.fogBuffer = m_activeRoom->getFogData();
	m_constantbufferData.lightBuffer = m_activeRoom->getLightData();

	for (size_t i = 0; i < m_activeRoom->getGameObjectsPtr()->size(); i++)
	{
		m_gameObjects.emplace_back(m_activeRoom->getGameObjectsPtr()->at(i));
	}
	Transition::get().fadeOut();
}

Timer* GameState::getGameTimerPtr()
{
	return &m_gameTime;
}

void GameState::afterChange()
{
	roomChangeInit();
}

void GameState::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{
	spriteBatchPtr->Draw(m_crossHairSRV, m_crosshairPosition);
	if (m_activeRoom == m_rooms[0])
	{
		spriteBatchPtr->Draw(m_map, m_mapPosition);
		spriteBatchPtr->Draw(m_mapPlayer, m_mapPlayerPosition);
	}
	spriteFontPtr->DrawString(spriteBatchPtr, m_timerString.c_str(), DirectX::XMFLOAT2(10.f, 10.f), m_gameTime.isActive() ? DirectX::Colors::White : DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, std::to_string(m_gameTime.timeElapsed()).c_str(), DirectX::XMFLOAT2(70.f, 10.f), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(0.f, 0.f));

	m_activeRoom->drawUI(spriteBatchPtr, spriteFontPtr);
}

void GameState::onPop()
{
	iGameState::onPop();
}

void GameState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine, volatile bool* doneLoadingModels)
{
	m_gameOptions = options;


	m_device = device;
	m_dContext = dContext;
	//loadModels(); //Loads all models

	//-UI START -

	// Timer
	m_timerString = "Time: ";

	// Crosshair
	m_crossHairSRV = m_resourceHandler->getTexture(L"Textures/crosshair.png");
	ID3D11Resource* crosshairTexture = 0;
	m_crossHairSRV->GetResource(&crosshairTexture);
	ID3D11Texture2D* crosshairTexture2D = 0;
	crosshairTexture->QueryInterface<ID3D11Texture2D>(&crosshairTexture2D);
	D3D11_TEXTURE2D_DESC desc;
	crosshairTexture2D->GetDesc(&desc);
	int crosshairX = (m_gameOptions.width / 2) - (desc.Width / 2);
	int crosshairY = (m_gameOptions.height / 2) - (desc.Height / 2);
	m_crosshairPosition = DirectX::XMFLOAT2((float)crosshairX, (float)crosshairY);
	
	// Map
	m_map = m_resourceHandler->getTexture(L"Textures/map.png");
	ID3D11Resource* mapTexture = 0;
	m_map->GetResource(&mapTexture);
	ID3D11Texture2D* mapTexture2D = 0;
	mapTexture->QueryInterface<ID3D11Texture2D>(&mapTexture2D);
	mapTexture2D->GetDesc(&desc);
	int mapPositionX = m_gameOptions.width - 170;
	int mapPositionY = 50;
	m_mapPosition = DirectX::XMFLOAT2((float)mapPositionX, (float)mapPositionY);

	// MapPlayer
	m_mapPlayer = m_resourceHandler->getTexture(L"Textures/mapPlayerPos.png");
	ID3D11Resource* mapPlayerTexture = 0;
	m_mapPlayer->GetResource(&mapPlayerTexture);
	ID3D11Texture2D* mapPlayerTexture2D = 0;
	mapPlayerTexture->QueryInterface<ID3D11Texture2D>(&mapPlayerTexture2D);
	mapTexture2D->GetDesc(&desc);
	int mapPlayerPositionX = m_gameOptions.width - 73;
	int mapPlayerPositionY = 108;
	m_mapPlayerPosition = DirectX::XMFLOAT2((float)mapPlayerPositionX, (float)mapPlayerPositionY);

	//-UI END-
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	
	//Pyramid OBB
	DirectX::XMFLOAT3 center(0.f, 62.f, 80.f);
	DirectX::XMFLOAT3 extents(700.f, 800.f, 1.f);
	float rotationX = XMConvertToRadians(46.f);
	LPCWSTR test = std::to_wstring(rotationX).c_str();
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotationX, 0.f, 0.f);
	DirectX::XMFLOAT4 orientation;
	DirectX::XMStoreFloat4(&orientation, quaternion);

	m_pyramidOBB = DirectX::BoundingOrientedBox(
		center,
		extents,
		orientation
	);
	m_player.addPyramidOBB(&m_pyramidOBB);

	GameObject* hook = nullptr;
	GameObject* hookHand = nullptr;
	GameObject* hookGem = nullptr;
	GameObject* hookHandLeftWing = nullptr;
	GameObject* hookHandRightWing = nullptr;

	// Hook Head 
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	addGameObjectToWorld(true, false, 1, 2, &m_modelsPtr->at(2), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(.5f, .5f, .5f), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hook = m_gameObjects.back();

	// Hook Hand
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	addGameObjectToWorld(true, false, 1, 4, &m_modelsPtr->at(4), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHand = m_gameObjects.back();

	// Hook Left Wing
	vec = DirectX::XMVectorSet(9.f, 1.f, -20.f, 1.f);
	addGameObjectToWorld(true, false, 1, 30, &m_modelsPtr->at(30), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHandLeftWing = m_gameObjects.back();

	// Hook Right Wing
	vec = DirectX::XMVectorSet(11.f, 1.f, -20.f, 1.f);
	addGameObjectToWorld(true, false, 1, 31, &m_modelsPtr->at(31), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHandRightWing = m_gameObjects.back();

	// Hook Gem
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	addGameObjectToWorld(true, false, 1, 32, &m_modelsPtr->at(32), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookGem = m_gameObjects.back();

	// Hook Chain link
	DirectX::XMFLOAT3 vecF3 = hook->getMoveCompPtr()->getPositionF3();
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		vec = DirectX::XMVectorSet(vecF3.x, vecF3.y, vecF3.z - 5.f - ((float)i * 0.6f), 1.f);
		addGameObjectToWorld(true, false, 1, 5, &m_modelsPtr->at(5), vec, XMVectorSet(.9f, .9f, .9f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
		m_chainGObjects->push_back(m_gameObjects.back());
	}

	//Possible hook gameobjects
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(m_gameObjects.at(i));
		if (castToPlatform != nullptr)
		{
			m_platformBB.emplace_back(castToPlatform->getAABBPtr());
		}
	}

	// Player
	m_player.initialize(-1, -1, 60.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), hook, hookHand, hookGem, hookHandLeftWing, hookHandRightWing, m_chainGObjects, audioEngine, m_platformBB);
	m_player.setPosition(XMVectorSet(0.f, 8.f, 0.f, 0.f));
	// Player
	/*bool playerInitDoneLoading = false;
	playerInit(&playerInitDoneLoading, m_device, m_dContext, m_modelsPtr, &m_player, &m_pyramidOBB);*/
	
	// Room creation
	// Pyramid Room - [0]
	m_rooms.emplace_back(new PyramidRoom());
	m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1), audioEngine, &m_gameTime, options);
	dynamic_cast<PyramidRoom*>(m_rooms.back())->init(&m_pyramidOBB);
	m_activeRoom = m_rooms.back();
	

	// Viktor Room [1]
	m_rooms.emplace_back(new FindGemsRoom());
	m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -300, 1), audioEngine, &m_gameTime, options);
	dynamic_cast<FindGemsRoom*>(m_rooms.back())->init();

	//Kevin Room [2]
	m_rooms.emplace_back(new KevinsRoom());
	m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &m_wvpCBuffers, &m_player, XMVectorSet(100, 2, 100, 1), audioEngine, &m_gameTime, options);
	dynamic_cast<KevinsRoom*>(m_rooms.back())->init();
	
	//Edvin Room [3]
	m_rooms.emplace_back(new EdvinsRoom());
	m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &m_wvpCBuffers, &m_player, XMVectorSet(-200, 0, 200, 1), audioEngine, &m_gameTime, options);
	dynamic_cast<EdvinsRoom*>(m_rooms.back())->init();

	// Tristan Room [4]
	m_rooms.emplace_back(new TristansRoom());
	m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -200, 1), audioEngine, &m_gameTime, options);
	dynamic_cast<TristansRoom*>(m_rooms.back())->init();

	// final Room [5]
	m_rooms.emplace_back(new finalRoom());
	m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -200, 1), audioEngine, &m_gameTime, options);
	dynamic_cast<finalRoom*>(m_rooms.back())->init();

	m_player.setSpawnPosition(m_activeRoom->getEntrancePosition());

	m_player.setPosition(m_activeRoom->getEntrancePosition());

	//Get active room platforms to send to hookHand.
	for (size_t i = 0; m_activeRoom && i < m_activeRoom->getGameObjectsPtr()->size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(m_activeRoom->getGameObjectsPtr()->at(i));
		if (castToPlatform != nullptr)
		{
			m_platformBB.emplace_back(castToPlatform->getAABBPtr());
		}
	}
	
	PyramidRoom* pyramidRoomPtr = dynamic_cast<PyramidRoom*>(m_rooms[0]);
	for (size_t i = 0; pyramidRoomPtr && i < pyramidRoomPtr->getBBForHook().size(); i++)
	{
		m_platformBB.emplace_back(pyramidRoomPtr->getBBForHook().at(i));
	}
	m_player.updateHookHandBB(m_platformBB);

	//Initialize audio component for platforms and add theire boundingboxes to playerBoundingBoxes
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(m_gameObjects.at(i));
		if (castToPlatform != nullptr)
		{
			castToPlatform->setPlayerBoundingBox(m_player.getAABBPtr());
			castToPlatform->initAudioComponent(audioEngine, m_player.getMoveCompPtr());
		}
	}

	// Camera
	m_camera.followMoveComp(m_player.getMoveCompPtr());
	m_camera.initialize(
		options.mouseSensitivity,
		options.fov,
		(float)options.width / (float)options.height,
		0.1f,
		1000.f
	);

	//Add rooms vector to rooms then add portals
	for (int i = 0; i < m_rooms.size(); i++)
	{
		m_rooms.at(i)->addRooms(&m_rooms);
		m_rooms.at(i)->portals();
	}

	m_activeRoom->updatePlayerBB();

	m_gameTime.start();
	m_nrOfGameObjects = (int)m_gameObjects.size();
	roomChangeInit();
}

static void playerInit(bool* finished, ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* modelList, Player* player, BoundingOrientedBox* pyramidOBB)
{
	//allocator<GameObject> allocGO;
	//allocator<ConstBuffer<VS_CONSTANT_BUFFER>> allocCB;
	//std::vector<GameObject*> gameObjects(5, allocGO);
	//std::vector<ConstBuffer<VS_CONSTANT_BUFFER>> matrixCBuffers(5, allocCB);

	//unsigned index = 0;
	//GameObject* hookHand			= gameObjects[index++];
	//GameObject* hookGem				= gameObjects[index++];
	//GameObject* hookHandLeftWing	= gameObjects[index++];
	//GameObject* hookHandRightWing	= gameObjects[index++];

	//// Hook Head
	//GameObject* hook = gameObjects[index++];
	//XMVECTOR vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	//addGameObjectToWorldStatic(player, hook, matrixCBuffers[index], true, false, 1, 2, &modelList->at(2), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(.5f, .5f, .5f), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	//
	//// Hook Hand
	//vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	//addGameObjectToWorld(true, false, 1, 4, &modelList->at(4), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	//hookHand = m_gameObjects.back();

	//// Hook Gem
	//vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	//addGameObjectToWorld(true, false, 1, 32, &modelList->at(32), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	//hookGem = m_gameObjects.back();

	//// Hook Left Wing
	//vec = DirectX::XMVectorSet(9.f, 1.f, -20.f, 1.f);
	//addGameObjectToWorld(true, false, 1, 30, &modelList->at(30), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	//hookHandLeftWing = m_gameObjects.back();

	//// Hook Right Wing
	//vec = DirectX::XMVectorSet(11.f, 1.f, -20.f, 1.f);
	//addGameObjectToWorld(true, false, 1, 31, &modelList->at(31), vec, XMVectorSet(.5f, .5f, .5f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	//hookHandRightWing = m_gameObjects.back();

	//// Hook Chain link
 //   DirectX::XMFLOAT3 vecF3 = hook->getMoveCompPtr()->getPositionF3();
	//for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	//{
	//	vec = DirectX::XMVectorSet(vecF3.x, vecF3.y, vecF3.z - 5.f -((float)i * 0.6f), 1.f);
	//	addGameObjectToWorld(true, false, 1, 5, &modelList->at(5), vec, XMVectorSet(.9f, .9f, .9f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	//	m_chainGObjects->push_back(m_gameObjects.back());
	//}

	////Possible hook gameobjects
	//for (size_t i = 0; i < m_gameObjects.size(); i++)
	//{
	//	Platform* castToPlatform = dynamic_cast<Platform*>(m_gameObjects.at(i));
	//	if (castToPlatform != nullptr)
	//	{
	//		m_platformBB.emplace_back(castToPlatform->getAABBPtr());
	//	}
	//}

	//// Player
	//player->initialize(-1, -1, 60.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), hook, hookHand, hookGem, hookHandLeftWing, hookHandRightWing, m_chainGObjects, audioEngine, m_platformBB);
	//player->setPosition(XMVectorSet(0.f, 8.f, 0.f, 0.f));
	//player->addPyramidOBB(pyramidOBB);

	//*finished = true;
}

void GameState::update(float dt)
{
	m_constantbufferData.dirBuffer = m_activeRoom->getDirectionalLight();
	m_constantbufferData.fogBuffer = m_activeRoom->getFogData();
	m_constantbufferData.lightBuffer = m_activeRoom->getLightData();
	
	// Player
	m_player.update(dt);

	// Game Objects from gameState
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
	  Portal* portalPtr = dynamic_cast<Portal*>(m_gameObjects[i]);

		if (portalPtr != nullptr)
		{
			portalPtr->update();

			if (portalPtr->shouldChangeActiveRoom())
			{
				m_activeRoom = m_rooms.at(portalPtr->getRoomID());
				portalPtr->resetActiveRoomVariable();
				m_majorChange = true;
				m_activeRoom->onEntrance();
			}
		}
		else
			m_gameObjects[i]->update(dt);

		// World View Projection Matrix Contruction
		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = m_camera.getViewMatrix() * m_camera.getProjectionMatrix();
		wvpData.wvp = m_gameObjects[i]->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = m_gameObjects[i]->getWorldMatrix();

		m_wvpCBuffers[m_gameObjects[i]->getWvpCBufferIndex()].upd(&wvpData);
		if (m_gameObjects[i]->m_removeMe)
		{
			m_gameObjects[i]->setVisibility(false);
			m_gameObjects[i]->setIfCollidable(false);
		}
	}

	if(m_activeRoom != nullptr)
		m_activeRoom->update(dt, &m_camera, m_activeRoom, m_majorChange);


	DirectX::XMFLOAT3 playerPos;
	XMStoreFloat3(&playerPos, m_player.getPosition());
	m_playerHeight = 4;
	m_pyramidHeight = 585 - m_playerHeight;
	m_playerPosY = (playerPos.y - m_playerHeight) / m_pyramidHeight;		//0 - 1
	
	if (m_playerPosY >= 1)
		m_playerPosY = 1;
	if (m_playerPosY <= 0)
		m_playerPosY = 0;

	m_markerTop = 108;
	m_markerBottom = 601;
	m_difference = m_markerBottom - m_markerTop;		//493
	m_mapPlayerPosition.y = m_markerBottom - (m_playerPosY * m_difference);

}

states GameState::handleInput(Keyboard* keyboard, Mouse* mousePtr, float dt)
{
	states changeStateTo = states::NONE;
	while (!keyboard->empty())
	{
		KeyboardEvent keyboardEvent = keyboard->readKey();
		if (keyboardEvent.getEvent() == Event::Pressed)
		{
			if (keyboardEvent.getKey() == (char)27)
				changeStateTo = states::POP;

		}
	}

	if (!m_player.getIsSpawning())
	{
		// Camera
		while (!mousePtr->empty())
		{
			MouseEvent mEvent = mousePtr->readEvent();
			if (mEvent.getEvent() == Event::MouseRAW_MOVE)
				m_camera.update(mEvent, dt);
			else if (mEvent.getEvent() == Event::MouseLPressed)
			{
				m_player.shoot();
			}
			else if (mEvent.getEvent() == Event::MouseRPressed)
			{
				m_player.retract();
			}
		}

		// Controls
		if (keyboard->isKeyPressed('W'))
			m_player.movePlayer(Direction::FORWARD, dt);

		if (keyboard->isKeyPressed('S'))
			m_player.movePlayer(Direction::BACKWARD, dt);

		if (keyboard->isKeyPressed('A'))
			m_player.movePlayer(Direction::LEFT, dt);

		if (keyboard->isKeyPressed('D'))
			m_player.movePlayer(Direction::RIGHT, dt);

		if (keyboard->isKeyPressed(' ')) // Space
		{
			if (m_player.getQAMode())
				m_player.movePlayer(Direction::UP, dt);
			else
				m_player.jump(dt);
		}

		if (keyboard->isKeyPressed((unsigned char)16)) // Shift
			m_player.flyDown(dt);

		if (keyboard->isKeyPressed('E'))
		{
			m_player.setUse(true);
		}
		else
		{
			m_player.setUse(false);
		}

		// QA Toggle
		if (keyboard->isKeyPressed('P'))
		{
			m_player.setQAMode(false);
			StatusTextHandler::get().sendText("QA Mode OFF!", 0.5);
		}
		if (keyboard->isKeyPressed('O'))
		{
			m_player.setQAMode(true);
			StatusTextHandler::get().sendText("QA Mode ON!", 0.5);
		}

		// For Debugging purposes
		if (keyboard->isKeyPressed('R'))
		{
			m_player.respawnLogic();
			m_player.resetVelocity(); // Reset Velocity
		}
	}

	if (m_activeRoom == m_rooms[5]) //In final room
	{
		finalRoom* fRoom = dynamic_cast<finalRoom*>(m_activeRoom);
		if (fRoom != nullptr && fRoom->wonGame)
		{
			if (!m_gameOver)
			{
				highScoreCheck();
				changeStateTo = states::WON;
				m_gameOver = true;
			}
			else
			{
				changeStateTo = states::POP;
			}
		}
	}
	return changeStateTo;
}

void GameState::highScoreCheck()
{
	fstream fileStream;
	fileStream.open(m_fileToRead);
	int currentTimeElapsed = (int)m_gameTime.timeElapsed();
	int time;
	std::string name;
	std::pair<std::string, int> score[10];
	int nrOf = 0;
	bool change = false;
	if (fileStream.is_open())
	{
		while (!fileStream.eof() && nrOf < 10)
		{
			fileStream >> name;
			fileStream >> time;
			if ((time > currentTimeElapsed) && (!change))
			{
				score[nrOf].first = m_gameOptions.name;
				score[nrOf++].second = currentTimeElapsed;
				change = true;
			}
			
			if (nrOf < 10 && name != "")
			{
				score[nrOf].first = name;
				score[nrOf++].second = time;
			}
			
		}
		fileStream.close();

		if (change)
		{
			submitHighScore(m_fileToRead, score, nrOf);
		}
	}
}

void submitHighScore(std::string fileToWriteTo, std::pair<std::string, int>* scoreToWrite, int nrOfScore)
{
	std::ofstream outputStream;
	outputStream.open(fileToWriteTo); //File to write
	if (outputStream.is_open())
	{
		for (int i = 0; i < nrOfScore; i++) //Print out all the node names
		{
			outputStream << scoreToWrite[i].first << " ";
			outputStream << std::to_string(scoreToWrite[i].second);
			if (i + 1 < nrOfScore)
			{
				outputStream << "\n";
			}
		}
	}
}

XMFLOAT3 GameState::getCameraPos() const
{
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, m_player.getPosition());

	return temp;
}
