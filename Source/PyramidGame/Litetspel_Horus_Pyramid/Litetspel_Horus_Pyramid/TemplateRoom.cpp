#include "pch.h"
#include "TemplateRoom.h"

TemplateRoom::TemplateRoom()
{
	Room::Room();
}
TemplateRoom::~TemplateRoom()
{
	Room::~Room();
}

void TemplateRoom::update(float dt, Camera* camera)
{
	Room::update(dt, camera);
}

void TemplateRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
}

void TemplateRoom::onEntrance()
{

}
void TemplateRoom::createBoundingBoxes()
{
	this->addBoundingBox({ -10, 15, -10, 1 }, { 5, 5, 2 });
}
void TemplateRoom::createSceneObjects()
{
	this->addPlatformToRoom(3, nullptr, &m_models->at(3), DirectX::XMVectorSet(0, 5, 1, 1.f), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
}
void TemplateRoom::onCompleted()
{

}