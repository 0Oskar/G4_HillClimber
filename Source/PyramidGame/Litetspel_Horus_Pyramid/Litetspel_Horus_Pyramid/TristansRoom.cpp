#include "pch.h"
#include "TristansRoom.h"

void TristansRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	this->addBoundingBox({ 70, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Left Wall
	this->addBoundingBox({ -70, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Right Wall
	this->addBoundingBox({ 0, 4.5f, 107 }, DirectX::XMFLOAT3(100, 75, 1)); //Back Wall
	this->addBoundingBox({ 0, 4.5f, -100 }, DirectX::XMFLOAT3(100, 75, 1)); //Front Wall

	this->addBoundingBox({ 0, 45, 90 }, DirectX::XMFLOAT3(100, 1, 30)); //Floor 1
	this->addBoundingBox({ 0, 45, -52 }, DirectX::XMFLOAT3(100, 1, 50)); //Floor 2
	this->addBoundingBox({ 0, 45, 0 }, DirectX::XMFLOAT3(50, 1, 100)); //Floor TEMP

}

void TristansRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// PuzzleRoom
	pos = DirectX::XMVectorSet(0, 4.5f + 45, 0, 1); // World pos
	this->addGameObjectToRoom(true, false, 1, 21, &m_models->at(21), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));
}

void TristansRoom::onCompleted()
{

}

TristansRoom::TristansRoom()
{
	Room::initParent();
	this->m_entrencePosition = { -2, 42 , 90, 1 };
}

TristansRoom::~TristansRoom()
{
	Room::~Room();
}

void TristansRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);
}

void TristansRoom::onEntrance()
{

}

void TristansRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
}

void TristansRoom::portals()
{

}
