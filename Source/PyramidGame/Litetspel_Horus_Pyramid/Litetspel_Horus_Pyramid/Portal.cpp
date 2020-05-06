#include "Portal.h"

Portal::Portal()
{
	this->m_teleportLocation = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	this->player = nullptr;
}

Portal::~Portal()
{

}

void Portal::initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation,Player * player)
{
	this->m_teleportLocation = teleportLocation + XMVectorSet(0.f, player->getAABB().Extents.y + 0.1f, 0.f, 1.f);
	this->player = player;

	initializeStatic(false, modelIndex, wvpCBufferIndex, mdl);
	
}

void Portal::update()
{
	if (getAABB().Intersects(player->getAABB()))
	{
		this->player->setPosition(this->m_teleportLocation);
		this->player->getphysicsCompPtr()->setVelocity({0.f, 0.f, 0.f});
	}
}
