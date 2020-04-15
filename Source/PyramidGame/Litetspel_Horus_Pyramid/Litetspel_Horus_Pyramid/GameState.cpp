#include "pch.h"
#include "GameState.h"

GameState::GameState() {}

GameState::~GameState() {}

DirectX::XMMATRIX* GameState::getViewMatrix() const
{
	return this->m_camera.getViewMatrix();
}

DirectX::XMMATRIX* GameState::getProjectionMatrix() const
{
	return this->m_camera.getProjectionMatrix();
}

void GameState::initlialize(GameOptions options)
{
	// Player
	this->m_player.initialize(1.f, DirectX::XMFLOAT3(.001f, .001f, .001f), DirectX::XMFLOAT3(.99f, .99f, .99f));
	this->m_player.setPosition(DirectX::XMVectorSet(0.f, 4.f, -1.f, 1.f));

	// Camera
	this->m_camera.followMoveComp(this->m_player.getMoveCompPtr());
	this->m_camera.initialize(
		0.2f,
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
}