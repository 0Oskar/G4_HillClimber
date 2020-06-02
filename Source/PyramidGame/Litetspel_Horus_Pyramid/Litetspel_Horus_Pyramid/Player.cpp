#include "pch.h"
#include "Player.h"

Player::Player() : GameObject()
{
	this->m_lastFly = false;
	this->m_QAmode = false;
	this->m_lastOnGroundYPos = -1.f;
	this->m_failThreshold = 10.f;
}

Player::~Player() {}

void Player::initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, GameObject* gObj, GameObject* hookGun, GameObject* hookGem, GameObject* hookHandLeftWing, GameObject* hookHandRightWing, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine, std::vector<DirectX::BoundingBox*> platformBB)
{
	this->m_isStatic = true;
	this->m_collidable = true;
	this->m_modelIndex = modelIndex;
	this->m_wvpCBufferIndex = wvpCBufferIndex;

	XMFLOAT3 boundingbox(1.f, 4.f, 1.f);
	this->m_movementComp = new MovementComponent();
	this->m_spawnPosition = XMVectorSet(0.f, boundingbox.y, 0.f, 1.f);
	this->m_physicsComp = new PhysicsComponent();
	this->m_physicsComp->initialize(this->m_movementComp, mass, acceleration, deceleration);
	this->m_physicsComp->setBoundingBox(this->m_movementComp->getPositionF3(), boundingbox);
	this->m_hookHand.init(gObj, m_movementComp, m_physicsComp, &m_collidableAABBoxes, hookGun, hookGem, hookHandLeftWing, hookHandRightWing, chainGObjects, audioEngine, platformBB);
}

void Player::updateHookHandBB(std::vector<DirectX::BoundingBox*> platformBB)
{
	this->m_hookHand.setBB(platformBB);
}

void Player::addAABB(DirectX::BoundingBox* aabb)
{
	this->m_collidableAABBoxes.push_back(aabb);
}
void Player::addAABBFromVector(std::vector<DirectX::BoundingBox>* aabb)
{
	for (int i = 0; i < aabb->size(); i++)
	{
		this->m_collidableAABBoxes.push_back(&aabb->at(i));
	}
}

void Player::clearAABB()
{
	this->m_collidableAABBoxes.clear();
}

void Player::addOrientedBBFromVector(std::vector<DirectX::BoundingOrientedBox>* obb)
{
	for (int i = 0; i < obb->size(); i++)
	{
		this->m_collidableOrientedBoxes.push_back(&obb->at(i));
	}
}

void Player::addPyramidOBB(DirectX::BoundingOrientedBox* obb)
{
	this->m_pyramidOBB = *obb;
}

void Player::setSpawnPosition(XMVECTOR position)
{
	this->m_spawnPosition = position + XMVectorSet(0.f, this->getAABB().Extents.y, 0.f, 0.f);
}

void Player::respawn()
{
	this->m_movementComp->position = this->m_spawnPosition;
	this->m_lastOnGroundYPos = XMVectorGetY(this->m_spawnPosition);
	this->m_movementComp->rotation = { 0.f, 0.f, 0.f, 1.f };
	this->m_physicsComp->setVelocity({ 0.f, 0.f, 0.f });
	this->m_physicsComp->setIsJumping(false);
	this->m_physicsComp->setIsFalling(true);
	this->m_lastFly = false;
}

bool Player::getinUse()
{
	return this->inUse;
}

void Player::resetVelocity()
{
	this->m_physicsComp->setVelocity(DirectX::XMFLOAT3());
}

void Player::flyDown(float speed)
{
	this->m_physicsComp->addForceDir(Direction::DOWN, speed);
}

void Player::update(float dt)
{
	if (this->m_hookHand.shouldFly())
	{
		this->m_physicsComp->setVelocity(DirectX::XMFLOAT3(0, 0, 0));
		this->m_lastFly = true;
		DirectX::XMVECTOR normalziedAndDT = DirectX::XMVectorScale(DirectX::XMVector3Normalize(this->m_hookHand.m_toHeadDir), dt);
		normalziedAndDT = DirectX::XMVectorScale(normalziedAndDT, 100);
		this->m_movementComp->position = (DirectX::XMVectorAdd(this->m_movementComp->position, normalziedAndDT));
	}
	else
	{
		// Gravity

		if (!this->m_QAmode)
			this->m_physicsComp->addGravity(dt);

		// Update lastGroundPos
		if (this->m_lastFly || !this->m_physicsComp->getIsFalling())
		{
			this->m_lastOnGroundYPos = XMVectorGetY(this->m_movementComp->position);
			//OutputDebugString(L"lastOnGroundYPos updated!\n");
		}
		this->m_lastFly = false;

		// Fail State
		if (!this->m_QAmode && this->m_lastOnGroundYPos != -1 && this->m_lastOnGroundYPos > XMVectorGetY(this->m_movementComp->position) + this->m_failThreshold)
		{
			//this->respawn();
		}

		// Handle Collisions
		this->m_physicsComp->handleCollision(this->m_collidableAABBoxes, this->m_pyramidOBB, dt, this->m_collidableOrientedBoxes);
	}

	this->m_physicsComp->updatePosition(dt, true); // If Camera is not following player, remove last argument( Only dt)
	if (this->m_QAmode)
		this->m_physicsComp->addYDecel(dt);
	this->m_hookHand.update(dt);
}

void Player::shoot()
{
	this->m_hookHand.fire();
}

bool Player::canMove()
{
	bool canMove = true;
	if (this->m_hookHand.shouldFly())
	{
		canMove = false;
	}
	
	return canMove;
}

void Player::jump(float dt)
{
	if(this->canMove())
		this->m_physicsComp->jump(3.f, dt);
}

void Player::setUse(bool isUsing)
{
	this->inUse = isUsing;
}

void Player::movePlayer(Direction dir, float dt)
{
	if (this->canMove())
		this->m_physicsComp->addForceDir(dir, dt);
}

void Player::setQAMode(bool qaMode)
{
	this->m_QAmode = qaMode;
}

void Player::retract()
{
	this->m_hookHand.retract();
}