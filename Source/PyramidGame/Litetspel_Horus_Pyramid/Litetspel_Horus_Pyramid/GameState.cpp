#include "pch.h"
#include "GameState.h"

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

void GameState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{
	// Material
	MaterialData mat;
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	GameObject* hook = nullptr;
	GameObject* hookHand = nullptr;

	this->m_device = device;
	this->m_dContext = dContext;

	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[0].loadVertexFromOBJ(device, dContext, L"Models/desertGround.obj", mat, L"Textures/sandTexture.png");

	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	this->addGameObjectToWorld(false, true, 0, 0, &m_models[0], vec, NormalScale, DirectX::XMFLOAT3(1000.f, 10.f, 1000.f));

	// Pyramid
	//,	Model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.65164f, 0.60648f, 0.22648f, 1.0f);
	this->m_models[1].loadVertexFromOBJ(device, dContext, L"Models/PyramidNewModel.obj", mat, L"Textures/pyramidTexture.png");

	vec = DirectX::XMVectorSet(0.f, 0.f, 100.f, 1.f);
	this->addGameObjectToWorld(false, true, 0, 1, &m_models[1], vec, NormalScale);

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

	// HookHead model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[2].loadVertexFromOBJ(device, dContext, L"Models/BirdHookModel.obj", mat, L"Textures/BirdTexture.png");

	// platform model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[3].loadVertexFromOBJ(device, dContext, L"Models/platform.obj", mat, L"Textures/platformTextureCracks1.png");

	// HookHead 
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 2, &m_models[2], vec, NormalScale, DirectX::XMFLOAT3(.5f, .5f, .5f), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hook = this->m_gameObjects.back();
  
	// HookHand
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[4].loadVertexFromOBJ(device, dContext, L"Models/HookModel.obj", mat, L"Textures/HookTexture.png");

	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 4, &m_models[4], vec, DirectX::XMVectorSet(.7f, .7f, .7f, 1.f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hookHand = this->m_gameObjects.back();

	//platform 1
	vec = DirectX::XMVectorSet(-20.f, 20.f, 36.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 2
	vec = DirectX::XMVectorSet(-10.f, 20.f, 36.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 3
	vec = DirectX::XMVectorSet(10.f, 30.f, 48.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 4
	vec = DirectX::XMVectorSet(-40.f, 40.f, 60.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 5
	vec = DirectX::XMVectorSet(30.f, 50.f, 72.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 6
	vec = DirectX::XMVectorSet(-10.f, 60.f, 84.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 7
	vec = DirectX::XMVectorSet(20.f, 70.f, 96.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 8
	vec = DirectX::XMVectorSet(0.f, 80.f, 108.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 9
	vec = DirectX::XMVectorSet(-10.f, 90.f, 120.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 10
	vec = DirectX::XMVectorSet(10.f, 100.f, 132.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 11
	vec = DirectX::XMVectorSet(-10.f, 110.f, 144.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	// platform 12 final
	vec = DirectX::XMVectorSet(0.f, 120.f, 170.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(13.f, 0.5f, 13.f));
	vec = DirectX::XMVectorSet(5.f, 1.f, 5.f, 1.f);
	this->m_gameObjects.back()->setScale(vec);

	vec = DirectX::XMVectorSet(10.f, 10, 0, 1.f);
	this->addGameObjectToWorld(true, true, 2, 3, &m_models[3], vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	vec = DirectX::XMVectorSet(10.f, 15, 0, 1.f);
	this->addGameObjectToWorld(true, true, 2, 3, &m_models[3], vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	vec = DirectX::XMVectorSet(10.f, 25, 0, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	vec = DirectX::XMVectorSet(0.f, 10.f, 24.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
  
  // Chain Link
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[5].loadVertexFromOBJ(device, dContext, L"Models/ChainLink.obj", mat, L"Textures/BirdTexture.png");

  DirectX::XMFLOAT3 vecF3 = hook->getMoveCompPtr()->getPositionF3();
	vec = DirectX::XMVectorSet(vecF3.x, vecF3.y, vecF3.z - 5.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 5, &m_models[5], vec, NormalScale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
  
	this->m_chainGObjects->push_back(this->m_gameObjects.back());

	for (size_t i = 1; i < NR_OF_CHAIN_LINKS; i++)
	{
		vec = DirectX::XMVectorSet(vecF3.x, vecF3.y, vecF3.z - 5.f -((float)i * 0.6f), 1.f);
	  this->addGameObjectToWorld(true, false, 1, 5, &m_models[5], vec, NormalScale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
		this->m_chainGObjects->push_back(this->m_gameObjects.back());
	}
	
	

	// Player
	this->m_player.initialize(-1, -1, 60.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), hook, hookHand, this->m_chainGObjects, audioEngine);
	this->m_player.setPosition(DirectX::XMVectorSet(0.f, 5.f, -1.f, 1.f));
	
	//Room creation
	this->m_rooms.emplace_back(new TemplateRoom());
	this->m_rooms.back()->initialize(m_device, m_dContext, &this->m_models, &this->m_wvpCBuffers, &m_player, XMVectorSet(0, 0, 0, 1));
	dynamic_cast<TemplateRoom*>(this->m_rooms.back())->init();
	m_activeRoom = m_rooms.back();




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
		0.1f,
		options.fov,
		(float)options.width / (float)options.height,
		0.1f,
		1000.f
	);
}

void GameState::update(Keyboard* keyboard, MouseEvent mouseEvent, Mouse* mousePtr, float dt)
{
	// Player
	this->m_player.update(keyboard, mousePtr, dt);

	// Camera
	this->m_camera.update(mouseEvent, dt);

	// Game Objects
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		this->m_gameObjects[i]->update(dt);

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
		this->m_activeRoom->update(dt, &m_camera);
}