#include "pch.h"
#include"KevinsRoom.h"

KevinsRoom::KevinsRoom()
{
	Room::initParent();
}
KevinsRoom::~KevinsRoom()
{
	Room::~Room();
}

void KevinsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);
}

void KevinsRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
}

void KevinsRoom::portals()
{
	//Add portals here
}

void KevinsRoom::onEntrance()
{
	//What should happen when player enter room
}
void KevinsRoom::createBoundingBoxes()
{
	//Add all boundingboxes that are not part of a gameobject(Walls and ground for rooms etc)
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
}
void KevinsRoom::createSceneObjects()
{
	

}
void KevinsRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
}