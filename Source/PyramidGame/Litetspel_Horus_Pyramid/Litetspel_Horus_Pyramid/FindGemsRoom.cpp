#include "pch.h"
#include "FindGemsRoom.h"

void FindGemsRoom::createBoundingBoxes()
{
	this->addBoundingBox({ 30.5, 0, 0, 1 }, DirectX::XMFLOAT3(2, 15, 35));   // Right wall
	this->addBoundingBox({ -30.5, 0, 0, 1 }, DirectX::XMFLOAT3(2, 15, 35));  // Left wall

	this->addBoundingBox({ 0, 0, -31, 1 }, DirectX::XMFLOAT3(30, 15, 2));    // Back wall
	this->addBoundingBox({ 0, 4, -29.5, 1 }, DirectX::XMFLOAT3(5, 6, 2));    // Portal wall

	this->addBoundingBox({ 20, 0, 31, 1 }, DirectX::XMFLOAT3(13, 15, 2));    // Front wall right
	this->addBoundingBox({ -20, 0, 31, 1 }, DirectX::XMFLOAT3(13, 15, 2));   // Front wall left

	this->addBoundingBox({ 0, 4, 50, 1 }, DirectX::XMFLOAT3(10, 6, 2));      // Small room back wall
	this->addBoundingBox({ 9.5, 2, 41, 1 }, DirectX::XMFLOAT3(2, 7, 10));    // Small room right wall
	this->addBoundingBox({ -9.5, 2, 41, 1 }, DirectX::XMFLOAT3(2, 7, 10));   // Small room left wall

	this->addBoundingBox({ 0, 5, 0, 1 }, DirectX::XMFLOAT3(4.5, 7, 4.5));    // Center pillar
	this->addBoundingBox({ 15, 1, 0, 1 }, DirectX::XMFLOAT3(2, 2, 4.5));     // Sarcophagus right
	this->addBoundingBox({ -15, 1, 0, 1 }, DirectX::XMFLOAT3(2, 2, 4.5));    // Sarcophagus left

	this->addBoundingBox({ 14.6, 6, 22.2, 1 }, DirectX::XMFLOAT3(2, 12, 2));   // Pillar front right
	this->addBoundingBox({ -14.6, 6, 22.2, 1 }, DirectX::XMFLOAT3(2, 12, 2));  // Pillar front left
	this->addBoundingBox({ 14.6, 6, -22.2, 1 }, DirectX::XMFLOAT3(2, 12, 2));   // Pillar back right
	this->addBoundingBox({ -14.6, 6, -22.2, 1 }, DirectX::XMFLOAT3(2, 12, 2));  // Pillar back left

	this->addBoundingBox({ 18, 0, -23, 1 }, DirectX::XMFLOAT3(1, 1, 1.5));  // Chest "Right side"

}

void FindGemsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	int nrOfCurrentLModels = 26; // Get the real number later

	// Room
	pos = DirectX::XMVectorSet(0, -1, 0, 1);
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Trasure Gate
	pos = DirectX::XMVectorSet(0, 5, 34, 1);
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Pedistal for gems
	pos = DirectX::XMVectorSet(0, -4, 50, 1);
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Leaver top part
	pos = DirectX::XMVectorSet(0, -1.5, 43.1, 1);
	rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(5, 10, 2)); // fix bounding box so it match my lever
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	m_Lever = dynamic_cast<Lever*>(m_gameObjects.back());

	// -- 5 gems --

	// Gem 1
	pos = DirectX::XMVectorSet(-28, -2, 26, 1);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 2
	pos = DirectX::XMVectorSet(0, 1, 50, 1);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 3
	pos = DirectX::XMVectorSet(0, 2, 50, 1);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 4
	pos = DirectX::XMVectorSet(0, 3, 50, 1);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 5
	pos = DirectX::XMVectorSet(0, 4, 50, 1);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
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
	
	this->m_entrencePosition = { 0, 0, -26.5, 1 }; //


	// Init Gems spawns, 8
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(18.5, -0.5, -23.5, 1));  // Chest
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(17.5, -0.5, 22.5, 1));   // Front Right Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-13, -0.9, 19.6, 1));    // Front Left Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-17.5, -0.8, -3, 1));    // Left Sarcophagus
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-4.2, -0.8, -26.9, 1));  // Next to portal
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(28, -2, -25, 1));        // Bottom right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(25, -2, 28.5, 1));		  // Front right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-28, -2, 26, 1));        // Front left moat
}

void FindGemsRoom::portals()
{
}
