#include "pch.h"
#include "GameState.h"

using namespace DirectX;

GameState::GameState() 
{
	this->m_device = nullptr;
	this->m_dContext = nullptr;
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

std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* GameState::getWvpCBuffersPtr()
{
	return &this->m_wvpCBuffers;
}

void GameState::addGameObjectToWorld(bool dynamic, bool colide, int weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1))
{
	this->m_gameObjects.emplace_back(new GameObject());
	GameObject* gObject = this->m_gameObjects.back();
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers.back().init(m_device, m_dContext);
	int bufferIndex = m_wvpCBuffers.size() - 1;
	if (dynamic)
	{
		gObject->initializeDynamic(colide, mdlIndx, bufferIndex, weight, acceleration, deceleration, mdl);
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
	dynamic_cast<Platform*>(this->m_gameObjects.back())->init(true, mdlIndex, m_wvpCBuffers.size() - 1, pyramid, mdl);

	this->m_gameObjects.back()->setPosition(position);
	this->m_gameObjects.back()->setBoundingBox(platformBoundingBox);
	this->m_player.addAABB(this->m_gameObjects.back()->getAABBPtr());
}

void GameState::addLeverToWorld(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB)
{
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers.back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(new Lever());
	dynamic_cast<Lever*>(this->m_gameObjects.back())->init(false, mdlIndex, m_wvpCBuffers.size() - 1, mdl);
	this->m_gameObjects.back()->setPosition(position);
	this->m_gameObjects.back()->setBoundingBox(leverBB);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rotation;
}

float GameState::convertDegreesToRadians(float degree)
{
	float radians;
	
	radians = degree / (180 / 3.1415926535);

	return radians;
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
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	this->addGameObjectToWorld(false, true, 0, 0, &m_models[0], vec, NormalScale, DirectX::XMFLOAT3(1000.f, 10.f, 1000.f));

	// Pyramid
	//,	Model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.65164f, 0.60648f, 0.22648f, 1.0f);
	this->m_models[1].loadVertexFromOBJ(device, dContext, L"Models/BasePyr.obj", mat, L"Textures/pyramidTexture.png");

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

	// Sphere "HookHead"
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[2].loadVertexFromOBJ(device, dContext, L"Models/BirdHookModel.obj", mat, L"Textures/BirdTexture.png");

	// platform model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[3].loadVertexFromOBJ(device, dContext, L"Models/platform.obj", mat, L"Textures/platformTextureCracks1.png");

	//HookHead 
	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->addGameObjectToWorld(true, false, 1, 2, &m_models[2], vec, NormalScale, DirectX::XMFLOAT3(.5f, .5f, .5f), DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));
	hook = this->m_gameObjects.back();
	// HookHand
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[4].loadVertexFromOBJ(device, dContext, L"Models/HookModel.obj", mat, L"Textures/HookTexture.png");

	//HookHand
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



	//Puzzle Room (Kevins Lever room)
	vec = DirectX::XMVectorSet(5.f, 25.f, -85.f, 1.f);
	this->addPlatformToWorld(3, &m_pyramidOBB, &m_models[3], vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[5].loadVertexFromOBJ(device, dContext, L"Models/LeverRoom.obj", mat, L"Textures/sandTexture.png");

	vec = DirectX::XMVectorSet(-10.f, 2, -100, 1.f);
	this->addGameObjectToWorld(false, true, 2, 5, &m_models[5], vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	//Lever
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[6].loadVertexFromOBJ(device, dContext, L"Models/Lever.obj", mat, L"Textures/platformTextureCracks1.png");

	vec = DirectX::XMVectorSet(-15.f, 5.f, -88.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, convertDegreesToRadians(-90), convertDegreesToRadians(-270), 1.f);
	this->addLeverToWorld(6, &m_models[6], vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));

	vec = DirectX::XMVectorSet(7.5f, 5.f, -138.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, 0.f, convertDegreesToRadians(-270), 1.f);
	this->addLeverToWorld(6, &m_models[6], vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));

	//Lever* leverPtr = dynamic_cast<Lever*>(this->lever[0]);

	//�ndra pos
	vec = DirectX::XMVectorSet(10.f, 27, -80, 1.f);
	this->addGameObjectToWorld(true, false, 2, 3, &m_models[3], vec, DirectX::XMVectorSet(0.1f, 0.1f, 0.1f, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setDrawBB(true);
	this->trapBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	this->dartTrap.emplace_back(this->m_gameObjects.back());

	vec = DirectX::XMVectorSet(10.f, 27, -60, 1.f);
	this->addGameObjectToWorld(true, false, 2, 3, &m_models[3], vec, DirectX::XMVectorSet(0.1f, 0.1f, 0.1f, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setDrawBB(true);
	this->trapBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	this->dartTrap.emplace_back(this->m_gameObjects.back());

	triggerBB.emplace_back();
	triggerBB.back().Center = DirectX::XMFLOAT3(-10.f, 27.f, -80.f);
	triggerBB.back().Extents = DirectX::XMFLOAT3(20.f, 10.f, 2.5f);

	triggerBB.emplace_back();
	triggerBB.back().Center = DirectX::XMFLOAT3(-10.f, 27.f, -60.f);
	triggerBB.back().Extents = DirectX::XMFLOAT3(20.f, 10.f, 2.5f);

	pussels.emplace_back();
	pussels.back().Center = DirectX::XMFLOAT3(-10.f, 1.0f, -104.0f);
	pussels.back().Extents = DirectX::XMFLOAT3(20.f, 1.5f, 20.f);

	pussels.emplace_back();
	pussels.back().Center = DirectX::XMFLOAT3(-10.f, 1.0f, -154.0f);
	pussels.back().Extents = DirectX::XMFLOAT3(20.f, 1.5f, 20.8f);

	pussels.emplace_back();
	pussels.back().Center = DirectX::XMFLOAT3(-10.f, 11.6f, -86.f);
	pussels.back().Extents = DirectX::XMFLOAT3(20.f, 13.f, 1.5f);

	pussels.emplace_back();
	pussels.back().Center = DirectX::XMFLOAT3(-10.f, 23.f, -77.f);
	pussels.back().Extents = DirectX::XMFLOAT3(20.f, 1.5f, 10.8f);

	pussels.emplace_back();
	pussels.back().Center = DirectX::XMFLOAT3(-10.f, 23.f, -45.4f);
	pussels.back().Extents = DirectX::XMFLOAT3(20.f, 1.5f, 11.8f);

	for (int i = 0; i < pussels.size(); i++)
	{
		this->m_player.addAABB(&pussels.at(i));
	}
	
	
	
	
	//vec = DirectX::XMVectorSet()
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_gameObjects.at(i));
		if (castToPlatform != nullptr)
		{
			platformBB.emplace_back(castToPlatform->getAABBPtr());
		}
	}

	// Player
	this->m_player.initialize(-1, -1, 1.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), hook, hookHand, audioEngine, platformBB);
	this->m_player.setPosition(DirectX::XMVectorSet(0.f, 5.f, -1.f, 1.f));
	
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_gameObjects.at(i));
		if (castToPlatform != nullptr)
		{
			castToPlatform->setPlayerBoundingBox(this->m_player.getAABBPtr());
			castToPlatform->initAudioComponent(audioEngine, m_player.getMoveCompPtr());
		}
	}

	//Lever function
	this->lever[0]->setPlayerBoundingBox(this->m_player.getAABBPtr());
	this->lever[1]->setPlayerBoundingBox(this->m_player.getAABBPtr());

	// Camera
	this->m_camera.followMoveComp(this->m_player.getMoveCompPtr());
	this->m_camera.initialize(
		0.01f,
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

	// DART

		if (triggerBB[0].Intersects(this->m_player.getAABB()))
		{
			if (trapActive1 == true)
			{
				dartFly1 = true;
			}			
		}

		if (triggerBB[1].Intersects(this->m_player.getAABB()))
		{
			if (trapActive2 == true)
			{
				dartFly2 = true;
			}
		}

	for (int i = 0; i < dartTrap.size(); i++)
	{
		if (dartTrap[i]->getAABB().Intersects(this->m_player.getAABB()))
		{
			this->m_player.getMoveCompPtr()->position = DirectX::XMVectorSet(-20.f, -50.f, -165.f, 1.f);
		}
	}
	
	if (dartFly1 == true)
	{
		if (dartPosition1 <= 0)
		{
			dartPosition1 = 40.f;
			dartTrap[0]->getMoveCompPtr()->position = DirectX::XMVectorSet(10.f, 27, -80, 1.f);
			dartFly1 = false;
		}
		else
		{
			dartTrap[0]->getMoveCompPtr()->position += DirectX::XMVectorSet(-40.f * dt, 0, 0, 1.f);
			dartPosition1 -= 40.f * dt;
		}	
	}

	if (dartFly2 == true)
	{
		if (dartPosition2 <= 0)
		{
			dartPosition2 = 40.f;
			dartTrap[1]->getMoveCompPtr()->position = DirectX::XMVectorSet(10.f, 27.f, -60.f, 1.f);
			dartFly2 = false;
		}
		else
		{
			dartTrap[1]->getMoveCompPtr()->position += DirectX::XMVectorSet(-40.f * dt, 0, 0, 1.f);
			dartPosition2 -= 40.f * dt;
		}
	}

	//Lever

	
	this->lever[0]->collidesWithPlayer();

	if (this->lever[0]->getCanUseLever() == true)
	{
		if (this->m_player.getinUse() == true)
		{
			trapActive1 = false;
		}
	}

	this->lever[1]->collidesWithPlayer();

	if (this->lever[1]->getCanUseLever() == true)
	{
		if (this->m_player.getinUse() == true)
		{
			trapActive2 = false;
		}
	}






	

	
}