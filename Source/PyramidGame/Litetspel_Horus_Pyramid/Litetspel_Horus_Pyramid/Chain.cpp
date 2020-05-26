#include "pch.h"
#include "Chain.h"

void Chain::simulateLinks(GameObject* link1, GameObject* link2, bool notFirst, float dt)
{
	XMVECTOR linkVector;
	if (notFirst)
		linkVector = link1->getPosition() - link2->getPosition();
	else
		linkVector = (link1->getPosition() + (link1->getMoveCompPtr()->forward / 2)) - link2->getPosition();

	float linkLength = XMVectorGetX(XMVector3LengthEst(linkVector));

	XMVECTOR finalForce = XMVectorZero();
	if (linkLength != 0)
	{
		XMVECTOR linkVectorNorm = XMVector3NormalizeEst(linkVector);
		// force = -k * (x - d)
		if (notFirst)
			finalForce = finalForce + (linkVectorNorm * ((linkLength - this->m_length) * -this->m_constant));
		else
			finalForce = finalForce + (linkVectorNorm * ((linkLength - this->m_length) * -(this->m_constant * 3)));

		// Apply friction
		XMFLOAT3 velocty1 = link1->getphysicsCompPtr()->getVelocity();
		XMFLOAT3 velocty2 = link2->getphysicsCompPtr()->getVelocity();

		XMVECTOR velocityDifference = XMVectorSet(
			velocty1.x - velocty2.x,
			velocty1.y - velocty2.y,
			velocty1.z - velocty2.z,
			0.f
		);
		// friction force = -k * velocity
		finalForce += -(velocityDifference)*this->m_friction;

		// Apply force
		if (notFirst)
			link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
		link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);
	}
}

void Chain::linkRetractionUpdate()
{
	int nrOfLinks = std::ceil(XMVectorGetX(XMVector3LengthEst(this->m_hookGObject->getPosition() - this->m_gaunletGObject->getPosition())) / (this->m_length));
	for (int i = 0; i < nrOfLinks; i++)
		this->m_chainRetracted[i] = false;

	for (int i = nrOfLinks - 1; i < NR_OF_CHAIN_LINKS; i++)
		this->m_chainRetracted[i] = true;
}

Chain::Chain()
{
	this->m_visible = true;
	this->m_shooting = false;
	this->m_retracting = false;
	this->m_chainGObjects = nullptr;
	this->m_nrOfUnretractedLinks = 0;
	this->m_hookGObject = nullptr;
	this->m_gaunletGObject = nullptr;

	this->m_constant = 20.f;
	this->m_length = 0.6f;
	this->m_friction = 0.45f;
}

Chain::~Chain() {}

void Chain::initialize(GameObject* hookGObject, GameObject* gaunletGObject, std::vector<GameObject*>* chainGObjects)
{
	this->m_chainGObjects = chainGObjects;
	this->m_hookGObject = hookGObject;
	this->m_gaunletGObject = gaunletGObject;
	this->m_chainRetracted.resize(NR_OF_CHAIN_LINKS);
	XMVECTOR pos;
	XMFLOAT3 hookPos = this->m_hookGObject->getMoveCompPtr()->getPositionF3();
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		pos = XMVectorSet(hookPos.x, hookPos.y, hookPos.z - this->m_length - (float)i * this->m_length, 1.f);
		this->m_chainGObjects->at(i)->setPosition(pos);
		this->m_chainRetracted[i] = i > 0 ? true : false;
	}
	this->m_nrOfUnretractedLinks = 1;
}

bool Chain::isVisible() const
{
	return this->m_visible;
}

void Chain::setVisibility(bool visible)
{
	this->m_visible = visible;
	for (size_t i = 0; i < this->m_chainGObjects->size(); i++)
		this->m_chainGObjects->at(i)->setVisibility(visible);
}

void Chain::setShooting(bool shooting)
{
	this->m_shooting = shooting;
}

void Chain::setRetracting(bool retracting)
{
	this->m_retracting = retracting;
}

void Chain::update(float dt)
{
	int lastUnRetractedlink = 0;
	GameObject* link1;
	GameObject* link2;
	this->linkRetractionUpdate();

	// Head to Hand
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		if (i == 0)
		{
			link1 = this->m_hookGObject;
			link2 = this->m_chainGObjects->at(i);
		}
		else
		{
			if (this->m_chainRetracted[i])
				break;
			
			link1 = this->m_chainGObjects->at(i - 1);
			link2 = this->m_chainGObjects->at(i);
			lastUnRetractedlink = i;
		}

		if (!this->m_chainRetracted[i])
			this->simulateLinks(link1, link2, i != 0, dt);
	}

	// Hand to Head
	for (int i = lastUnRetractedlink; i >= 0; i--)
	{
		if (i == lastUnRetractedlink)
		{
			link1 = this->m_gaunletGObject;
			link2 = this->m_chainGObjects->at(i);
		}
		else
		{
			link1 = this->m_chainGObjects->at((size_t)i + 1);
			link2 = this->m_chainGObjects->at(i);
		}


		if (!this->m_chainRetracted[i])
			this->simulateLinks(link1, link2, i != lastUnRetractedlink, dt);
	}

	if (this->m_nrOfUnretractedLinks == 1)
		this->m_chainGObjects->at(0)->setVisibility(false);
	else
		this->m_chainGObjects->at(0)->setVisibility(true);

	// Update Position with Velocity
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		if (this->m_chainRetracted[i])
		{
			this->m_chainGObjects->at(i)->setPosition(this->m_gaunletGObject->getPosition());
			this->m_chainGObjects->at(i)->setRotation(this->m_gaunletGObject->getRotation());
			this->m_chainGObjects->at(i)->getphysicsCompPtr()->setVelocity({ 0.f, 0.f, 0.f });
		}
		XMFLOAT3 velocity = this->m_chainGObjects->at(i)->getphysicsCompPtr()->getVelocity();
		this->m_chainGObjects->at(i)->getphysicsCompPtr()->updatePositionNoDecel(dt);
	}
}