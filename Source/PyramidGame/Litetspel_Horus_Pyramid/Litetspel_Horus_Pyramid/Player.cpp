#include "pch.h"
#include "Player.h"

Player::Player() : GameObject()
{
}

Player::~Player()
{
}

void Player::initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration)
{
	this->m_isStatic = true;
	this->m_collidable = true;
	this->m_modelIndex = modelIndex;
	this->m_wvpCBufferIndex = wvpCBufferIndex;

	this->m_movementComp = new MovementComponent();
	this->m_physicsComp = new PhysicsComponent();
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