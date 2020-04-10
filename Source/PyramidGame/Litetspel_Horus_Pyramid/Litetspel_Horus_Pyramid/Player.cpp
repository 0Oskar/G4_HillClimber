#include "pch.h"
#include "Player.h"

Player::Player()
{
	this->m_speed = 1.f;
	this->m_position = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
	this->m_rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	this->m_forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	this->m_left = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
	this->m_right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
	this->m_backward = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
}

Player::~Player() {}

DirectX::XMVECTOR Player::getPosition() const
{
	return this->m_position;
}

void Player::setPosition(DirectX::XMVECTOR newPosition)
{
	this->m_position = newPosition;
}

void Player::move(DirectX::XMVECTOR moveDistance)
{
	this->m_position = DirectX::XMVectorAdd(moveDistance, this->m_position);
}
