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
			finalForce = finalForce + (linkVectorNorm * ((linkLength - m_length) * -m_constant));
		else
			finalForce = finalForce + (linkVectorNorm * ((linkLength - m_length) * -(m_constant * 3)));

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
		finalForce += -(velocityDifference)*m_friction;

		// Apply force
		if (notFirst)
			link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
		link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);
	}
}

void Chain::linkRetractionUpdate()
{
	uint32_t nrOfLinks = (uint32_t)std::ceil(XMVectorGetX(XMVector3LengthEst(m_hookGObject->getPosition() - m_gaunletGObject->getPosition())) / (m_length));
	if (nrOfLinks > NR_OF_CHAIN_LINKS)
		nrOfLinks = NR_OF_CHAIN_LINKS;

	for (uint32_t i = 0; i < nrOfLinks; i++)
		m_chainRetracted[i] = false;

	for (int i = nrOfLinks - 1; i < NR_OF_CHAIN_LINKS; i++)
		m_chainRetracted[i] = true;
}

Chain::Chain()
{
	m_visible = true;
	m_shooting = false;
	m_retracting = false;
	m_chainGObjects = nullptr;
	m_nrOfUnretractedLinks = 0;
	m_hookGObject = nullptr;
	m_gaunletGObject = nullptr;

	m_constant = 20.f;
	m_length = 0.6f;
	m_friction = 0.45f;
}

Chain::~Chain() {}

void Chain::initialize(GameObject* hookGObject, GameObject* gaunletGObject, std::vector<GameObject*>* chainGObjects)
{
	m_chainGObjects = chainGObjects;
	m_hookGObject = hookGObject;
	m_gaunletGObject = gaunletGObject;
	m_chainRetracted.resize(NR_OF_CHAIN_LINKS);
	XMVECTOR pos;
	XMFLOAT3 hookPos = m_hookGObject->getMoveCompPtr()->getPositionF3();
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		pos = XMVectorSet(hookPos.x, hookPos.y, hookPos.z - m_length - (float)i * m_length, 1.f);
		m_chainGObjects->at(i)->setPosition(pos);
		m_chainRetracted[i] = i > 0 ? true : false;
	}
	m_nrOfUnretractedLinks = 1;
}

bool Chain::isVisible() const
{
	return m_visible;
}

void Chain::setVisibility(bool visible)
{
	m_visible = visible;
	for (size_t i = 0; i < m_chainGObjects->size(); i++)
		m_chainGObjects->at(i)->setVisibility(visible);
}

void Chain::setShooting(bool shooting)
{
	m_shooting = shooting;
}

void Chain::setRetracting(bool retracting)
{
	m_retracting = retracting;
}

void Chain::update(float dt)
{
	int lastUnRetractedlink = 0;
	GameObject* link1;
	GameObject* link2;
	linkRetractionUpdate();

	// Head to Hand
	for (uint32_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		if (i == 0)
		{
			link1 = m_hookGObject;
			link2 = m_chainGObjects->at(i);
		}
		else
		{
			if (m_chainRetracted[i])
				break;
			
			link1 = m_chainGObjects->at(i - 1);
			link2 = m_chainGObjects->at(i);
			lastUnRetractedlink = i;
		}

		if (!m_chainRetracted[i])
			simulateLinks(link1, link2, i != 0, dt);
	}

	// Hand to Head
	for (int i = lastUnRetractedlink; i >= 0; i--)
	{
		if (i == lastUnRetractedlink)
		{
			link1 = m_gaunletGObject;
			link2 = m_chainGObjects->at(i);
		}
		else
		{
			link1 = m_chainGObjects->at((size_t)i + 1);
			link2 = m_chainGObjects->at(i);
		}


		if (!m_chainRetracted[i])
			simulateLinks(link1, link2, i != lastUnRetractedlink, dt);
	}

	if (m_nrOfUnretractedLinks == 1)
		m_chainGObjects->at(0)->setVisibility(false);
	else
		m_chainGObjects->at(0)->setVisibility(true);

	// Update Position with Velocity
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		if (m_chainRetracted[i])
		{
			m_chainGObjects->at(i)->setPosition(m_gaunletGObject->getPosition());
			m_chainGObjects->at(i)->setRotation(m_gaunletGObject->getRotation());
			m_chainGObjects->at(i)->getphysicsCompPtr()->setVelocity({ 0.f, 0.f, 0.f });
		}
		XMFLOAT3 velocity = m_chainGObjects->at(i)->getphysicsCompPtr()->getVelocity();
		m_chainGObjects->at(i)->getphysicsCompPtr()->updatePositionNoDecel(dt);
	}
}