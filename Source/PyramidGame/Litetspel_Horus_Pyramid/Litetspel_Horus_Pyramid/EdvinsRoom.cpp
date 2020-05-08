#include "pch.h"
#include "EdvinsRoom.h"

void EdvinsRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
}

void EdvinsRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);


	// PuzzleRoom 
	vec = DirectX::XMVectorSet(0, 3, 0, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 8, &m_models->at(8), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	
	//Buttons
	// Button 1
	vec = DirectX::XMVectorSet(-40, -1, 10, 10); //world pos
	this->addGameObjectToRoom(true, false, 1, 9, &m_models->at(9), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Button 2
	vec = DirectX::XMVectorSet(-40, -1, 5, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 9, &m_models->at(9), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Button 3
	vec = DirectX::XMVectorSet(-40, -1, 0, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 9, &m_models->at(9), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Button 4
	vec = DirectX::XMVectorSet(-40, -1, -5, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 9, &m_models->at(9), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Button 5
	vec = DirectX::XMVectorSet(-40, -1, -10, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 9, &m_models->at(9), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	
	//Bricks
	// Brick 1
	vec = DirectX::XMVectorSet(-10, 20, 15, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 11, &m_models->at(11), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Brick 2
	vec = DirectX::XMVectorSet(-10, 20, 8, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 12, &m_models->at(12), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Brick 3
	vec = DirectX::XMVectorSet(-10, 20, 1, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 13, &m_models->at(13), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Brick 4
	vec = DirectX::XMVectorSet(-10, 20, -6, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 14, &m_models->at(14), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Brick 5
	vec = DirectX::XMVectorSet(-10, 20, -13, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 15, &m_models->at(15), vec, NormalScale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

}

void EdvinsRoom::onCompleted()
{
}

//Public:

EdvinsRoom::EdvinsRoom()
{
	Room::initParent();
	this->m_entrencePosition = { -60, 5, 0, 1 };
}

EdvinsRoom::~EdvinsRoom()
{
	Room::~Room();
}

void EdvinsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);
}

void EdvinsRoom::onEntrance()
{
}

void EdvinsRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
}

void EdvinsRoom::portals()
{
}
