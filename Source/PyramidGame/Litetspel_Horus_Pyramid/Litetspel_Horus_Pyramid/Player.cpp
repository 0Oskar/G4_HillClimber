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
	this->m_physicsComp->setBoundingBox(this->m_movementComp->getPositionF3(), DirectX::XMFLOAT3(1.f, 2.f, 1.f));
}

void Player::addAABB(DirectX::BoundingBox* aabb)
{
	this->m_collidableAABBoxes.push_back(aabb);
}

void Player::update(Keyboard* keyboard, MouseEvent mouseEvent, float dt)
{
	// Gravity
	this->m_physicsComp->addGravity(dt);

	// Controls
	if (keyboard->isKeyPressed('W'))
		this->m_physicsComp->addForceDir(Direction::FORWARD, dt);

	if (keyboard->isKeyPressed('S'))
		this->m_physicsComp->addForceDir(Direction::BACKWARD, dt);

	if (keyboard->isKeyPressed('A'))
		this->m_physicsComp->addForceDir(Direction::LEFT, dt);

	if (keyboard->isKeyPressed('D'))
		this->m_physicsComp->addForceDir(Direction::RIGHT, dt);

	if (keyboard->isKeyPressed(' '))
		this->m_physicsComp->addForceDir(Direction::UP, dt, 15.f);

	if (keyboard->isKeyPressed((unsigned char)16))
		this->m_physicsComp->addForceDir(Direction::DOWN, dt);

	// For Debugging purposes
	if (keyboard->isKeyPressed('R'))
	{
		this->m_movementComp->position = DirectX::XMVectorSet(0.f, 4.f, -1.f, 1.f);
		this->m_physicsComp->setVelocity(DirectX::XMFLOAT3()); // Reset Velocity
	}

	// Handle Collisions
	this->m_physicsComp->handleCollision(this->m_collidableAABBoxes, dt);

	// Update Position with Velocity
	this->m_physicsComp->updatePosition(dt);
}