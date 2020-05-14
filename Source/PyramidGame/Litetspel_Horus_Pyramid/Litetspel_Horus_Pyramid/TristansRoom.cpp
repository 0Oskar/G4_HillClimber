#include "pch.h"
#include "TristansRoom.h"

void TristansRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
}

void TristansRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// PuzzleRoom
	pos = DirectX::XMVectorSet(0, 0, 0, 1); // World pos
	this->addGameObjectToRoom(true, false, 1, 20, &m_models->at(20), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));
}

void TristansRoom::onCompleted()
{

}

TristansRoom::TristansRoom()
{
	Room::initParent();
	this->m_entrencePosition = { 0, 10 , 2, 1 };
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
