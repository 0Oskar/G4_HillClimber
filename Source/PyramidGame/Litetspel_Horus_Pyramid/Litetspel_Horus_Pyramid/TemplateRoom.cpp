#include "pch.h"
#include "TemplateRoom.h"

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
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	//Add all boundingboxes that are not part of a gameobject(Walls and ground for rooms etc)
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
}
void TemplateRoom::createSceneObjects()
{
	//Add all gameobjects that the room should have.
	this->addPlatformToRoom(3, &m_models->at(3), DirectX::XMVectorSet(0, 5, 1, 1.f), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	//Demonstrate addOjvectToRoom function
	this->m_objectTest = new Lever(); //Create any child of gameobject here. In this case a lever
	this->m_objectTest->init(false, 7, -1, &m_models->at(7)); //Needs to be initialized. 
	this->addObjectToRoom(this->m_objectTest); //Function add pointer to m_gameobjects and assigns it a wvpbuffer.
	this->m_objectTest->setPosition({ 0, 0, 0 }); //We can now set positions etc

}
void TemplateRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
}