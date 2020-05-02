#include "pch.h"
#include "Chain.h"

Chain::Chain()
{
	this->m_visible = true;
	this->m_shooting = false;
	this->m_chainGObjects = nullptr;
	this->m_hookGObject = nullptr;
	this->m_gaunletGObject = nullptr;

	this->m_constant = 10.f;
	this->m_length = 0.6f;
	this->m_friction = 0.4f;
}

Chain::~Chain() {}

void Chain::initialize(GameObject* hookGObject, GameObject* gaunletGObject, std::vector<GameObject*>* chainGObjects)
{
	this->m_chainGObjects = chainGObjects;
	this->m_hookGObject = hookGObject;
	this->m_gaunletGObject = gaunletGObject;
	XMVECTOR pos;
	XMFLOAT3 hookPos = this->m_hookGObject->getMoveCompPtr()->getPositionF3();
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		pos = XMVectorSet(hookPos.x, hookPos.y, hookPos.z - this->m_length - (float)i * this->m_length, 1.f);
		this->m_chainGObjects->at(i)->setPosition(pos);
	}
}

bool Chain::isVisible() const
{
	return this->m_visible;
}

void Chain::setVisibility(bool visible)
{
	this->m_visible = visible;
	for (size_t i = 0; i < this->m_chainGObjects->size(); i++)
	{
		this->m_chainGObjects->at(i)->setVisibility(visible);
	}
}

void Chain::setShooting(bool shooting)
{
	this->m_shooting = shooting;
}

void Chain::update(float dt)
{
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS - 1; i++)
	{
		GameObject* link1;
		GameObject* link2;

		if (i == 0)
		{
			link1 = this->m_hookGObject;
			link2 = this->m_chainGObjects->at(i);
		}
		else
		{
			link1 = this->m_chainGObjects->at(i);
			link2 = this->m_chainGObjects->at(i - 1);
		}

		XMVECTOR linkVector = link1->getPosition() - link2->getPosition();

		float linkLength = XMVectorGetX(XMVector3LengthEst(linkVector));

		XMVECTOR finalForce = XMVectorZero();
		if (linkLength != 0)
		{
			XMVECTOR linkVectorNorm = XMVector3NormalizeEst(linkVector);
			// force = -k * (x - d)
			finalForce = finalForce + (linkVectorNorm * ((linkLength - this->m_length) * -this->m_constant));

			// friction force = -k * velocity
			// Adply friction
			XMFLOAT3 velocty1 = link1->getphysicsCompPtr()->getVelocity();
			XMFLOAT3 velocty2 = link2->getphysicsCompPtr()->getVelocity();

			XMVECTOR velocityDifference = XMVectorSet(
				velocty1.x - velocty2.x,
				velocty1.y - velocty2.y,
				velocty1.z - velocty2.z,
				0.f
			);
			finalForce += -(velocityDifference)*this->m_friction;

			// Apply force
			if (i != 0)
			{
				link1->setRotation(linkVector);
				link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
			}
			link2->setRotation(linkVector);
			link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);

				/*link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
			link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);*/
		}
	}

	for (size_t i = NR_OF_CHAIN_LINKS - 1; i > 0; i--)
	{
		GameObject* link1;
		GameObject* link2;
		XMVECTOR link1Position;
		if (i == NR_OF_CHAIN_LINKS - 1)
		{
			link1 = this->m_gaunletGObject;
			link2 = this->m_chainGObjects->at(i);
			if (this->m_shooting)
				link1Position = link1->getPosition();
			else
				link1Position = link1->getPosition() - (link1->getMoveCompPtr()->backward * 40);
		}
		else
		{
			link1 = this->m_chainGObjects->at(i);
			link2 = this->m_chainGObjects->at(i + 1);
			link1Position = link1->getPosition();
		}

		XMVECTOR linkVector = link1Position - link2->getPosition();

		float linkLength = XMVectorGetX(XMVector3LengthEst(linkVector));

		XMVECTOR finalForce = XMVectorZero();
		if (linkLength != 0)
		{
			XMVECTOR linkVectorNorm = XMVector3NormalizeEst(linkVector);
			// force = -k * (x - d)
			finalForce = finalForce + (linkVectorNorm * ((linkLength - this->m_length) * -this->m_constant));

			// friction force = -k * velocity
			// Adply friction
			XMFLOAT3 velocty1 = link1->getphysicsCompPtr()->getVelocity();
			XMFLOAT3 velocty2 = link2->getphysicsCompPtr()->getVelocity();

			XMVECTOR velocityDifference = XMVectorSet(
				velocty1.x - velocty2.x,
				velocty1.y - velocty2.y,
				velocty1.z - velocty2.z,
				0.f
			);
			finalForce += -(velocityDifference)*this->m_friction;

			// Apply force
			if (i != NR_OF_CHAIN_LINKS - 1)
				link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
			link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);
			/*{
				link1->setRotation(linkVector);
				link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
			}
			link2->setRotation(linkVector);
			link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);*/


		}
	}
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS - 1; i++)
	{
		XMFLOAT3 velocity = this->m_chainGObjects->at(i)->getphysicsCompPtr()->getVelocity();
		//this->m_chainGObjects->at(i)->setRotation(this->m_hookGObject->getMoveCompPtr()->forward);
		//this->m_chainGObjects->at(i)->setRotation(XMVectorSet(velocity.x, velocity.y, velocity.z, 1.f));
		this->m_chainGObjects->at(i)->getphysicsCompPtr()->updatePositionNoDecel(dt);
	}
}
