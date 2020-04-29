#include "pch.h"
#include "Chain.h"

Chain::Chain()
{
	this->m_chainGObjects = nullptr;
	this->m_hookGObject = nullptr;
	this->m_gaunletGObject = nullptr;

	this->m_constant = 0.5f;
	this->m_length = 0.6f;
	this->m_friction = 0.1f;
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
				link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
			link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);

			/*if (linkLength >= this->m_length)
			{
				link1->getphysicsCompPtr()->setVelocity(XMFLOAT3(0.f, 0.f, 0.f));
				link2->getphysicsCompPtr()->setVelocity(XMFLOAT3(0.f, 0.f, 0.f));
			}*/

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
			link1Position = link1->getPosition() - link1->getMoveCompPtr()->backward - link1->getMoveCompPtr()->backward;
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
		}
		
		//XMFLOAT3 gaunletPos = this->m_gaunletGObject->getMoveCompPtr()->getPositionF3();
		//XMVECTOR pos = XMVectorSet(gaunletPos.x, gaunletPos.y, gaunletPos.z + 3.f, 1.f);
		//this->m_chainGObjects->at(NR_OF_CHAIN_LINKS - 1)->setPosition(pos);
	}
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS - 1; i++)
		this->m_chainGObjects->at(i)->getphysicsCompPtr()->updatePositionNoDecel(dt);
}
