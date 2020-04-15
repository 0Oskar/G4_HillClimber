#pragma once

#include "Camera.h"
#include "Player.h"

class GameState
{
private:
	Player m_player;
	Camera m_camera;

public:
	GameState();
	~GameState();

	// Getters
	DirectX::XMMATRIX* getViewMatrix() const;
	DirectX::XMMATRIX* getProjectionMatrix() const;

	// Initialization
	void initlialize(GameOptions options);

	// Update
	void update(Keyboard* keyboard, MouseEvent mouseEvent, float dt);
};