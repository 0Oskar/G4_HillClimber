#pragma once

#include "Player.h"
#include "Camera.h"
#include "Model.h"

class GameState
{
private:
	Player m_player;
	Camera m_camera;

	std::vector<Model> m_models;
	std::vector<GameObject> m_gameObjects;
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>> m_wvpCBuffers;

public:
	GameState();
	~GameState();

	// Getters
	DirectX::XMMATRIX* getViewMatrix() const;
	DirectX::XMMATRIX* getProjectionMatrix() const;
	std::vector<Model>* getModelsPtr();
	std::vector<GameObject>* getGameObjectsPtr();
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr();

	// Initialization
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options);

	// Update
	void update(Keyboard* keyboard, MouseEvent mouseEvent, Mouse* mousePointer, float dt);
};