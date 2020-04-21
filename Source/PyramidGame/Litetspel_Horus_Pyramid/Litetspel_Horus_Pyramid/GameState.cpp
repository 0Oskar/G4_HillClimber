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

void GameState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options)
{
	// Material
	MaterialData mat;

	// Models
	this->m_gameObjects.resize(11);
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
	mat.diffuse = DirectX::XMFLOAT4(0.7038f, 0.27048f, 0.0828f, 1.0f);
	this->m_models[0].loadVertexVector(device, dContext, groundvertices, mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[0].init(device, dContext);

	this->m_gameObjects[0].initializeStatic(true, 0, 0);

	DirectX::XMVECTOR vec = DirectX::XMVectorSet(100.f, 100.f, 100.f, 1.f);
	this->m_gameObjects[0].setScale(vec);
	this->m_gameObjects[0].setBoundingBox(DirectX::XMFLOAT3(100.f, 0.1f, 100.f));
	this->m_player.addAABB(this->m_gameObjects[0].getAABBPtr()); // add Bounding Box to player collidables

	// Quads
	//,	Model
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.61424f, 0.04136f, 0.04136f, 1.0f);
	this->m_models[1].initModel(device, dContext, mat);

	// 1.
	//,	WVP Buffer
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[1].init(device, dContext);

	//,	Game Object
	this->m_gameObjects[1].initializeStatic(false, 1, 1);

	//,	Movement changes
	vec = DirectX::XMVectorSet(0.f, 3.f, 0.f, 1.f);
	this->m_gameObjects[1].setPosition(vec);

	// 2.
	//,	WVP Buffer
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[2].init(device, dContext);

	//,	Game Object
	this->m_gameObjects[2].initializeStatic(false, 1, 2); // Same Model

	//,	Movement changes
	vec = DirectX::XMVectorSet(0.f, 2.f, 0.1f, 1.f);
	this->m_gameObjects[2].setPosition(vec);


	// Pyramid
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.75164f, 0.60648f, 0.22648f, 1.0f);
	this->m_models[2].loadVertexFromOBJ(device, dContext, L"Models/BasePyr.obj", mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[3].init(device, dContext);
	
	this->m_gameObjects[3].initializeStatic(false, 2, 3);

	vec = DirectX::XMVectorSet(0.f, 0.f, 100.f, 1.f);
	this->m_gameObjects[3].setPosition(vec);

	// Cube 1
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.396f, 0.74151f, 0.69102f, 1.0f);
	this->m_models[3].loadVertexFromOBJ(device, dContext, L"Models/cube.obj", mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[4].init(device, dContext);

	this->m_gameObjects[4].initializeStatic(true, 3, 4);

	vec = DirectX::XMVectorSet(-5.f, 1.5f, 0.f, 1.f);
	this->m_gameObjects[4].setPosition(vec);
	vec = DirectX::XMVectorSet(1.5f, 1.5f, 1.5f, 1.f);
	this->m_gameObjects[4].setScale(vec);

	this->m_gameObjects[4].setBoundingBox(DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f));
	this->m_player.addAABB(this->m_gameObjects[4].getAABBPtr()); // add Bounding Box to player collidables

	// Cube 2
	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[5].init(device, dContext);

	this->m_gameObjects[5].initializeStatic(true, 3, 5);
	
	vec = DirectX::XMVectorSet(-5.f, 6.f, 6.f, 1.f);
	this->m_gameObjects[5].setPosition(vec);
	vec = DirectX::XMVectorSet(1.5f, 1.5f, 1.5f, 1.f);
	this->m_gameObjects[5].setScale(vec);

	this->m_gameObjects[5].setBoundingBox(DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f));
	this->m_player.addAABB(this->m_gameObjects[5].getAABBPtr()); // add Bounding Box to player collidables

	// TreeV2
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.1f, 0.74151f, 0.1f, 1.0f);
	this->m_models[4].loadVertexFromOBJ(device, dContext, L"Models/TreeV2.obj", mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[6].init(device, dContext);

	this->m_gameObjects[6].initializeStatic(false, 4, 6);

	vec = DirectX::XMVectorSet(20.f, 0.f, 0.f, 1.f);
	this->m_gameObjects[6].setPosition(vec);

	// LillyPad
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	this->m_models[5].loadVertexFromOBJ(device, dContext, L"Models/LillyPad.obj", mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[7].init(device, dContext);

	this->m_gameObjects[7].initializeStatic(false, 5, 7);

	vec = DirectX::XMVectorSet(10.f, 1.f, -10.f, 1.f);
	this->m_gameObjects[7].setPosition(vec);

	// Cactus
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.4f, 0.5f, 0.4f, 1.0f);
	this->m_models[6].loadVertexFromOBJ(device, dContext, L"Models/Cactus.obj", mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[8].init(device, dContext);

	this->m_gameObjects[8].initializeStatic(false, 6, 8);

	vec = DirectX::XMVectorSet(0.f, 1.f, -20.f, 1.f);
	this->m_gameObjects[8].setPosition(vec);
	/*vec = DirectX::XMVectorSet(2.f, 2.f, 2.f, 1.f);
	this->m_gameObjects[8].setScale(vec);*/

	// Teapot
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	this->m_models[7].loadVertexFromOBJ(device, dContext, L"Models/teapot.obj", mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[9].init(device, dContext);

	this->m_gameObjects[9].initializeStatic(false, 7, 9);

	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->m_gameObjects[9].setPosition(vec);
	vec = DirectX::XMVectorSet(.5f, .5f, .5f, 1.f);
	this->m_gameObjects[9].setScale(vec);

	//Sphere "HookHead"
	this->m_models.emplace_back();
	mat.diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	this->m_models[8].loadVertexFromOBJ(device, dContext, L"Models/sphere.obj", mat);

	this->m_wvpCBuffers.emplace_back();
	this->m_wvpCBuffers[10].init(device, dContext);

	this->m_gameObjects[10].initializeDynamic(true, 8, 10, 1, DirectX::XMFLOAT3(10, 10, 10), DirectX::XMFLOAT3(10, 10, 10));

	vec = DirectX::XMVectorSet(10.f, 1.f, -20.f, 1.f);
	this->m_gameObjects[10].setPosition(vec);
	vec = DirectX::XMVectorSet(.2f, .2f, .2f, 1.f);
	this->m_gameObjects[10].setScale(vec);

	// Player
	this->m_player.initialize(-1, -1, 1.f, DirectX::XMFLOAT3(20.f, 20.f, 20.f), DirectX::XMFLOAT3(.01f, .01f, .01f), &this->m_gameObjects[10]);
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

void GameState::update(Keyboard* keyboard, MouseEvent mouseEvent, float dt)
{
	// Player
	this->m_player.update(keyboard, mouseEvent, dt);

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