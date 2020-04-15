#include "pch.h"
#include "Player.h"

Player::Player()
{
	this->m_movementComp = new MovementComponent();
	this->m_physicsComp = new PhysicsComponent();
}

Player::~Player() {}

void Player::initialize(float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration)
{
	this->m_physicsComp->initialize(this->m_movementComp, mass, acceleration, deceleration);
}

void Player::update(Keyboard* keyboard, MouseEvent mouseEvent, float dt)
{
	if (keyboard->isKeyPressed('W'))
		this->m_physicsComp->addForceDir(Direction::FORWARD, dt);

	if (keyboard->isKeyPressed('S'))
		this->m_physicsComp->addForceDir(Direction::BACKWARD, dt);

	if (keyboard->isKeyPressed('A'))
		this->m_physicsComp->addForceDir(Direction::LEFT, dt);

	if (keyboard->isKeyPressed('D'))
		this->m_physicsComp->addForceDir(Direction::RIGHT, dt);

	if (keyboard->isKeyPressed(' '))
		this->m_physicsComp->addForceDir(Direction::UP, dt);

	if (keyboard->isKeyPressed((unsigned char)16))
		this->m_physicsComp->addForceDir(Direction::DOWN, dt);

	this->m_physicsComp->updatePosition(dt);
}

DirectX::XMVECTOR Player::getPosition() const
{
	return this->m_movementComp->position;
}

DirectX::XMMATRIX Player::getWorldMatrix() const
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixScalingFromVector(this->m_movementComp->scale) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_movementComp->rotation) *
		DirectX::XMMatrixTranslationFromVector(this->m_movementComp->position));
}

MovementComponent* Player::getMoveCompPtr()
{
	return this->m_movementComp;
}

void Player::setPosition(DirectX::XMVECTOR newPosition)
{
	if (this->m_movementComp)
		this->m_movementComp->position = newPosition;
}

void Player::move(Direction dir, float dt)
{
	this->m_physicsComp->addForceDir(dir, dt);
}