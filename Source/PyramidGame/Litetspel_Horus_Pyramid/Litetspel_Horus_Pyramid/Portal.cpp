#include "Portal.h"

Portal::Portal()
{
	this->m_teleportLocation = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	this->player = nullptr;
	this->m_changeActiveRoom = false;
	this->m_roomPtr = nullptr;
}

Portal::~Portal()
{

}

void Portal::initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation,Player * player, Room* room)
{
	if (room != nullptr) //If we have no room then use portal 
	{
		this->m_teleportLocation = teleportLocation + XMVectorSet(0.f, player->getAABB().Extents.y + 0.1f, 0.f, 1.f);
	}
	else
	{
		this->m_teleportLocation = room->getEntrancePosition();
	}
	this->player = player;
	this->m_roomPtr = room;

	initializeStatic(false, modelIndex, wvpCBufferIndex, mdl);
	
}

void Portal::update()
{
	if (getAABB().Intersects(player->getAABB()))
	{
		this->player->setPosition(this->m_teleportLocation);
		this->player->getphysicsCompPtr()->setVelocity({0.f, 0.f, 0.f});
		if (m_roomPtr)
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
	if (this->m_roomPtr != nullptr)
		return this->m_changeActiveRoom;
	else
		return false;
}

Room* Portal::getRoomPtr()
{
	return m_roomPtr;
}
