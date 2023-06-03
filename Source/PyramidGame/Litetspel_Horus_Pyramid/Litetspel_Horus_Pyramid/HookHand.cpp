#include"pch.h"
#include"HookHand.h"

HookHand::HookHand()
{
	m_hookState = hookState::idle;
	m_audioComponent = new AudioComponent();

	m_wingsOpen = false;
}

HookHand::~HookHand()
{
	if (m_audioComponent)
	{
		delete m_audioComponent;
		m_audioComponent = nullptr;
	}
}

void HookHand::init(GameObject* gObject, MovementComponent* movementComponent, PhysicsComponent* physicsComponent, std::vector<DirectX::BoundingBox*>* boundingBoxes, GameObject* hookGun, GameObject* hookGem, GameObject* hookHandLeftWing, GameObject* hookHandRightWing, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine, std::vector<DirectX::BoundingBox*> platformBB)
{
	MaterialData mat;
	mat.diffuse = DirectX::XMFLOAT4(0.5, 0.5, 0.5, 1);
	m_hookGameObject = gObject;
	m_gunGameObject = hookGun;
	m_gemGameObject = hookGem;
	m_gemGameObject->setVisibility(false); // Is Green
	m_leftWingGameObject = hookHandLeftWing;
	m_rightWingGameObject = hookHandRightWing;
	m_chain.initialize(gObject, hookGun, chainGObjects);
	m_hookPhysicsComp = m_hookGameObject->getphysicsCompPtr();
	m_playerMovement = movementComponent;
	m_playerPhysics = physicsComponent;
	m_hookGameObject->setPosition(m_playerMovement->position);
	m_hookTimer.start();
	m_boundingBoxes = boundingBoxes;
	m_platformsBB = new std::vector<DirectX::BoundingBox*>(platformBB);
	
	m_audioComponent->init(audioEngine, movementComponent, 2);
	m_audioComponent->loadSound(m_fireSoundString);
	m_audioComponent->loadSound(m_chainExtendingSound);
}

void HookHand::setBB(std::vector<DirectX::BoundingBox*> platformBB)
{
	m_platformsBB = new std::vector<DirectX::BoundingBox*>(platformBB);
}

bool HookHand::canFire() const
{
	return m_hookState == hookState::idle;
}
bool HookHand::canRecall() const
{
	return m_hookState == hookState::shooting || m_hookState == hookState::flyYouFool;
}
void HookHand::fire()
{
	if (canFire())
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(m_playerMovement->rotation);

		DirectX::XMVECTOR forwardX = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		m_shootDirection = forwardX;
		m_hookState = hookState::shooting;

		m_audioComponent->playSound(m_fireSoundString, 0, false, 0.01f, 1);
		m_audioComponent->playSound(m_chainExtendingSound, 1, true);
		m_shouldPlayChainSound = true;
	}
}

void HookHand::retract()
{
	if (canRecall())
	{
		m_hookState = hookState::recalling;
		m_audioComponent->playSound(m_chainExtendingSound, 1, true);
		m_shouldPlayChainSound = true;
	}
}

void HookHand::reset()
{
	float hookSpeedForward = 100.f;
	hookSpeedRetract = 100.f;
	/*m_hookState = hookState::idle;
	m_hookGameObject->getMoveCompPtr()->rotation = m_playerMovement->rotation + hookRotOffsetConst;
	m_hookGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(m_gunGameObject->getMoveCompPtr()->position, hookPosOffset);*/
}

bool HookHand::colide()
{
	bool colided = false;
	for (size_t i = 0; i < m_platformsBB->size(); i++)
	{
		if (m_hookGameObject->getAABB().Intersects(*m_platformsBB->at(i)))
		{
			DirectX::XMVECTOR posToTopAndCenterOfBox = DirectX::XMVectorSet(DirectX::XMVectorGetX(m_hookGameObject->getPosition()), m_platformsBB->at(i)->Center.y, DirectX::XMVectorGetZ(m_hookGameObject->getPosition()), 0);
			posToTopAndCenterOfBox = DirectX::XMVectorAdd(posToTopAndCenterOfBox, DirectX::XMVectorSet(0, m_platformsBB->at(i)->Extents.y + (m_hookGameObject->getAABB().Extents.y * 2), 0, 0));  // Move in Y the platform extends then + the size of the boundingbox of the hookHead.
			m_platformCenter = DirectX::XMVectorAdd(posToTopAndCenterOfBox, DirectX::XMVectorScale(m_playerMovement->forward, 2));
			colided = true;
		}
	}
	return  colided;
}
void HookHand::updateHandModel(float dt)
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(m_playerMovement->rotation);
	DirectX::XMVECTOR rotOff = m_playerMovement->rotation;
	
	// Hook Hand/Gun
	DirectX::XMVECTOR gunPosOffsetRotated = XMVector3TransformCoord(gunPosOffset, rotationMatrix);
	m_gunGameObject->getMoveCompPtr()->localRotation = gunRotOffset;
	m_gunGameObject->getMoveCompPtr()->rotation = rotOff + gunRotOffset;
	m_gunGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(m_playerMovement->position, gunPosOffsetRotated);
	
	// Hook Gem
	m_gemGameObject->getMoveCompPtr()->rotation = m_gunGameObject->getMoveCompPtr()->rotation;
	m_gemGameObject->getMoveCompPtr()->position = m_gunGameObject->getMoveCompPtr()->position + XMVectorSet(0.f, 0.003f, 0.f, 0.f);

	// Wings
	// - Position
	DirectX::XMVECTOR left = XMVector3TransformCoord(leftWingPosOffset, rotationMatrix);
	m_leftWingGameObject->getMoveCompPtr()->position = m_gunGameObject->getMoveCompPtr()->position + left;

	DirectX::XMVECTOR right = XMVector3TransformCoord(rightWingPosOffset, rotationMatrix);
	m_rightWingGameObject->getMoveCompPtr()->position = m_gunGameObject->getMoveCompPtr()->position + right;

	// - Rotation
	XMVECTOR gunRotation = m_gunGameObject->getMoveCompPtr()->rotation;
	m_leftWingGameObject->getMoveCompPtr()->rotation = gunRotation;
	m_rightWingGameObject->getMoveCompPtr()->rotation = gunRotation;

	// -- Extend Wing logic
	float finalLeftRotationY = XMVectorGetY(leftWingRotOffset);
	float currentLeftRotationY = XMVectorGetY(m_leftWingGameObject->getMoveCompPtr()->localRotation);
	
	float finalRightRotationY = XMVectorGetY(rightWingRotOffset);
	float currentRightRotationY = XMVectorGetY(m_rightWingGameObject->getMoveCompPtr()->localRotation);

	if (m_wingsOpen)
	{
		float rotationDifferenceY = finalLeftRotationY - currentLeftRotationY;

		if (rotationDifferenceY <= 0.f) // No animation needed
		{
			m_leftWingGameObject->getMoveCompPtr()->localRotation = leftWingRotOffset;
			m_rightWingGameObject->getMoveCompPtr()->localRotation = rightWingRotOffset;
		}
		else // Needs to animate
		{
			m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentLeftRotationY + (dt * wingAnimationSpeed), 0.f, 0.f);
			m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentRightRotationY - (dt * wingAnimationSpeed), 0.f, 0.f);
			if (finalLeftRotationY <= XMVectorGetY(m_leftWingGameObject->getMoveCompPtr()->localRotation))
			{
				m_leftWingGameObject->getMoveCompPtr()->localRotation = leftWingRotOffset;
				m_rightWingGameObject->getMoveCompPtr()->localRotation = rightWingRotOffset;
			}
		}
	}
	else
	{
		float rotationDifferenceY = 0.f - currentLeftRotationY;

		if (rotationDifferenceY >= 0.f) // No animation needed
		{
			m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
			m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
		}
		else // Needs to animate
		{
			m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentLeftRotationY - (dt * wingAnimationSpeed), 0.f, 0.f);
			m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorSet(0.f, currentRightRotationY + (dt * wingAnimationSpeed), 0.f, 0.f);
			if (0.f >= XMVectorGetY(m_leftWingGameObject->getMoveCompPtr()->localRotation))
			{
				m_leftWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
				m_rightWingGameObject->getMoveCompPtr()->localRotation = XMVectorZero();
			}
		}
	}

	hookPosOffset = XMVector3TransformCoord(hookPosOffsetConst, rotationMatrix);
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

void HookHand::update(const float dt)
{
	updateHandModel(dt);

	if (m_hookState == hookState::shooting)
	{
		if (colide())
		{
			m_hookState = hookState::hit;
		}
		else
			//m_hookGameObject->getMoveCompPtr()->move(DirectX::XMVectorScale(m_shootDirection, dt * hookSpeedForward));
			m_hookGameObject->move(DirectX::XMVectorScale(m_shootDirection, 1.f/*hookSpeedForward*/), dt);

		DirectX::XMVECTOR fromPlayerToHook = DirectX::XMVectorSubtract(m_hookGameObject->getPosition(), m_playerMovement->position);
		if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthEst(fromPlayerToHook), 1) >= maxDistance)
		{
			m_hookState = hookState::recalling;
			m_hookPhysicsComp->setVelocity(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
		}

		// Wing rotation
		m_wingsOpen = true;
	}
	else if (m_hookState == hookState::recalling)
	{
		m_origin = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(m_gunGameObject->getMoveCompPtr()->position, hookPosOffset), m_hookGameObject->getPosition());
		m_hookPhysicsComp->setVelocity(DirectX::XMFLOAT3(0.f, 0.f, 0.f));

		if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthEst(m_origin), 1) <= 2)
		{
			m_hookState = hookState::waiting;
			m_hookTimer.restart();
		}
		else
		{
			m_hookGameObject->getMoveCompPtr()->move(DirectX::XMVectorScale(DirectX::XMVector3Normalize(m_origin), dt * hookSpeedRetract));
		}
	}
	else if (m_hookState == hookState::hit)
	{
		m_gemGameObject->setVisibility(true);
		m_toHeadDir = DirectX::XMVectorSubtract(m_hookGameObject->getPosition(), m_playerMovement->position);
		m_hookState = hookState::flyYouFool;
		m_hookGameObject->getphysicsCompPtr()->setVelocity({0.f, 0.f , 0.f });
		m_audioComponent->playSound(m_chainExtendingSound, 1, true);
		m_shouldPlayChainSound = true;
	}
	else if (m_hookState == hookState::flyYouFool)
	{
		//DirectX::XMVECTOR addPosVector = DirectX::XMVectorAdd(DirectX::XMVectorSet(0, 5, 0, 0), DirectX::XMVectorDivide(m_playerMovement->forward, DirectX::XMVectorSet(2, 2, 2, 0)));
		DirectX::XMVECTOR addPosVector = DirectX::XMVectorSet(0, 4, 0, 0);

		m_toHeadDir = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(m_platformCenter, addPosVector), m_playerMovement->position);
		if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthEst(m_toHeadDir), 1) <= 1)
		{
			m_hookState = hookState::idle;
			m_gemGameObject->setVisibility(false);
		}
	}
	else
	{
		m_audioComponent->stopSound(1);
		if (m_hookState == hookState::waiting)
		{
			if (m_hookTimer.timeElapsed() >= hookDelayTime)
			{
				m_hookState = hookState::idle;
			}
		}
		m_hookGameObject->getMoveCompPtr()->rotation = m_playerMovement->rotation + hookRotOffsetConst;
		m_hookGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(m_gunGameObject->getMoveCompPtr()->position, hookPosOffset);

		// Wing rotation
		m_wingsOpen = false;
		// Reset speed for when reset() is used
		hookSpeedForward = 40.f;
		hookSpeedRetract = 40.f;
	}

	m_chain.update(dt);
}

bool HookHand::shouldFly() const
{
	bool shouldFly = false;
	if (m_hookState == hookState::flyYouFool)
	{
		shouldFly = true;
	}
	return shouldFly;
}