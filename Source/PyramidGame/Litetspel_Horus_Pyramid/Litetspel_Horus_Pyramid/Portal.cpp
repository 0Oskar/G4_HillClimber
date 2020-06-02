#include "pch.h"
#include "Portal.h"

Portal::Portal()
{
	this->m_teleportLocation = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	this->player = nullptr;
	this->m_changeActiveRoom = false;
	this->m_roomID = -1;
	this->m_oneTimeUse = false;
	this->m_active = false;
}

Portal::~Portal()
{

}

void Portal::setActiveStatus(bool activeStatus)
{
	this->m_active = activeStatus;
}

void Portal::initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation,Player * player, int roomID, bool oneTimeUse)
{

	this->m_teleportLocation = teleportLocation + XMVectorSet(0.f, player->getAABB().Extents.y + 0.1f, 0.f, 1.f);

	this->player = player;
	this->m_roomID = roomID;
	this->m_oneTimeUse = oneTimeUse;
	this->m_active = true;

	initializeStatic(false, modelIndex, wvpCBufferIndex, mdl);
	
}

void Portal::update()
{
	if (!m_active) return;
	if (getAABB().Intersects(player->getAABB()))
	{
		if (this->m_oneTimeUse)
			this->m_active = false;

		this->player->getphysicsCompPtr()->setVelocity({ 0.f, 0.f, 0.f });
		this->player->setPosition(this->m_teleportLocation);
		this->player->getphysicsCompPtr()->setVelocity({0.f, 0.f, 0.f});
		if (m_roomID != -1)
		{
			m_changeActiveRoom = true;
		}
	}
}

void Portal::resetActiveRoomVariable()
{
	this->m_changeActiveRoom = false;
}

bool Portal::shouldChangeActiveRoom() const
{
	if (this->m_roomID != -1)
		return this->m_changeActiveRoom;
	else
		return false;
}

int Portal::getRoomID() const
{
	return m_roomID;
}
