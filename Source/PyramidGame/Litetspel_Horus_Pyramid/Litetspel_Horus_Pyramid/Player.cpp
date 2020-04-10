#include "pch.h"
#include "Player.h"

Player::Player()
{
	this->m_movementComp = nullptr;
}

Player::~Player() {}

void Player::initialize()
{
	this->m_movementComp = new MovementComponent();
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