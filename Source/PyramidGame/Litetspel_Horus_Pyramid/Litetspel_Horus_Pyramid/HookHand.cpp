#include"pch.h"
#include"HookHand.h"

HookHand::HookHand()
{
	this->m_hookState = hookState::idle;
}

void HookHand::init(GameObject* gObject, MovementComponent* movementComponent, std::vector<DirectX::BoundingBox*>* boundingBoxes)
{
	MaterialData mat;
	mat.diffuse = DirectX::XMFLOAT4(0.5, 0.5, 0.5, 1);
	this->m_hookGameObject = gObject;
	this->m_playerMovement = movementComponent;
	this->m_hookGameObject->setPosition(this->m_playerMovement->position);
	this->m_hookTimer.start();
	this->m_boundingBoxes = boundingBoxes;
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
			DirectX::XMVECTOR posToTopAndCenterOfBox = DirectX::XMVectorSet(m_boundingBoxes->at(i)->Center.x, m_boundingBoxes->at(i)->Center.y, m_boundingBoxes->at(i)->Center.z, 0); //Get center position of "Plattform"
			posToTopAndCenterOfBox = DirectX::XMVectorAdd(posToTopAndCenterOfBox, DirectX::XMVectorSet(0, m_boundingBoxes->at(i)->Extents.y + (this->m_hookGameObject->getAABB().Extents.y * 2), 0, 0));  // Move in Y the platform extends then + the size of the boundingbox of the hookHead.
			m_platformCenter = posToTopAndCenterOfBox;
			colided = true;
		}
	}
	return  colided;
}

void HookHand::update(float dt)
{
	if (m_hookState == hookState::shooting)
	{
		if (this->colide())
		{
			this->m_hookState = hookState::hit;
		}
		else
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
	else if (m_hookState == hookState::hit)
	{
		this->m_toHeadDir = DirectX::XMVectorSubtract(this->m_hookGameObject->getPosition(), this->m_playerMovement->position);
		this->m_hookState = hookState::flyYouFool;
	}
	else if(m_hookState == hookState::flyYouFool)
	{
		//DirectX::XMVECTOR addPosVector = DirectX::XMVectorAdd(DirectX::XMVectorSet(0, 5, 0, 0), DirectX::XMVectorDivide(this->m_playerMovement->forward, DirectX::XMVectorSet(2, 2, 2, 0)));
		DirectX::XMVECTOR addPosVector = DirectX::XMVectorSet(0, 3, 0, 0);

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
		this->m_hookGameObject->getMoveCompPtr()->position = this->m_playerMovement->position;
		this->m_hookGameObject->getMoveCompPtr()->rotation = this->m_playerMovement->rotation;
		this->m_hookGameObject->getMoveCompPtr()->updateDirVectors();
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