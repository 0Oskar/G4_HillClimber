#include "pch.h"
#include "Portal.h"

Portal::Portal()
{
	this->m_teleportLocation = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	this->player = nullptr;
	this->m_changeActiveRoom = false;
	this->m_roomID = -1;
}

Portal::~Portal()
{

}

void Portal::initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation,Player * player, int roomID)
{

	this->m_teleportLocation = teleportLocation + XMVectorSet(0.f, player->getAABB().Extents.y + 0.1f, 0.f, 1.f);

	this->player = player;
	this->m_roomID = roomID;

	initializeStatic(false, modelIndex, wvpCBufferIndex, mdl);
	
}

void Portal::update()
{
	if (getAABB().Intersects(player->getAABB()))
	{
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

bool Portal::shouldChangeActiveRoom()
{
	if (this->m_roomID != -1)
		return this->m_changeActiveRoom;
	else
		return false;
}

int Portal::getRoomID()
{
	return m_roomID;
}
