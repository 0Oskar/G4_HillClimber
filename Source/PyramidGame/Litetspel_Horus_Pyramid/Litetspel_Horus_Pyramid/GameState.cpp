#include "pch.h"
#include "GameState.h"

using namespace DirectX;
void submitHighScore(std::string fileToWriteTo, std::pair<std::string, int>* scoreToWrite, int nrOfScore);

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
	this->m_map = nullptr;
	this->m_mapPosition = DirectX::XMFLOAT2();
	this->m_mapPlayer = nullptr;
	this->m_mapPlayerPosition = DirectX::XMFLOAT2();
	this->m_resourceHandler = &ResourceHandler::get();
	this->m_timerString = "";
	this->m_gameOver = false;
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

std::wstring GameState::getRoomUITexturePath()
{
	return dynamic_cast<FindGemsRoom*>(this->m_rooms.at(1))->getRoomUITexturePath();
}
/*
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

	this->m_models[1].initializeModelBff(m_device, m_dContext, "BasePyramidNew.bff", mat, L"Textures/pyramidTexture.png");

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
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vRoom_New.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGate_New.bff", mat, L"Textures/ColorTexture.png");  

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vLever_New.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem1_New.bff", mat, L"Textures/ColorTexture.png");

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem2_New.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem3_New.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem4_New.bff", mat, L"Textures/ColorTexture.png"); 

	this->m_models.emplace_back(); 
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f); 
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vGem5_New.bff", mat, L"Textures/ColorTexture.png");

	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vPedistal_New.bff", mat, L"Textures/ColorTexture.png");

	this->m_models.emplace_back();
    //START
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[35].loadVertexFromOBJ(m_device, m_dContext, L"Models/PalmTree.obj", mat, L"Textures/ColorTexture.png");


	//32. Hook Highlight Gem
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f); //reset material
	this->m_models[32].initializeModelBff(m_device, m_dContext, "Gauntlet_GreenGem.bff", mat, L"Textures/ColorTexture.png"); //load model
  
	//33. Diamond
	this->m_models.emplace_back(); //add empty model
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f); //reset material
	this->m_models[33].initializeModelBff(m_device, m_dContext, "Diamond.bff", mat, L"Textures/ColorTexture.png"); //load model

	//33. Bell

	//35. Clouds
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[36].loadVertexFromOBJ(m_device, m_dContext, L"Models/Clouds.obj", mat, L"Textures/ColorTexture.png");

	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[++nrOfCurrentLModels].initializeModelBff(m_device, m_dContext, "vRamp_New.bff", mat, L"Textures/ColorTexture.png");
	//END
}
*/

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
	if (m_activeRoom == m_rooms[0])
	{
		spriteBatchPtr->Draw(this->m_map, this->m_mapPosition);
		spriteBatchPtr->Draw(this->m_mapPlayer, this->m_mapPlayerPosition);
	}
	spriteFontPtr->DrawString(spriteBatchPtr, this->m_timerString.c_str(), DirectX::XMFLOAT2(10.f, 10.f), this->m_gameTime.isActive() ? DirectX::Colors::White : DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, std::to_string(this->m_gameTime.timeElapsed()).c_str(), DirectX::XMFLOAT2(70.f, 10.f), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(0.f, 0.f));

	this->m_activeRoom->drawUI(spriteBatchPtr, spriteFontPtr);
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
	
	// Map
	this->m_map = this->m_resourceHandler->getTexture(L"Textures/map.png");
	ID3D11Resource* mapTexture = 0;
	this->m_map->GetResource(&mapTexture);
	ID3D11Texture2D* mapTexture2D = 0;
	mapTexture->QueryInterface<ID3D11Texture2D>(&mapTexture2D);
	mapTexture2D->GetDesc(&desc);
	int mapPositionX = this->m_gameOptions.width - 170;
	int mapPositionY = 50;
	this->m_mapPosition = DirectX::XMFLOAT2((float)mapPositionX, (float)mapPositionY);

	// MapPlayer
	this->m_mapPlayer = this->m_resourceHandler->getTexture(L"Textures/mapPlayerPos.png");
	ID3D11Resource* mapPlayerTexture = 0;
	this->m_mapPlayer->GetResource(&mapPlayerTexture);
	ID3D11Texture2D* mapPlayerTexture2D = 0;
	mapPlayerTexture->QueryInterface<ID3D11Texture2D>(&mapPlayerTexture2D);
	mapTexture2D->GetDesc(&desc);
	int mapPlayerPositionX = this->m_gameOptions.width - 73;
	int mapPlayerPositionY = 108;
	this->m_mapPlayerPosition = DirectX::XMFLOAT2((float)mapPlayerPositionX, (float)mapPlayerPositionY);

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
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1), audioEngine, &this->m_gameTime, options);
	dynamic_cast<PyramidRoom*>(this->m_rooms.back())->init(&m_pyramidOBB);
	m_activeRoom = m_rooms.back();

	//Template Room [1] Viktor
	this->m_rooms.emplace_back(new FindGemsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -300, 1), audioEngine, &this->m_gameTime, options);
	dynamic_cast<FindGemsRoom*>(this->m_rooms.back())->init();
	

	//Kevin Room [2]
	this->m_rooms.emplace_back(new KevinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(100, 2, 100, 1), audioEngine, &this->m_gameTime, options);
	dynamic_cast<KevinsRoom*>(this->m_rooms.back())->init();

	//Edvin Room [3]
	this->m_rooms.emplace_back(new EdvinsRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(-200, 0, 200, 1), audioEngine, &this->m_gameTime, options);
	dynamic_cast<EdvinsRoom*>(this->m_rooms.back())->init();


	// Tristan Room [4]
	this->m_rooms.emplace_back(new TristansRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -200, 1), audioEngine, &this->m_gameTime, options);
	dynamic_cast<TristansRoom*>(this->m_rooms.back())->init();
	

	// final Room [5]
	this->m_rooms.emplace_back(new finalRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, m_modelsPtr, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, -200, 1), audioEngine, &this->m_gameTime, options);
	dynamic_cast<finalRoom*>(this->m_rooms.back())->init();
	

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


	DirectX::XMFLOAT3 playerPos;
	XMStoreFloat3(&playerPos, this->m_player.getPosition());
	this->playerHeight = 4;
	this->pyramidHeight = 585 - playerHeight;
	this->playerPosY = (playerPos.y - playerHeight) / pyramidHeight;		//0 - 1
	
	if (playerPosY >= 1)
		playerPosY = 1;
	if (playerPosY <= 0)
		playerPosY = 0;

	this->markerTop = 108;
	this->markerBottom = 601;
	this->difference = markerBottom - markerTop;		//493
	this->m_mapPlayerPosition.y = markerBottom - (playerPosY * difference);

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

		if (this->m_activeRoom == this->m_rooms[5]) //In final room
		{
			finalRoom* fRoom = dynamic_cast<finalRoom*>(this->m_activeRoom);
			if (fRoom != nullptr && fRoom->wonGame)
			{
				if (!m_gameOver)
				{
					this->highScoreCheck();
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
	fileStream.open(fileToRead);
	int currentTimeElapsed = this->m_gameTime.timeElapsed();
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
				score[nrOf].first = this->m_gameOptions.name;
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
			submitHighScore(this->fileToRead, score, nrOf);
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

XMFLOAT3 GameState::getCameraPos()
{
	return this->m_player.getMoveCompPtr()->getPositionF3();
}
