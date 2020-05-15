#include "pch.h"
#include "FindGemsRoom.h"

void FindGemsRoom::createBoundingBoxes()
{
}

void FindGemsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	int nrOfCurrentLModels = 20; // Get the real number later

	// Room
	pos = DirectX::XMVectorSet(-30, 3, 0, 1);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	nrOfCurrentLModels++;

	// Trasure Gate
	//pos = DirectX::XMVectorSet(0, 3, 0, 1);
	//this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	//nrOfCurrentLModels++;

	// Pedistal for gems
	/*pos = DirectX::XMVectorSet(0, 3, 0, 1);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	nrOfCurrentLModels++;*/

	// Leaver top part
	//pos = DirectX::XMVectorSet(0, 3, 0, 1);
	//rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	//this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(5, 10, 2)); // fix bounding box so it match my lever
	//nrOfCurrentLModels++;
	//this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	//lever = dynamic_cast<Lever*>(m_gameObjects.back());

	// -- 5 gems --

	// Gem 1
	//pos = DirectX::XMVectorSet(0, 3, 0, 1);
	//this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	//nrOfCurrentLModels++;
	//this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	//this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 2
	//pos = DirectX::XMVectorSet(0, 5, 0, 1);
	//this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	//nrOfCurrentLModels++;
	//this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	//this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 3
	//pos = DirectX::XMVectorSet(0, 7, 0, 1);
	//this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	//nrOfCurrentLModels++;
	//this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	//this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 4
	//pos = DirectX::XMVectorSet(0, 9, 0, 1);
	//this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	//nrOfCurrentLModels++;
	//this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	//this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 5
	//pos = DirectX::XMVectorSet(0, 11, 0, 1);
	//this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	//nrOfCurrentLModels++;
	//this->gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	//this->gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
}


void FindGemsRoom::onCompleted()
{
}

FindGemsRoom::FindGemsRoom()
{
}

FindGemsRoom::~FindGemsRoom()
{
	Room::~Room();
}

void FindGemsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
}

void FindGemsRoom::onEntrance()
{
}

void FindGemsRoom::init()
{
}

void FindGemsRoom::portals()
{
}
