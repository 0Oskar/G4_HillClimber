#include"pch.h"
#include"HookHand.h"

HookHand::HookHand()
{
	this->m_hookState = hookState::idle;
}

void HookHand::init(GameObject* gObject, MovementComponent* movementComponent)
{
	MaterialData mat;
	mat.diffuse = DirectX::XMFLOAT4(0.5, 0.5, 0.5, 1);
	this->m_hookGameObject = gObject;
	this->m_playerMovement = movementComponent;
	this->m_hookGameObject->setPosition(this->m_playerMovement->position);
	this->m_hookTimer.start();
	
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
		this->m_shootDirection = this->m_hookGameObject->getMoveCompPtr()->forward;
		this->m_hookState = hookState::shooting;
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
}

void HookHand::update(float dt)
{
	if (m_hookState == hookState::shooting)
	{
		this->m_hookGameObject->getMoveCompPtr()->move(DirectX::XMVectorScale(this->m_shootDirection, dt * this->hookSpeedForward));
	}
	else if (m_hookState == hookState::recalling)
	{
		this->m_origin = DirectX::XMVectorSubtract(this->m_playerMovement->position, this->m_hookGameObject->getPosition());

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
	else
	{
		if (this->m_hookState == hookState::waiting)
		{
			if (this->m_hookTimer.timeElapsed() >= this->hookDelayTime)
			{
				this->m_hookState = hookState::idle;
			}
		}
		this->m_hookGameObject->getMoveCompPtr()->position = this->m_playerMovement->position;
		this->m_hookGameObject->getMoveCompPtr()->rotation = this->m_playerMovement->rotation;
	}
}