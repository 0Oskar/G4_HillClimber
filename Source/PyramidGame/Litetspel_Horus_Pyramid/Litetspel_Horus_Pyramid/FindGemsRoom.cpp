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
	pos = this->gatePos;
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->gate = this->m_gameObjects.back();

	// Leaver top part
	pos = DirectX::XMVectorSet(0, -1.5, 43.1, 1) + DirectX::XMVectorSet(1.2, 0, 0, 0); // extra offset bc of Rot
	rot = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(45), 0.f);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 1, 1)); // fix bounding box so it match my lever
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	m_Lever = dynamic_cast<Lever*>(m_gameObjects.back());
	rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// -- 5 gems --
	std::vector<int> randomIntList = { 0,1,2,3,4,5,6,7 };
	std::random_shuffle(randomIntList.begin(), randomIntList.end());

	// Gem 1
	pos = m_PreFixedSpawnpoints.at(randomIntList[0]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 2
	pos = m_PreFixedSpawnpoints.at(randomIntList[1]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 3
	pos = m_PreFixedSpawnpoints.at(randomIntList[2]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 4
	pos = m_PreFixedSpawnpoints.at(randomIntList[3]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 5
	pos = m_PreFixedSpawnpoints.at(randomIntList[4]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 2)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Pedistal for gems
	++nrOfCurrentLModels;
	pos = m_PreFixedSpawnpoints[5] + DirectX::XMVectorSet(0, -1.5, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[6] + DirectX::XMVectorSet(0, -1.5, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[7] + DirectX::XMVectorSet(0, -1.5, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	
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

	//this->m_Gems[0]->collidesWithPlayer();

	//this->m_Lever->collidesWithPlayer(); <----- game crashes
	if (this->m_Lever->getCanUseLever() == true) 
	{
		this->m_Lever->setRotation(DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-45), 0.f));
		this->m_Lever->setPosition(DirectX::XMVectorSet(0, -1.5, 43.1, 1) + DirectX::XMVectorSet(-1.2, 0, 0, 0));
	}

	if (this->m_MoveGateUp) // If all 5 gems in center pillar
	{
		//this->m_MoveGateUp = true;
		if (XMVectorGetY(this->gatePos) >= 12.f) //pos: 0, 5, 34, 1
		{
			this->gate->getMoveCompPtr()->position = this->gate->getMoveCompPtr()->position + DirectX::XMVectorSet(0, 3 * dt, 0, 0);
		
		}
	}



}

void FindGemsRoom::onEntrance()
{
}

void FindGemsRoom::init()
{
	this->m_entrencePosition = { 0, 0, -26.5, 1 };
	// Init Gems spawns, 8
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(18.5, -0.5, -23.5, 1));  // Chest
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(17.5, -0.5, 22.5, 1));   // Front Right Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-13, -0.9, 19.6, 1));    // Front Left Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-17.5, -0.8, -3, 1));    // Left Sarcophagus
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-4.2, -0.8, -26.9, 1));  // Next to portal
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(28.2, -3, -25, 1));        // Bottom right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(25, -3, 28.7, 1));		  // Front right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-28.2, -3, 26, 1));        // Front left moat

	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
	
	



}

void FindGemsRoom::portals()
{
}
