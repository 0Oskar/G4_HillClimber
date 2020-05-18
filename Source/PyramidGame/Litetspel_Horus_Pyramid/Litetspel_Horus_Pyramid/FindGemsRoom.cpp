#include "pch.h"
#include "FindGemsRoom.h"

void FindGemsRoom::createBoundingBoxes()
{
	//this->addBoundingBox({ 0, -1, 0, 1 }, DirectX::XMFLOAT3(20, 2, 20)); // Ground
}

void FindGemsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	int nrOfCurrentLModels = 26; // Get the real number later

	// Room
	pos = DirectX::XMVectorSet(0, 0, 0, 1);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	++nrOfCurrentLModels;

	// Trasure Gate
	pos = DirectX::XMVectorSet(0, 7, -34, 1);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	++nrOfCurrentLModels;

	// Pedistal for gems
	pos = DirectX::XMVectorSet(0, -2, -50, 1);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	++nrOfCurrentLModels;

	// Leaver top part
	pos = DirectX::XMVectorSet(0, 0, -43.1, 1);
	rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(5, 10, 2)); // fix bounding box so it match my lever
	++nrOfCurrentLModels;
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	lever = dynamic_cast<Lever*>(m_gameObjects.back());

	// -- 5 gems --

	// Gem 1
	pos = DirectX::XMVectorSet(0, 0, -50, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	++nrOfCurrentLModels;
	this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 2
	pos = DirectX::XMVectorSet(0, 1, -50, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	++nrOfCurrentLModels;
	this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 3
	pos = DirectX::XMVectorSet(0, 2, -50, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	++nrOfCurrentLModels;
	this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 4
	pos = DirectX::XMVectorSet(0, 3, -50, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	++nrOfCurrentLModels;
	this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 5
	pos = DirectX::XMVectorSet(0, 4, -50, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	++nrOfCurrentLModels;
	this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
}


void FindGemsRoom::onCompleted()
{
}

FindGemsRoom::FindGemsRoom()
{
	Room::initParent();
}

FindGemsRoom::~FindGemsRoom()
{
	Room::~Room();
}

void FindGemsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);
}

void FindGemsRoom::onEntrance()
{
}

void FindGemsRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
	
	this->m_entrencePosition = { 0, 0, 0, 1 }; //
}

void FindGemsRoom::portals()
{
}
