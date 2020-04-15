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
	this->m_player.initialize();
	this->m_player.setPosition(DirectX::XMVectorSet(0.f, 0.f, -1.f, 1.f));

	// Camera
	this->m_camera.followMoveComp(this->m_player.getMoveCompPtr());
	this->m_camera.initialize(
		0.2f,
		0.2f,
		options.fov,
		(float)options.width / (float)options.height,
		0.1f,
		1000.f
	);
}

void GameState::update(Keyboard* keyboard, MouseEvent mouseEvent, float dt)
{
	// Move Camera
	if (keyboard->isKeyPressed('W'))
		this->m_camera.move(Direction::FORWARD, dt);

	if (keyboard->isKeyPressed('S'))
		this->m_camera.move(Direction::BACKWARD, dt);

	if (keyboard->isKeyPressed('A'))
		this->m_camera.move(Direction::LEFT, dt);

	if (keyboard->isKeyPressed('D'))
		this->m_camera.move(Direction::RIGHT, dt);

	if (keyboard->isKeyPressed(' '))
		this->m_camera.move(Direction::UP, dt);

	if (keyboard->isKeyPressed((unsigned char)16))
		this->m_camera.move(Direction::DOWN, dt);

	// Rotate Camera
	if (mouseEvent.getEvent() == Event::MouseRAW_MOVE)
	{
		DirectX::XMFLOAT2 mouseDelta = DirectX::XMFLOAT2((float)mouseEvent.getPosX() * dt, (float)mouseEvent.getPosY() * dt);
		this->m_camera.update(mouseDelta);
	}
}