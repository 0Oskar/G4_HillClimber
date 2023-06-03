#include "pch.h"
#include "Portal.h"

Portal::Portal()
{
	m_teleportLocation = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_player = nullptr;
	m_changeActiveRoom = false;
	m_roomID = -1;
	m_oneTimeUse = false;
	m_active = false;
}

Portal::~Portal()
{

}

void Portal::setActiveStatus(bool activeStatus)
{
	m_active = activeStatus;
}

void Portal::initialize(int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation,Player * player, int roomID, bool oneTimeUse)
{

	m_teleportLocation = teleportLocation + XMVectorSet(0.f, player->getAABB().Extents.y + 0.1f, 0.f, 1.f);

	m_player = player;
	m_roomID = roomID;
	m_oneTimeUse = oneTimeUse;
	m_active = true;

	initializeStatic(false, wvpCBufferIndex, mdl);
	
}

void Portal::update()
{
	if (!m_active) return;
	if (getAABB().Intersects(m_player->getAABB()))
	{
		if (m_oneTimeUse)
			m_active = false;

		m_player->getphysicsCompPtr()->setVelocity({ 0.f, 0.f, 0.f });
		m_player->setPosition(m_teleportLocation);
		m_player->getphysicsCompPtr()->setVelocity({0.f, 0.f, 0.f});
		if (m_roomID != -1)
		{
			m_changeActiveRoom = true;
		}
	}
}

void Portal::resetActiveRoomVariable()
{
	m_changeActiveRoom = false;
}

bool Portal::shouldChangeActiveRoom() const
{
	if (m_roomID != -1)
		return m_changeActiveRoom;
	else
		return false;
}

int Portal::getRoomID() const
{
	return m_roomID;
}
