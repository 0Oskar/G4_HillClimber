#include "pch.h"
#include "Chain.h"

using namespace DirectX;

void Chain::simulateLinks(GameObject* link1, GameObject* link2, bool notFirst, float dt)
{
	assert(!pMath::F3IsNaN(link1->getphysicsCompPtr()->getVelocity()));
	assert(!pMath::F3IsNaN(link2->getphysicsCompPtr()->getVelocity()));

	XMVECTOR link1Position = link1->getPosition();
	if (link1 == m_gaunletGObject)
		link1Position = m_gauntletSpawnPosition;

	XMVECTOR link2Position = link2->getPosition();
	assert(!XMVector3IsNaN(link1Position));
	assert(!XMVector3IsNaN(link2Position));

	XMVECTOR vectorBetweenLinks = link1Position - link2Position;

	if (!notFirst)
		vectorBetweenLinks += link1->getMoveCompPtr()->forward * m_linkLength;

	float lengthBetweenLinks = XMVectorGetX(XMVector3Length(vectorBetweenLinks));
	if (isinf(lengthBetweenLinks)) {
		return;
	}

	if (lengthBetweenLinks == 0.f) return;
	
	XMVECTOR linkVectorNorm = XMVector3Normalize(vectorBetweenLinks);
	assert(!XMVector3IsNaN(linkVectorNorm));

	// force = -k * (x - d)
	XMVECTOR finalForce = (linkVectorNorm * ((lengthBetweenLinks - m_linkLength) * -m_constant));
	assert(!XMVector3IsNaN(finalForce));

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
	finalForce += -velocityDifference * m_friction;
	assert(!XMVector3IsNaN(finalForce));

	// Apply force
	if (notFirst)
		link1->getphysicsCompPtr()->addForceDir(finalForce, dt);
	link2->getphysicsCompPtr()->addForceDir(-finalForce, dt);

}

void Chain::linkRetractionUpdate()
{
	XMVECTOR hookToGaunletVector = XMVectorSubtract(m_hookGObject->getPosition(), m_gauntletSpawnPosition);
	uint32_t nrOfLinks = (uint32_t)(XMVectorGetX(XMVector3Length(hookToGaunletVector)) / (m_linkLength * 1.1f));
	if (nrOfLinks > NR_OF_CHAIN_LINKS)
		nrOfLinks = NR_OF_CHAIN_LINKS;

	for (uint32_t i = 0; i < nrOfLinks; i++)
	{
		m_retractedLinks[i] = false;
	}
	for (uint32_t i = nrOfLinks; i < NR_OF_CHAIN_LINKS; i++)
	{
		m_retractedLinks[i] = true;
		m_chainGObjects->at(i)->setPosition(m_gaunletGObject->getPosition());
		m_chainGObjects->at(i)->getphysicsCompPtr()->setVelocity({0.f, 0.f, 0.f });
	}
}

void Chain::calculateGauntletLinkSpawnPosition()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(m_gaunletGObject->getRotation());
	m_gauntletSpawnPosition = m_gaunletGObject->getPosition() + XMVector3TransformCoord(XMVectorSet(0.f, 0.3f, 0.6f, 0.f), rotationMatrix);
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
	m_gauntletSpawnPosition = XMVectorZero();

	m_constant = 40.f;
	m_linkLength = 0.45f;
	m_friction = 0.36f;
}

Chain::~Chain() {}

void Chain::initialize(GameObject* hookGObject, GameObject* gaunletGObject, std::vector<GameObject*>* chainGObjects)
{
	m_chainGObjects = chainGObjects;
	m_hookGObject = hookGObject;
	m_gaunletGObject = gaunletGObject;
	m_retractedLinks.resize(NR_OF_CHAIN_LINKS);
	//XMVECTOR pos;
	calculateGauntletLinkSpawnPosition();
	for (uint32_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		//pos = XMVectorSet(gauntletPos.x, gauntletPos.y, gauntletPos.z - m_linkLength - (float)i * m_linkLength, 1.f);
		m_chainGObjects->at(i)->setPosition(m_gauntletSpawnPosition);
		assert(!XMVector3IsNaN(m_gauntletSpawnPosition));
		assert(!pMath::F3IsNaN(m_chainGObjects->at(i)->getphysicsCompPtr()->getVelocity()));
		m_chainGObjects->at(i)->setRotation(m_gaunletGObject->getRotation());
		m_chainGObjects->at(i)->getphysicsCompPtr()->setVelocity({ 0.f, 0.f, 0.f });
		m_retractedLinks[i] = i > 0 ? true : false;
	}
	m_nrOfUnretractedLinks = 1;
}

bool Chain::isVisible() const
{
	return m_visible;
}

XMVECTOR Chain::getLinkPosition(uint32_t index)
{
	return m_chainGObjects->at(index)->getPosition();
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
	calculateGauntletLinkSpawnPosition();
	int32_t lastUnRetractedlink = 0;
	GameObject* link1;
	GameObject* link2;
	linkRetractionUpdate();

	// Head to Hand
	for (int32_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		if (i == 0)
		{
			link1 = m_hookGObject;
			link2 = m_chainGObjects->at(i);
		}
		else
		{
			if (m_retractedLinks[i])
				break;
			
			link1 = m_chainGObjects->at((size_t)i - 1);
			link2 = m_chainGObjects->at(i);
			lastUnRetractedlink = i;
		}

		if (!m_retractedLinks[i])
			simulateLinks(link1, link2, i != 0, dt);
	}

	// Hand to Head
	for (int32_t i = lastUnRetractedlink; i >= 0; i--)
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

		if (!m_retractedLinks[i])
			simulateLinks(link1, link2, i != lastUnRetractedlink, dt);
	}

	if (m_nrOfUnretractedLinks == 1)
		m_chainGObjects->at(0)->setVisibility(false);
	else
		m_chainGObjects->at(0)->setVisibility(true);

	// Update Position with Velocity
	for (size_t i = 0; i < NR_OF_CHAIN_LINKS; i++)
	{
		if (m_retractedLinks[i])
		{
			m_chainGObjects->at(i)->setPosition(m_gaunletGObject->getPosition());
			assert(!XMVector3IsNaN(m_chainGObjects->at(i)->getMoveCompPtr()->position));
			m_chainGObjects->at(i)->setRotation(m_gaunletGObject->getRotation());
			m_chainGObjects->at(i)->getphysicsCompPtr()->setVelocity({ 0.f, 0.f, 0.f });
		}
		else
		{
			XMFLOAT3 velocityF3 = m_chainGObjects->at(i)->getphysicsCompPtr()->getVelocity();
			assert(!pMath::F3IsNaN(velocityF3));
			XMVECTOR velocity = XMVector3Normalize(XMLoadFloat3(&velocityF3));
			XMStoreFloat3(&velocityF3, velocity);

			m_chainGObjects->at(i)->getphysicsCompPtr()->setVelocity(velocityF3);
			
			// Handle rotation
			XMVECTOR link1Position = m_chainGObjects->at(i)->getPosition();
			XMVECTOR link2Position;
			if (i == lastUnRetractedlink)
			{
				link2Position = m_gauntletSpawnPosition;
			}
			else
			{
				link2Position = m_chainGObjects->at(i + 1)->getPosition();
			}

			XMVECTOR toLink = link1Position - link2Position;
			if (XMVector3Equal(toLink, XMVectorZero())) continue;

			XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);
			XMVECTOR forwardDirection = XMVector3Normalize(toLink);

			float dot = XMVectorGetX(XMVector3Dot(-forwardDirection, XMVectorSet(0.f, 0.f, 1.f, 0.f)));
			if (dot > 0.f)
				forwardDirection = XMVectorNegate(forwardDirection);

			XMMATRIX orientation = XMMatrixLookToLH(link1Position, forwardDirection, upDirection);
			m_chainGObjects->at(i)->setRotation(pMath::rotationMatrixToEulerAngles(orientation));
		}

		m_chainGObjects->at(i)->getphysicsCompPtr()->updatePositionNoDecel(dt);
	}

	// Link Distance Snapping
	float constraintDistance = m_linkLength * 1.2f;
	float hookGauntletConstraintDistance = m_linkLength * 0.2f;

	// Head to Hand
	for (int32_t i = 0; i < lastUnRetractedlink; i++)
	{
		float constraint = constraintDistance;
		XMVECTOR link1Position;
		XMVECTOR link2Position;

		link2 = m_chainGObjects->at(i);
		if (i == 0)
		{
			link1Position = m_hookGObject->getPosition();
			link2Position = link2->getPosition();
			constraint = hookGauntletConstraintDistance;
		}
		else
		{
			link1Position = m_chainGObjects->at((size_t)i - 1)->getPosition();
			link2Position = link2->getPosition();
		}

		XMVECTOR vectorBetweenFirstLastLinks = link1Position - link2Position;
		float lengthBetweenLinks = XMVectorGetX(XMVector3Length(vectorBetweenFirstLastLinks));

		if (lengthBetweenLinks > 0.f && lengthBetweenLinks > constraint) {
			XMVECTOR dir = XMVector3Normalize(link2Position - link1Position);
			link2->setPosition(link1Position + (constraint * dir));
		}
	}

	// Hand to Head
	for (int32_t i = lastUnRetractedlink; i > 0; i--)
	{
		float constraint = constraintDistance;
		XMVECTOR link1Position;
		XMVECTOR link2Position;

		link2 = m_chainGObjects->at(i);
		if (i == lastUnRetractedlink)
		{
			link1Position = m_gauntletSpawnPosition;
			link2Position = link2->getPosition();
			constraint = hookGauntletConstraintDistance;
		}
		else
		{
			link1Position = m_chainGObjects->at((size_t)i + 1)->getPosition();
			link2Position = link2->getPosition();
		}

		XMVECTOR vectorBetweenFirstLastLinks = link1Position - link2Position;
		float lengthBetweenLinks = XMVectorGetX(XMVector3Length(vectorBetweenFirstLastLinks));

		if (lengthBetweenLinks > 0.f && lengthBetweenLinks > constraint) {
			XMVECTOR dir = XMVector3Normalize(link2Position - link1Position);
			link2->setPosition(link1Position + (constraint * dir));
		}
	}

}