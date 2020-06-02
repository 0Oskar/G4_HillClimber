#include"pch.h"
#include"HookHand.h"

HookHand::HookHand()
{
	this->m_hookState = hookState::idle;
	this->m_audioComponent = new AudioComponent();

	this->m_wingsOpen = false;
}

HookHand::~HookHand()
{
	if (this->m_audioComponent)
	{
		delete m_audioComponent;
	}
}

void HookHand::init(GameObject* gObject, MovementComponent* movementComponent, PhysicsComponent* physicsComponent, std::vector<DirectX::BoundingBox*>* boundingBoxes, GameObject* hookGun, GameObject* hookGem, GameObject* hookHandLeftWing, GameObject* hookHandRightWing, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine, std::vector<DirectX::BoundingBox*> platformBB)
{
	MaterialData mat;
	mat.diffuse = DirectX::XMFLOAT4(0.5, 0.5, 0.5, 1);
	this->m_hookGameObject = gObject;
	this->m_gunGameObject = hookGun;
	this->m_gemGameObject = hookGem;
	this->m_leftWingGameObject = hookHandLeftWing;
	this->m_rightWingGameObject = hookHandRightWing;
	this->m_chain.initialize(gObject, hookGun, chainGObjects);
	this->m_hookPhysicsComp = this->m_hookGameObject->getphysicsCompPtr();
	this->m_playerMovement = movementComponent;
	this->m_playerPhysics = physicsComponent;
	this->m_hookGameObject->setPosition(this->m_playerMovement->position);
	this->m_hookTimer.start();
	this->m_boundingBoxes = boundingBoxes;
	this->m_platformsBB = new std::vector<DirectX::BoundingBox*>(platformBB);
	
	this->m_audioComponent->init(audioEngine, movementComponent, 2);
	this->m_audioComponent->loadSound(m_fireSoundString);
	this->m_audioComponent->loadSound(m_chainExtendingSound);
}

void HookHand::setBB(std::vector<DirectX::BoundingBox*> platformBB)
{
	this->m_platformsBB = new std::vector<DirectX::BoundingBox*>(platformBB);
}

bool HookHand::canFire()
{
	return m_hookState == hookState::idle;
}
bool HookHand::canRecall()
{
	return m_hookState == hookState::shooting || m_hookState == hookState::flyYouFool;
}
void HookHand::fire()
{
	if (this->canFire())
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_playerMovement->rotation);

		DirectX::XMVECTOR forwardX = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		this->m_shootDirection = forwardX;
		this->m_hookState = hookState::shooting;

		this->m_audioComponent->playSound(this->m_fireSoundString, 0, false, 0.01f, 1);
		this->m_audioComponent->playSound(this->m_chainExtendingSound, 1, true);
		this->m_shouldPlayChainSound = true;
	}
}

void HookHand::retract()
{
	if (this->canRecall())
	{
		this->m_hookState = hookState::recalling;
		this->m_audioComponent->playSound(this->m_chainExtendingSound, 1, true);
		this->m_shouldPlayChainSound = true;
	}
}

void HookHand::reset()
{
	float hookSpeedForward = 100.f;
	this->hookSpeedRetract = 100.f;
	/*this->m_hookState = hookState::idle;
	this->m_hookGameObject->getMoveCompPtr()->rotation = this->m_playerMovement->rotation + this->hookRotOffsetConst;
	this->m_hookGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(this->m_gunGameObject->getMoveCompPtr()->position, hookPosOffset);*/
}

bool HookHand::colide()
{
	bool colided = false;
	for (int i = 0; i < m_platformsBB->size(); i++)
	{
		if (this->m_hookGameObject->getAABB().Intersects(*m_platformsBB->at(i)))
		{
			DirectX::XMVECTOR posToTopAndCenterOfBox = DirectX::XMVectorSet(DirectX::XMVectorGetX(this->m_hookGameObject->getPosition()), m_platformsBB->at(i)->Center.y, DirectX::XMVectorGetZ(this->m_hookGameObject->getPosition()), 0);
			posToTopAndCenterOfBox = DirectX::XMVectorAdd(posToTopAndCenterOfBox, DirectX::XMVectorSet(0, m_platformsBB->at(i)->Extents.y + (this->m_hookGameObject->getAABB().Extents.y * 2), 0, 0));  // Move in Y the platform extends then + the size of the boundingbox of the hookHead.
			m_platformCenter = DirectX::XMVectorAdd(posToTopAndCenterOfBox, DirectX::XMVectorScale(this->m_playerMovement->forward, 2));
			colided = true;
		}
	}
	return  colided;
}
void HookHand::updateHandModel(float dt)
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_playerMovement->rotation);
	DirectX::XMVECTOR rotOff = this->m_playerMovement->rotation;
	
	// Hook Hand/Gun
	DirectX::XMVECTOR gunPosOffsetRotated = XMVector3TransformCoord(this->gunPosOffset, rotationMatrix);
	this->m_gunGameObject->getMoveCompPtr()->localRotation = gunRotOffset;
	this->m_gunGameObject->getMoveCompPtr()->rotation = rotOff + gunRotOffset;
	this->m_gunGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(this->m_playerMovement->position, gunPosOffsetRotated);
	
	// Hook Gem
	this->m_gemGameObject->getMoveCompPtr()->rotation = this->m_gunGameObject->getMoveCompPtr()->rotation;
	this->m_gemGameObject->getMoveCompPtr()->position = this->m_gunGameObject->getMoveCompPtr()->position + XMVectorSet(0.f, 0.003f, 0.f, 0.f);

	// Wings
	// - Position
	DirectX::XMVECTOR left = XMVector3TransformCoord(this->leftWingPosOffset, rotationMatrix);
	this->m_leftWingGameObject->getMoveCompPtr()->position = this->m_gunGameObject->getMoveCompPtr()->position + left;

	DirectX::XMVECTOR right = XMVector3TransformCoord(this->rightWingPosOffset, rotationMatrix);
	this->m_rightWingGameObject->getMoveCompPtr()->position = this->m_gunGameObject->getMoveCompPtr()->position + right;

	// - Rotation
	XMVECTOR gunRotation = this->m_gunGameObject->getMoveCompPtr()->rotation;
	this->m_leftWingGameObject->getMoveCompPtr()->rotation = gunRotation;
	this->m_rightWingGameObject->getMoveCompPtr()->rotation = gunRotation;

	// -- Extend Wing logic
	float finalLeftRotationY = XMVectorGetY(this->leftWingRotOffset);
	float currentLeftRotationY = XMVectorGetY(this->m_leftWingGameObject->getMoveCompPtr()->localRotation);
	
	float finalRightRotationY = XMVectorGetY(this->rightWingRotOffset);
	float currentRightRotationY = XMVectorGetY(this->m_rightWingGameObject->getMoveCompPtr()->localRotation);

	if (this->m_wingsOpen)
	{
		float rotationDifferenceY = finalLeftRotationY - currentLeftRotationY;

		if (rotationDifferenceY <= 0.f) // No animation needed
		{
			this->m_leftWingGameObject->getMoveCompPtr()->localRotation = this->leftWingRotOffset;
			this->m_rightWingGameObject->getMoveCompPtr()->localRotation = this->rightWingRotOffset;
		}
		else // Needs to animate
		{
			this->m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentLeftRotationY + (dt * this->wingAnimationSpeed), 0.f, 0.f);
			this->m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentRightRotationY - (dt * this->wingAnimationSpeed), 0.f, 0.f);
			if (finalLeftRotationY <= XMVectorGetY(this->m_leftWingGameObject->getMoveCompPtr()->localRotation))
			{
				this->m_leftWingGameObject->getMoveCompPtr()->localRotation = this->leftWingRotOffset;
				this->m_rightWingGameObject->getMoveCompPtr()->localRotation = this->rightWingRotOffset;
			}
		}
	}
	else
	{
		float rotationDifferenceY = 0.f - currentLeftRotationY;

		if (rotationDifferenceY >= 0.f) // No animation needed
		{
			this->m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
			this->m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
		}
		else // Needs to animate
		{
			this->m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentLeftRotationY - (dt * this->wingAnimationSpeed), 0.f, 0.f);
			this->m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentRightRotationY + (dt * this->wingAnimationSpeed), 0.f, 0.f);
			if (0.f >= XMVectorGetY(this->m_leftWingGameObject->getMoveCompPtr()->localRotation))
			{
				this->m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
				this->m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
			}
		}
	}

	this->hookPosOffset = XMVector3TransformCoord(this->hookPosOffsetConst, rotationMatrix);
}

DirectX::XMVECTOR HookHand::invertX(DirectX::XMVECTOR VectorToInvertX)
{
	DirectX::XMVECTOR returnVector;
	DirectX::XMFLOAT4 rot;
	DirectX::XMStoreFloat4(&rot, VectorToInvertX);
	rot.x = -rot.x;
	
	returnVector = DirectX::XMLoadFloat4(&rot);
	return returnVector;
}

void HookHand::update(float dt)
{
	this->updateHandModel(dt);
	if (m_hookState == hookState::shooting)
	{
		if (this->colide())
		{
			this->m_hookState = hookState::hit;
		}
		else
			//this->m_hookGameObject->getMoveCompPtr()->move(DirectX::XMVectorScale(this->m_shootDirection, dt * this->hookSpeedForward));
			this->m_hookGameObject->move(DirectX::XMVectorScale(this->m_shootDirection, 1.f/*this->hookSpeedForward*/), dt);

		DirectX::XMVECTOR fromPlayerToHook = DirectX::XMVectorSubtract(this->m_hookGameObject->getPosition(), this->m_playerMovement->position);
		if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthEst(fromPlayerToHook), 1) >= maxDistance)
		{
			this->m_hookState = hookState::recalling;
			this->m_hookPhysicsComp->setVelocity(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
		}

		// Wing rotation
		this->m_wingsOpen = true;
	}
	else if (m_hookState == hookState::recalling)
	{
		this->m_origin = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(this->m_gunGameObject->getMoveCompPtr()->position, hookPosOffset), this->m_hookGameObject->getPosition());
		this->m_hookPhysicsComp->setVelocity(DirectX::XMFLOAT3(0.f, 0.f, 0.f));

		if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthEst(this->m_origin), 1) <= 2)
		{
			this->m_hookState = hookState::waiting;
			this->m_hookTimer.restart();
		}
		else
		{
			this->m_hookGameObject->getMoveCompPtr()->move(DirectX::XMVectorScale(DirectX::XMVector3Normalize(this->m_origin), dt * this->hookSpeedRetract));
		}
	}
	else if (m_hookState == hookState::hit)
	{
		this->m_toHeadDir = DirectX::XMVectorSubtract(this->m_hookGameObject->getPosition(), this->m_playerMovement->position);
		this->m_hookState = hookState::flyYouFool;
		this->m_hookGameObject->getphysicsCompPtr()->setVelocity({0.f, 0.f , 0.f });
		this->m_audioComponent->playSound(this->m_chainExtendingSound, 1, true);
		this->m_shouldPlayChainSound = true;
	}
	else if (m_hookState == hookState::flyYouFool)
	{
		//DirectX::XMVECTOR addPosVector = DirectX::XMVectorAdd(DirectX::XMVectorSet(0, 5, 0, 0), DirectX::XMVectorDivide(this->m_playerMovement->forward, DirectX::XMVectorSet(2, 2, 2, 0)));
		DirectX::XMVECTOR addPosVector = DirectX::XMVectorSet(0, 4, 0, 0);

		this->m_toHeadDir = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(this->m_platformCenter, addPosVector), this->m_playerMovement->position);
		if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthEst(this->m_toHeadDir), 1) <= 1)
		{
			this->m_hookState = hookState::idle;
		}
	}
	else
	{
		this->m_audioComponent->stopSound(1);
		if (this->m_hookState == hookState::waiting)
		{
			if (this->m_hookTimer.timeElapsed() >= this->hookDelayTime)
			{
				this->m_hookState = hookState::idle;
			}
		}
		this->m_hookGameObject->getMoveCompPtr()->rotation = this->m_playerMovement->rotation + this->hookRotOffsetConst;
		this->m_hookGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(this->m_gunGameObject->getMoveCompPtr()->position, hookPosOffset);

		// Wing rotation
		this->m_wingsOpen = false;
		// Reset speed for when this->reset() is used
		this->hookSpeedForward = 40.f;
		this->hookSpeedRetract = 40.f;
	}

	this->m_chain.update(dt);
}

bool HookHand::shouldFly()
{
	bool shouldFly = false;
	if (this->m_hookState == hookState::flyYouFool)
	{
		shouldFly = true;
	}
	return shouldFly;
}