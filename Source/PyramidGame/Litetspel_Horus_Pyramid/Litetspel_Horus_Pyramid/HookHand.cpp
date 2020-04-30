#include"pch.h"
#include"HookHand.h"

HookHand::HookHand()
{
	this->m_hookState = hookState::idle;
	this->m_audioEngine = nullptr;
}


void HookHand::init(GameObject* gObject, MovementComponent* movementComponent, std::vector<DirectX::BoundingBox*>* boundingBoxes, GameObject* hookGun, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{
	MaterialData mat;
	mat.diffuse = DirectX::XMFLOAT4(0.5, 0.5, 0.5, 1);
	this->m_hookGameObject = gObject;
	this->m_gunGameObject = hookGun;
	this->m_playerMovement = movementComponent;
	this->m_hookGameObject->setPosition(this->m_playerMovement->position);
	this->m_hookTimer.start();
	this->m_boundingBoxes = boundingBoxes;

	//Audio
	this->m_audioEngine = audioEngine;
	this->m_fireSound = std::make_shared<DirectX::SoundEffect>(audioEngine.get(), L"Sounds/Explo1.wav");
	this->m_ejectSound = std::make_shared<DirectX::SoundEffect>(audioEngine.get(), L"Sounds/Explo1.wav");
	m_effect = m_ejectSound->CreateInstance();
	m_effect->SetVolume(0.002f);
}

bool HookHand::canFire()
{
	return m_hookState == hookState::idle;
}
bool HookHand::canRecall()
{
	return m_hookState == hookState::shooting;
}
void HookHand::fire()
{
	if (this->canFire())
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_playerMovement->rotation);

		DirectX::XMVECTOR forwardX = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		this->m_shootDirection = forwardX;
		this->m_hookState = hookState::shooting;
		this->m_effect->Stop();
		this->m_effect->Play(false);
	}
}

void HookHand::retract()
{
	if (this->canRecall())
	{
		this->m_hookState = hookState::recalling;
		this->m_origin = DirectX::XMVectorSubtract(this->m_playerMovement->position, this->m_hookGameObject->getPosition());
		this->m_hookState = hookState::recalling;
	}
	else if (this->m_hookState == hookState::flyYouFool)
	{
		this->m_hookState = hookState::idle;
	}
}

bool HookHand::colide()
{
	bool colided = false;
	for (int i = 0; i < m_boundingBoxes->size(); i++)
	{
		if (this->m_hookGameObject->getAABB().Intersects(*m_boundingBoxes->at(i)))
		{
			DirectX::XMVECTOR posToTopAndCenterOfBox = DirectX::XMVectorSet(DirectX::XMVectorGetX(this->m_hookGameObject->getPosition()), m_boundingBoxes->at(i)->Center.y, DirectX::XMVectorGetZ(this->m_hookGameObject->getPosition()), 0);
			posToTopAndCenterOfBox = DirectX::XMVectorAdd(posToTopAndCenterOfBox, DirectX::XMVectorSet(0, m_boundingBoxes->at(i)->Extents.y + (this->m_hookGameObject->getAABB().Extents.y * 2), 0, 0));  // Move in Y the platform extends then + the size of the boundingbox of the hookHead.
			m_platformCenter = DirectX::XMVectorAdd(posToTopAndCenterOfBox, DirectX::XMVectorScale(this->m_playerMovement->forward, 2));
			colided = true;
		}
	}
	return  colided;
}
void HookHand::updateHandModel()
{
	DirectX::XMFLOAT4 rot;
	DirectX::XMStoreFloat4(&rot, this->m_playerMovement->rotation);
	rot.x = -rot.x;

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_playerMovement->rotation);
	DirectX::XMVECTOR rotOff;
	rotOff = this->invertX(this->m_playerMovement->rotation);

	DirectX::XMVECTOR gunPosOffsetRotated = XMVector3TransformCoord(this->gunPosOffset, rotationMatrix);
	this->m_gunGameObject->getMoveCompPtr()->rotation = DirectX::XMVectorAdd(rotOff, gunRotOffset);
	this->m_gunGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(this->m_playerMovement->position, gunPosOffsetRotated);
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
	
	this->updateHandModel();
	if (m_hookState == hookState::shooting)
	{
		if (this->colide())
		{
			this->m_hookState = hookState::hit;
		}
		else
			this->m_hookGameObject->getMoveCompPtr()->move(DirectX::XMVectorScale(this->m_shootDirection, dt * this->hookSpeedForward));

		DirectX::XMVECTOR fromPlayerToHook = DirectX::XMVectorSubtract(this->m_hookGameObject->getPosition(), this->m_playerMovement->position);
		if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthEst(fromPlayerToHook), 1) >= maxDistance)
		{
			this->m_hookState = hookState::recalling;
		}

	}
	else if (m_hookState == hookState::recalling)
	{
		this->m_origin = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(this->m_gunGameObject->getMoveCompPtr()->position, hookPosOffset), this->m_hookGameObject->getPosition());

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
		if (this->m_hookState == hookState::waiting)
		{
			if (this->m_hookTimer.timeElapsed() >= this->hookDelayTime)
			{
				this->m_hookState = hookState::idle;
			}
		}
		this->m_hookGameObject->getMoveCompPtr()->rotation = DirectX::XMVectorAdd(this->invertX(this->m_playerMovement->rotation), this->hookRotOffsetConst);;
		this->m_hookGameObject->getMoveCompPtr()->position = DirectX::XMVectorAdd(this->m_gunGameObject->getMoveCompPtr()->position, hookPosOffset);
	}
	
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