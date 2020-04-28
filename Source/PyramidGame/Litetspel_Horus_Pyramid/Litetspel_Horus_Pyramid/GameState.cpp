#include "pch.h"
#include "GameState.h"

GameState::GameState() {}

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

std::vector<GameObject>* GameState::getGameObjectsPtr()
{
	return &this->m_gameObjects;
}

std::vector<Platform>* GameState::getPlatformPtr()
{
	return &this->m_platforms;
}
std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* GameState::getWvpCBuffersPtr()
{
	return &this->m_wvpCBuffers;
}

void GameState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{
	// Material
	MaterialData mat;

	// Models
	this->m_gameObjects.resize(4);
	this->m_platforms.resize(13);

	// Ground Object
	/*std::vector<Vertex> groundvertices =
	{
		Vertex(
			-1.f, 0.f, -1.f,
			0.f, 1.f, 0.f,
			0, 0
		),
		Vertex(
			-1.f, 0.f, 1.f,
			0.f, 1.f, 0.f,
			0, 0
		),
		Vertex(
			1.f, 0.f, 1.f,
			0.f, 1.f, 0.f,
			0, 0
		),
		Vertex(
			-1.f, 0.f, -1.f,
			0.f, 1.f, 0.f,
			0, 0
		),
		Vertex(
			1.f, 0.f, 1.f,
			0.f, 1.f, 0.f,
			0, 0
		),
		Vertex(
			1.f, 0.f, -1.f,
			0.f, 1.f, 0.f,
			0, 0
		)
	};*/
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	//this->m_models[0].loadVertexVector(device, dContext, groundvertices, mat);
	this->m_models[0].loadVertexFromOBJ(device, dContext, L"Models/desertGround.obj", mat, L"Textures/sandTexture.png");

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[0].init(device, dContext);

	this->m_gameObjects[0].initializeStatic(true, 0, 0, &m_models[0]);

	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	this->m_gameObjects[0].setPosition(vec);
	this->m_gameObjects[0].setBoundingBox(DirectX::XMFLOAT3(1000.f, 10.f, 1000.f));
	this->m_player.addAABB(this->m_gameObjects[0].getAABBPtr()); // add Bounding Box to player collidables

	// Pyramid
	//,	Model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.65164f, 0.60648f, 0.22648f, 1.0f);
	this->m_models[1].loadVertexFromOBJ(device, dContext, L"Models/BasePyr.obj", mat, L"Textures/pyramidTexture.png");

	//,	WVP Buffer
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[1].init(device, dContext);

	//,	Game Object
	this->m_gameObjects[1].initializeStatic(false, 1, 1, &m_models[1]);

	//,	Movement changes
	vec = DirectX::XMVectorSet(0.f, 0.f, 100.f, 1.f);
	this->m_gameObjects[1].setPosition(vec);

	// 
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

	//// Cube 1
	//this->m_models.emplace_back();
	//mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	//this->m_models[3].loadVertexFromOBJ(device, dContext, L"Models/cube.obj", mat, L"Textures/pyramidTexture.png");

	//this->m_wvpCBuffers.emplace_back();
	//this->m_wvpCBuffers[4].init(device, dContext);

	//this->m_gameObjects[4].initializeStatic(true, 3, 4);

	//vec = DirectX::XMVectorSet(-5.f, 1.5f, 0.f, 1.f);
	//this->m_gameObjects[4].setPosition(vec);
	//vec = DirectX::XMVectorSet(1.5f, 1.5f, 1.5f, 1.f);
	//this->m_gameObjects[4].setScale(vec);

	//this->m_gameObjects[4].setBoundingBox(DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f));
	//this->m_player.addAABB(this->m_gameObjects[4].getAABBPtr()); // add Bounding Box to player collidables

	//// Cube 2
	//this->m_wvpCBuffers.emplace_back();
	//this->m_wvpCBuffers[5].init(device, dContext);

	//this->m_gameObjects[5].initializeStatic(true, 3, 5);
	//
	//vec = DirectX::XMVectorSet(-5.f, 6.f, 6.f, 1.f);
	//this->m_gameObjects[5].setPosition(vec);
	//vec = DirectX::XMVectorSet(1.5f, 1.5f, 1.5f, 1.f);
	//this->m_gameObjects[5].setScale(vec);

	//this->m_gameObjects[5].setBoundingBox(DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f));
	//this->m_player.addAABB(this->m_gameObjects[5].getAABBPtr()); // add Bounding Box to player collidables

	// Sphere "HookHead"
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[2].loadVertexFromOBJ(device, dContext, L"Models/BirdHookModel.obj", mat, L"Textures/BirdTexture.png");

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[2].init(device, dContext);

	this->m_gameObjects[2].initializeDynamic(true, 2, 2, 1, DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10), &m_models[2]);

	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->m_gameObjects[2].setPosition(vec);
	vec = DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f);
	this->m_gameObjects[2].setScale(vec);

	this->m_gameObjects[2].setBoundingBox(DirectX::XMFLOAT3(.5f, .5f, .5f)); // Dont add to player

	// platform 1
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[3].loadVertexFromOBJ(device, dContext, L"Models/platform.obj", mat, L"Textures/platformTextureCracks1.png");

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[3].init(device, dContext);

	this->m_platforms[1].init(true, 3, 3, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(0.f, 10.f, 24.f, 1.f);
	this->m_platforms[1].setPosition(vec);

	this->m_platforms[1].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[1].getAABBPtr());

	// platform 2
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[4].init(device, dContext);

	this->m_platforms[2].init(true, 3, 4, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(-10.f, 20.f, 36.f, 1.f);
	this->m_platforms[2].setPosition(vec);

	this->m_platforms[2].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[2].getAABBPtr());

	// platform 3
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[5].init(device, dContext);

	this->m_platforms[3].init(true, 3, 5, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(10.f, 30.f, 48.f, 1.f);
	this->m_platforms[3].setPosition(vec);

	this->m_platforms[3].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[3].getAABBPtr());

	// platform 4
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[6].init(device, dContext);

	this->m_platforms[4].init(true, 3, 6, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(-40.f, 40.f, 60.f, 1.f);
	this->m_platforms[4].setPosition(vec);

	this->m_platforms[4].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[4].getAABBPtr());

	// platform 5
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[7].init(device, dContext);

	this->m_platforms[5].init(true, 3, 7, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(30.f, 50.f, 72.f, 1.f);
	this->m_platforms[5].setPosition(vec);

	this->m_platforms[5].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[5].getAABBPtr());

	// platform 6
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[8].init(device, dContext);

	this->m_platforms[6].init(true, 3, 8, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(-10.f, 60.f, 84.f, 1.f);
	this->m_platforms[6].setPosition(vec);

	this->m_platforms[6].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[6].getAABBPtr());

	// platform 7
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[9].init(device, dContext);

	this->m_platforms[7].init(true, 3, 9, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(20.f, 70.f, 96.f, 1.f);
	this->m_platforms[7].setPosition(vec);

	this->m_platforms[7].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[7].getAABBPtr());

	// platform 8
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[10].init(device, dContext);

	this->m_platforms[8].init(true, 3, 10, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(0.f, 80.f, 108.f, 1.f);
	this->m_platforms[8].setPosition(vec);

	this->m_platforms[8].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[8].getAABBPtr());

	// platform 9
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[11].init(device, dContext);

	this->m_platforms[9].init(true, 3, 11, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(-10.f, 90.f, 120.f, 1.f);
	this->m_platforms[9].setPosition(vec);

	this->m_platforms[9].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[9].getAABBPtr());

	// platform 10
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[12].init(device, dContext);

	this->m_platforms[10].init(true, 3, 12, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(10.f, 100.f, 132.f, 1.f);
	this->m_platforms[10].setPosition(vec);

	this->m_platforms[10].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[10].getAABBPtr());

	// platform 11
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[13].init(device, dContext);

	this->m_platforms[11].init(true, 3, 13, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(-10.f, 110.f, 144.f, 1.f);
	this->m_platforms[11].setPosition(vec);

	this->m_platforms[11].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[11].getAABBPtr());

	// platform 12 final
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[14].init(device, dContext);

	this->m_platforms[12].init(true, 3, 14, &this->m_pyramidOBB, &m_models[3]);

	vec = DirectX::XMVectorSet(0.f, 120.f, 170.f, 1.f);
	this->m_platforms[12].setPosition(vec);
	vec = DirectX::XMVectorSet(5.f, 1.f, 5.f, 1.f);
	this->m_platforms[12].setScale(vec);

	this->m_platforms[12].setBoundingBox(DirectX::XMFLOAT3(13.f, 0.5f, 13.f));
	this->m_player.addAABB(this->m_platforms[12].getAABBPtr());

	// HookHand
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	this->m_models[4].loadVertexFromOBJ(device, dContext, L"Models/HookModel.obj", mat, L"Textures/HookTexture.png");

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[15].init(device, dContext);

	this->m_gameObjects[3].initializeDynamic(false, 4, 15, 1, DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10), &m_models[4]);

	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->m_gameObjects[3].setPosition(vec);
	vec = DirectX::XMVectorSet(.7f, .7f, .7f, 1.f);
	this->m_gameObjects[3].setScale(vec);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[16].init(device, dContext);

	this->m_platforms[0].init(true, 3, 16, &this->m_pyramidOBB, &m_models[3]);
	vec = DirectX::XMVectorSet(10.f, 20, 36, 1.f);
	this->m_platforms[0].setPosition(vec);
	this->m_platforms[0].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_platforms[0].getAABBPtr());



	// Player
	this->m_player.initialize(-1, -1, 1.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), &this->m_gameObjects[2], &this->m_gameObjects[3], audioEngine);
	this->m_player.setPosition(DirectX::XMVectorSet(0.f, 5.f, -1.f, 1.f));
	
	for (size_t i = 0; i < this->m_platforms.size(); i++)
	{
		this->m_platforms[i].setPlayerBoundingBox(this->m_player.getAABBPtr());

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
		this->m_gameObjects[i].update(dt);

		// World View Projection Matrix Contruction
		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = this->m_camera.getViewMatrix() * this->m_camera.getProjectionMatrix();
		wvpData.wvp = this->m_gameObjects[i].getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = this->m_gameObjects[i].getWorldMatrix();

		this->m_wvpCBuffers[this->m_gameObjects[i].getWvpCBufferIndex()].upd(&wvpData);
	}

	//Platforms
	for (int i = 0; i < this->m_platforms.size(); i++)
	{
		this->m_platforms[i].update(dt);

		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = this->m_camera.getViewMatrix() * this->m_camera.getProjectionMatrix();
		wvpData.wvp = this->m_platforms[i].getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = this->m_platforms[i].getWorldMatrix();

		this->m_wvpCBuffers[this->m_platforms[i].getWvpCBufferIndex()].upd(&wvpData);
		if (m_platforms[i].m_removeMe)
		{
			//Remove
		}
	}
}