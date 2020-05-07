#include "pch.h"
#include"TemplateRoom.h"

TemplateRoom::TemplateRoom()
{
	Room::initParent();
}
TemplateRoom::~TemplateRoom()
{
	Room::~Room();
}

void TemplateRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);
}

void TemplateRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
}

void TemplateRoom::portals()
{
	//Add portals here
}

void TemplateRoom::onEntrance()
{
	//What should happen when player enter room
}
void TemplateRoom::createBoundingBoxes()
{
	//Add all boundingboxes that are not part of a gameobject(Walls and ground for rooms etc)
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
}
void TemplateRoom::createSceneObjects()
{
	//Add all gameobjects that the room should have.
	this->addPlatformToRoom(3, &m_models->at(3), DirectX::XMVectorSet(0, 5, 1, 1.f), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
}
void TemplateRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
}