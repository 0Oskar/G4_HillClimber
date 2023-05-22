#include "pch.h"
#include "Player.h"

Player::Player() : GameObject()
{
	m_lastFly = false;
	m_QAmode = false;
	m_lastOnGroundYPos = -1.f;
	m_failThreshold = 10.f;
	m_spawning = false;
}

Player::~Player() {}

void Player::initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, GameObject* gObj, GameObject* hookGun, GameObject* hookGem, GameObject* hookHandLeftWing, GameObject* hookHandRightWing, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine, std::vector<DirectX::BoundingBox*> platformBB)
{
	m_isStatic = true;
	m_collidable = true;
	m_modelIndex = modelIndex;
	m_wvpCBufferIndex = wvpCBufferIndex;

	XMFLOAT3 boundingbox(1.f, 4.f, 1.f);
	m_movementComp = new MovementComponent();
	m_spawnPosition = XMVectorSet(0.f, boundingbox.y + 0.1f, 0.f, 1.f);
	m_physicsComp = new PhysicsComponent();
	m_physicsComp->initialize(m_movementComp, mass, acceleration, deceleration);
	m_physicsComp->setBoundingBox(m_movementComp->getPositionF3(), boundingbox);
	m_hookHand.init(gObj, m_movementComp, m_physicsComp, &m_collidableAABBoxes, hookGun, hookGem, hookHandLeftWing, hookHandRightWing, chainGObjects, audioEngine, platformBB);
}

void Player::updateHookHandBB(std::vector<DirectX::BoundingBox*> platformBB)
{
	m_hookHand.setBB(platformBB);
}

void Player::addAABB(DirectX::BoundingBox* aabb)
{
	m_collidableAABBoxes.push_back(aabb);
}
void Player::addAABBFromVector(std::vector<DirectX::BoundingBox>* aabb)
{
	for (int i = 0; i < aabb->size(); i++)
	{
		m_collidableAABBoxes.push_back(&aabb->at(i));
	}
}

void Player::clearAABB()
{
	m_collidableAABBoxes.clear();
}

void Player::addOrientedBBFromVector(std::vector<DirectX::BoundingOrientedBox>* obb)
{
	for (int i = 0; i < obb->size(); i++)
	{
		m_collidableOrientedBoxes.push_back(&obb->at(i));
	}
}

void Player::addPyramidOBB(DirectX::BoundingOrientedBox* obb)
{
	m_pyramidOBB = *obb;
}

void Player::setSpawnPosition(XMVECTOR position)
{
	m_spawnPosition = position + XMVectorSet(0.f, (*m_physicsComp->getAABBPtr()).Extents.y + 0.1f, 0, 0);
}

void Player::respawn()
{
	Transition::get().fadeIn({ 0.f, 0.f, 0.f });
	m_spawning = true;
}

void Player::respawnLogic()
{
	m_movementComp->position = m_spawnPosition;
	m_lastOnGroundYPos = XMVectorGetY(m_spawnPosition);
	m_movementComp->rotation = { 0.f, 0.f, 0.f, 1.f };
	m_physicsComp->setVelocity({ 0.f, 0.f, 0.f });
	m_physicsComp->setIsJumping(false);
	m_physicsComp->setIsFalling(true);
	m_lastFly = false;
	m_hookHand.reset();
	m_spawning = false;
}

bool Player::getinUse()
{
	return inUse;
}

void Player::resetVelocity()
{
	m_physicsComp->setVelocity(DirectX::XMFLOAT3());
}

void Player::flyDown(float speed)
{
	m_physicsComp->addForceDir(Direction::DOWN, speed);
}

bool Player::getQAMode() const
{
	return m_QAmode;
}

bool Player::getIsSpawning() const
{
	return m_spawning;
}

void Player::update(float dt)
{
	if (!m_spawning)
	{
		if (m_hookHand.shouldFly())
		{
			m_physicsComp->setVelocity(DirectX::XMFLOAT3(0, 0, 0));
			m_lastFly = true;
			DirectX::XMVECTOR normalziedAndDT = DirectX::XMVectorScale(DirectX::XMVector3Normalize(m_hookHand.m_toHeadDir), dt);
			normalziedAndDT = DirectX::XMVectorScale(normalziedAndDT, 100);
			m_movementComp->position = (DirectX::XMVectorAdd(m_movementComp->position, normalziedAndDT));
		}
		else
		{
			// Update lastGroundPos
			if (m_lastFly || !m_physicsComp->getIsFalling())
			{
				m_lastOnGroundYPos = XMVectorGetY(m_movementComp->position);
			}
			m_lastFly = false;

			// Fail State
			if (!m_QAmode && m_lastOnGroundYPos != -1 && m_lastOnGroundYPos > XMVectorGetY(m_movementComp->position) + m_failThreshold)
			{
				respawn();
			}

			// Handle Collisions
			if (m_QAmode) // Pyramid intersection turned off in QA Mode
			{
				BoundingOrientedBox QAPyramidOBB = m_pyramidOBB;
				QAPyramidOBB.Extents = XMFLOAT3(0.f, 0.f, 0.f);
				m_physicsComp->handleCollision(m_collidableAABBoxes, QAPyramidOBB, dt, m_collidableOrientedBoxes);
			}
			else
				m_physicsComp->handleCollision(m_collidableAABBoxes, m_pyramidOBB, dt, m_collidableOrientedBoxes);
		}

		m_physicsComp->updatePosition(dt, true); // If Camera is not following player, remove last argument( Only dt)

		// Gravity or Flying Deceleration
		if (m_QAmode)
			m_physicsComp->addYDecel(dt);
		else
			m_physicsComp->addGravity(dt);

		// Hook Update
		m_hookHand.update(dt);
	}
	else
	{
		if (Transition::get().isAnimationDone())
		{
			respawnLogic();
			Transition::get().fadeOut();
		}
	}
}

void Player::shoot()
{
	m_hookHand.fire();
}

bool Player::canMove()
{
	bool canMove = true;
	if (m_hookHand.shouldFly())
	{
		canMove = false;
	}
	
	return canMove;
}

void Player::jump(float dt)
{
	if(canMove())
		m_physicsComp->jump(2.f, dt);
}

void Player::setUse(bool isUsing)
{
	inUse = isUsing;
}

void Player::movePlayer(Direction dir, float dt)
{
	if (canMove())
		m_physicsComp->addForceDir(dir, dt);
}

void Player::setQAMode(bool qaMode)
{
	m_QAmode = qaMode;
}

void Player::retract()
{
	m_hookHand.retract();
}