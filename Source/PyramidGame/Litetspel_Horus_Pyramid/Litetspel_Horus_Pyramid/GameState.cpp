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

std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* GameState::getWvpCBuffersPtr()
{
	return &this->m_wvpCBuffers;
}

void GameState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{
	// Material
	MaterialData mat;

	// Models
	this->m_gameObjects.resize(15);

	// Ground Object
	std::vector<Vertex> groundvertices =
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
	};
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(.8f, .8f, .8f, 1.f);
	//this->m_models[0].loadVertexVector(device, dContext, groundvertices, mat);
	this->m_models[0].loadVertexFromOBJ(device, dContext, L"Models/desertGround.obj", mat, L"Textures/sandTexture.png");

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[0].init(device, dContext);

	this->m_gameObjects[0].initializeStatic(true, 0, 0);

	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	this->m_gameObjects[0].setPosition(vec);
	/*this->m_gameObjects[0].setScale(vec);*/
	this->m_gameObjects[0].setBoundingBox(DirectX::XMFLOAT3(1000.f, 10.f, 1000.f));
	this->m_player.addAABB(this->m_gameObjects[0].getAABBPtr()); // add Bounding Box to player collidables

	// Pyramid
	//,	Model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.75164f, 0.60648f, 0.22648f, 1.0f);
	this->m_models[1].loadVertexFromOBJ(device, dContext, L"Models/BasePyr.obj", mat, L"Textures/pyramidTexture.png");

	//,	WVP Buffer
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[1].init(device, dContext);

	//,	Game Object
	this->m_gameObjects[1].initializeStatic(false, 1, 1);

	//,	Movement changes
	vec = DirectX::XMVectorSet(0.f, 0.f, 100.f, 1.f);
	this->m_gameObjects[1].setPosition(vec);

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
	mat.diffuse = DirectX::XMFLOAT4(0.1f, 0.2f, 1.f, 1.0f);
	this->m_models[2].loadVertexFromOBJ(device, dContext, L"Models/sphere.obj", mat, L"Textures/pyramidTexture.png");

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[2].init(device, dContext);

	this->m_gameObjects[2].initializeDynamic(true, 2, 2, 1, DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));

	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->m_gameObjects[2].setPosition(vec);
	vec = DirectX::XMVectorSet(.2f, .2f, .2f, 1.f);
	this->m_gameObjects[2].setScale(vec);

	this->m_gameObjects[2].setBoundingBox(DirectX::XMFLOAT3(.5f, .5f, .5f)); // Dont add to player

	// platform 1
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	this->m_models[3].loadVertexFromOBJ(device, dContext, L"Models/platform.obj", mat, L"Textures/platformTextureCracks1.png");

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[3].init(device, dContext);

	this->m_gameObjects[3].initializeStatic(true, 3, 3);

	vec = DirectX::XMVectorSet(0.f, 10.f, 24.f, 1.f);
	this->m_gameObjects[3].setPosition(vec);

	this->m_gameObjects[3].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[3].getAABBPtr());

	// platform 2
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[4].init(device, dContext);

	this->m_gameObjects[4].initializeStatic(true, 3, 4);

	vec = DirectX::XMVectorSet(-10.f, 20.f, 36.f, 1.f);
	this->m_gameObjects[4].setPosition(vec);

	this->m_gameObjects[4].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[4].getAABBPtr());

	// platform 3
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[5].init(device, dContext);

	this->m_gameObjects[5].initializeStatic(true, 3, 5);

	vec = DirectX::XMVectorSet(10.f, 30.f, 48.f, 1.f);
	this->m_gameObjects[5].setPosition(vec);

	this->m_gameObjects[5].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[5].getAABBPtr());

	// platform 4
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[6].init(device, dContext);

	this->m_gameObjects[6].initializeStatic(true, 3, 6);

	vec = DirectX::XMVectorSet(-40.f, 40.f, 60.f, 1.f);
	this->m_gameObjects[6].setPosition(vec);

	this->m_gameObjects[6].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[6].getAABBPtr());

	// platform 5
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[7].init(device, dContext);

	this->m_gameObjects[7].initializeStatic(true, 3, 7);

	vec = DirectX::XMVectorSet(30.f, 50.f, 72.f, 1.f);
	this->m_gameObjects[7].setPosition(vec);

	this->m_gameObjects[7].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[7].getAABBPtr());

	// platform 6
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[8].init(device, dContext);

	this->m_gameObjects[8].initializeStatic(true, 3, 8);

	vec = DirectX::XMVectorSet(-10.f, 60.f, 84.f, 1.f);
	this->m_gameObjects[8].setPosition(vec);

	this->m_gameObjects[8].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[8].getAABBPtr());

	// platform 7
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[9].init(device, dContext);

	this->m_gameObjects[9].initializeStatic(true, 3, 9);

	vec = DirectX::XMVectorSet(20.f, 70.f, 96.f, 1.f);
	this->m_gameObjects[9].setPosition(vec);

	this->m_gameObjects[9].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[9].getAABBPtr());

	// platform 8
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[10].init(device, dContext);

	this->m_gameObjects[10].initializeStatic(true, 3, 10);

	vec = DirectX::XMVectorSet(0.f, 80.f, 108.f, 1.f);
	this->m_gameObjects[10].setPosition(vec);

	this->m_gameObjects[10].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[10].getAABBPtr());

	// platform 9
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[11].init(device, dContext);

	this->m_gameObjects[11].initializeStatic(true, 3, 11);

	vec = DirectX::XMVectorSet(-10.f, 90.f, 120.f, 1.f);
	this->m_gameObjects[11].setPosition(vec);

	this->m_gameObjects[11].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[11].getAABBPtr());

	// platform 10
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[12].init(device, dContext);

	this->m_gameObjects[12].initializeStatic(true, 3, 12);

	vec = DirectX::XMVectorSet(10.f, 100.f, 132.f, 1.f);
	this->m_gameObjects[12].setPosition(vec);

	this->m_gameObjects[12].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[12].getAABBPtr());

	// platform 11
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[13].init(device, dContext);

	this->m_gameObjects[13].initializeStatic(true, 3, 13);

	vec = DirectX::XMVectorSet(-10.f, 110.f, 144.f, 1.f);
	this->m_gameObjects[13].setPosition(vec);

	this->m_gameObjects[13].setBoundingBox(DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	this->m_player.addAABB(this->m_gameObjects[13].getAABBPtr());

	// platform 12 final
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[14].init(device, dContext);

	this->m_gameObjects[14].initializeStatic(true, 3, 14);

	vec = DirectX::XMVectorSet(0.f, 120.f, 170.f, 1.f);
	this->m_gameObjects[14].setPosition(vec);
	vec = DirectX::XMVectorSet(5.f, 1.f, 5.f, 1.f);
	this->m_gameObjects[14].setScale(vec);

	this->m_gameObjects[14].setBoundingBox(DirectX::XMFLOAT3(13.f, 0.5f, 13.f));
	this->m_player.addAABB(this->m_gameObjects[14].getAABBPtr());

	// Player
	this->m_player.initialize(-1, -1, 1.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), &this->m_gameObjects[2], audioEngine);
	this->m_player.setPosition(DirectX::XMVectorSet(0.f, 5.f, -1.f, 1.f));

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
}