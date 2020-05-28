#include "pch.h"
#include "FindGemsRoom.h"

void FindGemsRoom::createBoundingBoxes()
{
	this->addBoundingBox({ 30.5, 0, 0, 1 }, DirectX::XMFLOAT3(2, 15, 35));   // Right wall
	this->addBoundingBox({ -30.5, 0, 0, 1 }, DirectX::XMFLOAT3(2, 15, 35));  // Left wall

	this->addBoundingBox({ 0, 0, -31, 1 }, DirectX::XMFLOAT3(30, 15, 2));    // Back wall
	this->addBoundingBox({ 0, 4, -29.5, 1 }, DirectX::XMFLOAT3(5, 6, 2));    // Portal wall
	m_PortalBB = &this->m_boundingBoxes.back();


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

	this->addBoundingBox({ 0, 3, 35, 1 }, DirectX::XMFLOAT3(12, 9, 2));
	m_GateBB = &this->m_boundingBoxes.back();
	
}

void FindGemsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	int nrOfCurrentLModels = 26; // Get the real number later

	// Room
	pos = DirectX::XMVectorSet(0, 5.5, 7.8, 1);
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Trasure Gate
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), this->gatePos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->gate = this->m_gameObjects.back();

	// Lever top part
	pos = DirectX::XMVectorSet(0, -1.5, 43.1, 1) + DirectX::XMVectorSet(1.2, 0, 0, 0); // extra offset bc of Rot
	rot = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(45), 0.f);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 3, 1)); // fix bounding box so it match my lever
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	this->m_Lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	this->m_Lever[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// -- 5 gems --
	// Gem 1
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[0]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5, 3.5, 2.5)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	DirectX::XMFLOAT3 gemPos = XMLoadFloat3(this->m_Gems.at(0)->getPosition());  // FIX
	this->m_Gems.at(0)->setBoundingBox(gemPos - DirectX::XMVectorSet(0.f, -20.f, 0.f, 1.f));
	

	// Gem 2
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[1]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5, 3.5, 2.5)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(1)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 3
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[2]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5, 3.5, 2.5)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(2)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 4
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[3]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5, 3.5, 2.5)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(3)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem 5
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[4]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5, 3.5, 2.5)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_Gems.at(4)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Gem Slots
	/* */
	pos = DirectX::XMVectorSet(0.8, 3, 2.5, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 4)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_gemSlots.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
	this->m_gemSlots.at(0)->setScale(DirectX::XMVectorSet(0.01, 0.01, 0.01, 1)); // 0.01

	pos = DirectX::XMVectorSet(-0.8, 1.2, -2.5, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 4)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_gemSlots.at(1)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
	this->m_gemSlots.at(1)->setScale(DirectX::XMVectorSet(0.01, 0.01, 0.01, 1)); // 0.01

	pos = DirectX::XMVectorSet(2.5, 1, -1.9, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(4, 2, 2)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_gemSlots.at(2)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
	this->m_gemSlots.at(2)->setScale(DirectX::XMVectorSet(0.01, 0.01, 0.01, 1)); // 0.01

	pos = DirectX::XMVectorSet(2.5, 4.7, 0.8, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(4, 2, 2)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_gemSlots.at(3)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
	this->m_gemSlots.at(3)->setScale(DirectX::XMVectorSet(0.01, 0.01, 0.01, 1)); // 0.01

	pos = DirectX::XMVectorSet(-2.5, 3, -0.85, 1);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(4, 2, 2)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->m_gemSlots.at(4)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB
	this->m_gemSlots.at(4)->setScale(DirectX::XMVectorSet(0.01, 0.01, 0.01, 1)); // 0.01
	// */

	// Pedistal for gems
	++nrOfCurrentLModels;
	pos = m_PreFixedSpawnpoints[5] + DirectX::XMVectorSet(0, -3, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[6] + DirectX::XMVectorSet(0, -3, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[7] + DirectX::XMVectorSet(0, -3, 0, 0);
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

	// Activate Inventory UI
	if (this->m_gemInPlace == -1) {
		this->m_gemInPlace = 0;
	}

	// Pick up gem
	this->m_Gems[0]->collidesWithPlayer();
	if (this->m_Gems[0]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[0]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 1;
	}
	this->m_Gems[1]->collidesWithPlayer();
	if (this->m_Gems[1]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[1]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 2;
	}
	this->m_Gems[2]->collidesWithPlayer();
	if (this->m_Gems[2]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[2]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 3;
	}
	this->m_Gems[3]->collidesWithPlayer();
	if (this->m_Gems[3]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[3]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 4;
	}
	this->m_Gems[4]->collidesWithPlayer();
	if (this->m_Gems[4]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[4]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 5;
	}

	// Place Gem in Slot
	this->m_gemSlots[0]->collidesWithPlayer();
	if (this->m_gemSlots[0]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_0 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[0]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(75), 0.f, 0.f, 1.f));
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_0 = true;
	}
	this->m_gemSlots[1]->collidesWithPlayer();
	if (this->m_gemSlots[1]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_1 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[1]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(180), pMath::convertDegreesToRadians(180), pMath::convertDegreesToRadians(180), 0.f));
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_1 = true;
	}
	this->m_gemSlots[2]->collidesWithPlayer();
	if (this->m_gemSlots[2]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_2 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[2]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(75), pMath::convertDegreesToRadians(90), 0.f, 1.f));
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_2 = true;
	}
	this->m_gemSlots[3]->collidesWithPlayer();
	if (this->m_gemSlots[3]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_3 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[3]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(75), pMath::convertDegreesToRadians(90), 0.f, 1.f));
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_3 = true;
	}
	this->m_gemSlots[4]->collidesWithPlayer();
	if (this->m_gemSlots[4]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_4 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[4]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(75), pMath::convertDegreesToRadians(-90), 0.f, 1.f));
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_4 = true;
	}

	// Check gate
	if (this->m_GameSlotFilled_0 == true && this->m_GameSlotFilled_1 == true && this->m_GameSlotFilled_2 == true && this->m_GameSlotFilled_3 == true && this->m_GameSlotFilled_4 == true) {
		this->m_MoveGateUp = true;
	}

	// Use Lever
	this->m_Lever[0]->collidesWithPlayer(); 
	if (this->m_Lever[0]->getCanUseLever() == true && this->m_player->getinUse() == true)
	{
		this->m_Lever[0]->setPosition(DirectX::XMVectorSet(0, -1.5, 43.1, 1) + this->m_worldPosition + DirectX::XMVectorSet(-1.2, 0, 0, 0));
		this->m_Lever[0]->setRotation(DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-45), 0.f));
		//this->m_portal->setActive(false);  
	}

	// Move gate
	if (this->m_MoveGateUp) // If all 5 gems in center pillar // this->m_gemInPlace.at(0) == true
	{
		if (XMVectorGetY(this->gatePos) <= 20.f) //pos: 0, 5, 34, 1
		{
			this->gate->getMoveCompPtr()->position = this->gate->getMoveCompPtr()->position + DirectX::XMVectorSet(0, 3 * dt, 0, 0);
			this->gatePos += XMVectorSetY(this->gatePos, 3 * dt);
			this->m_gemInPlace = -1;
			this->m_GateBB->Center.y = XMVectorGetY(this->gatePos);
		}
	}
}

void FindGemsRoom::onEntrance()
{
	//this->m_portal->setActive(false);
}


std::wstring FindGemsRoom::getRoomUITexturePath()
{
	std::wstring temp = L"";
	if (this->m_gemInPlace == -1) {
		temp = L"";
	}
	else if (this->m_gemInPlace == 0) {
		 temp = L"GemsUI_null";
	}
	else if (this->m_gemInPlace == 1) {
		temp = L"GemsUI_1";
	}
	else if (this->m_gemInPlace == 2) {
		temp = L"GemsUI_2";
	}
	else if (this->m_gemInPlace == 3) {
		temp = L"GemsUI_3";
	}
	else if (this->m_gemInPlace == 4) {
		temp = L"GemsUI_4";
	}
	else if (this->m_gemInPlace == 5) {
		temp = L"GemsUI_5";
	}
	return temp;
}

void FindGemsRoom::init()
{	
	this->m_entrencePosition = { 0, 0, -25, 1 };
	this->m_holdingGem = false;
	this->m_gemInPlace = -1;
	this->m_GameSlotFilled_0 = false;
	this->m_GameSlotFilled_1 = false;
	this->m_GameSlotFilled_2 = false;
	this->m_GameSlotFilled_3 = false;
	this->m_GameSlotFilled_4 = false;
	
	srand(time(0)); // Rand the Index for the pre fixed spawnpoints
	for (int i = 0; i < 8; i++)
	{
		unsigned int index = rand() % 8;
		std::swap(m_SpawnIndex[i], m_SpawnIndex[index]);	
	}
	
	// Init Gems spawns, 8
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(18.5, -0.8, -23.5, 1));  // Chest
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(17.5, -0.4, 22.5, 1));   // Front Right Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-13, -0.8, 19.6, 1));    // Front Left Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-17.5, -0.4, -3, 1));    // Left Sarcophagus
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-4.2, -0.6, -26.9, 1));  // Next to portal
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(28.2, -2, -25, 1));        // Bottom right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(25, -2, 28.7, 1));		  // Front right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-28.2, -2, 26, 1));        // Front left moat

	this->createSceneObjects();
	this->createBoundingBoxes();
	//this->portals();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
	
	



}

void FindGemsRoom::portals()
{

	// Portal - "Index in GameState = 10"
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 3.5, -31, 1);
	DirectX::XMVECTOR pos2 = DirectX::XMVectorSet(0, 0, 0, 1);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(180), 0.f, 0.f);
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);

	this->addPortalToRoom(pos2, 10, &m_models->at(10), pos, scale, DirectX::XMFLOAT3(5, 6, 4), 0, true);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	this->m_portal = dynamic_cast<Portal*>(this->m_gameObjects.back());
	//this->addGameObjectToRoom(true, false, 1, 10, &m_models->at(10), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
}
