#include "pch.h"
#include "FindGemsRoom.h"

void FindGemsRoom::createBoundingBoxes()
{
	addBoundingBox({ 0.f, -1.f, 0.f, 1.f }, DirectX::XMFLOAT3(30.f, 1.f, 55.f));	    // Ground
	addBoundingBox({ 0.f, 18.f, 0.f, 1.f }, DirectX::XMFLOAT3(34.f, 1.f, 55.f));	    // 
	addBoundingBox({ 30.5f, 0.f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 15.f, 35.f));     // Right wall
	addBoundingBox({ -30.5f, 0.f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 15.f, 35.f));    // Left wall

	addBoundingBox({ 0.f, 0.f, -31.f, 1.f }, DirectX::XMFLOAT3(30.f, 15.f, 2.f));     // Back wall
	addBoundingBox({ 0.f, 4.f, -29.5f, 1.f }, DirectX::XMFLOAT3(5.f, 6.f, 2.f));      // Portal wall
	m_PortalBB = &m_boundingBoxes.back();


	addBoundingBox({ 20.f, 0.f, 31.f, 1.f }, DirectX::XMFLOAT3(13.f, 15.f, 2.f));     // Front wall right
	addBoundingBox({ -20.f, 0.f, 31.f, 1.f }, DirectX::XMFLOAT3(13.f, 15.f, 2.f));    // Front wall left

	addBoundingBox({ 0.f, 4.f, 50.f, 1.f }, DirectX::XMFLOAT3(10.f, 6.f, 2.f));       // Small room back wall
	addBoundingBox({ 0.f, 10.f, 45.f, 1.f }, DirectX::XMFLOAT3(10.f, 1.f, 10.f));     // Small room roof
	addBoundingBox({ 9.5f, 2.f, 41, 1.f }, DirectX::XMFLOAT3(2.f, 7.f, 10.f));        // Small room right wall
	addBoundingBox({ -9.5f, 2.f, 41.f, 1.f }, DirectX::XMFLOAT3(2.f, 7.f, 10.f));     // Small room left wall

	addBoundingBox({ 0.f, 5.f, 0.f, 1.f }, DirectX::XMFLOAT3(4.5, 7.f, 4.5f));        // Center pillar
	addBoundingBox({ 15.f, 1.2f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 2.4f, 4.5f));       // Sarcophagus right
	addBoundingBox({ -15, 1.2f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 2.4f, 4.5f));        // Sarcophagus left

	addBoundingBox({ 14.6f, 6.f, 22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));    // Pillar front right
	addBoundingBox({ -14.6f, 6.f, 22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));   // Pillar front left
	addBoundingBox({ 14.6f, 6.f, -22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));   // Pillar back right
	addBoundingBox({ -14.6f, 6.f, -22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));  // Pillar back left

	addBoundingBox({ 18.f, 0.f, -23.f, 1.f }, DirectX::XMFLOAT3(1.f, 1.f, 1.5f));     // Chest "Right side"

	addBoundingBox({ 0.f, 3.f, 35.f, 1.f }, DirectX::XMFLOAT3(12.f, 9.f, 2.f));       // Gate
	m_GateBB = &m_boundingBoxes.back();
}

void FindGemsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	int nrOfCurrentLModels = 36; // Get the real number later

	// Room
	pos = DirectX::XMVectorSet(0.f, 5.5f, 7.8f, 1.f);
	++nrOfCurrentLModels;
	addGameObjectToRoom(true, false, 1, &m_models->at("vRoom_New2.bff"), pos, scale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Trasure Gate
	++nrOfCurrentLModels;
	addGameObjectToRoom(true, false, 1, &m_models->at("vGate_New.bff"), gatePos, scale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	gate = m_gameObjects.back();

	// Lever top part
	pos = DirectX::XMVectorSet(0, -1.5f, 43.1f, 1.f) + DirectX::XMVectorSet(1.2f, 0.f, 0.f, 0.f); // extra offset bc of Rot
	rot = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(45.0f), 0.f);
	++nrOfCurrentLModels;
	addLeverToRoom(&m_models->at("vLever_New.bff"), pos, rot, XMFLOAT3(2.f, 3.f, 1.f)); // fix bounding box so it match my lever
	m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	m_Lever.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	m_Lever[0]->setPlayerBoundingBox(m_player->getAABBPtr());
	rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// -- 5 gems --
	// Gem 1
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[0]);
	++nrOfCurrentLModels;
	addLeverToRoom(&m_models->at("vGem1_New.bff"), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	m_Gems.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_Gems.at(0)->setPlayerBoundingBox(m_player->getAABBPtr());

	// Gem 2
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[1]);
	++nrOfCurrentLModels;
	addLeverToRoom(&m_models->at("vGem2_New.bff"), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	m_Gems.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_Gems.at(1)->setPlayerBoundingBox(m_player->getAABBPtr());

	// Gem 3
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[2]);
	++nrOfCurrentLModels;
	addLeverToRoom(&m_models->at("vGem3_New.bff"), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	m_Gems.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_Gems.at(2)->setPlayerBoundingBox(m_player->getAABBPtr());

	// Gem 4
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[3]);
	++nrOfCurrentLModels;
	addLeverToRoom(&m_models->at("vGem4_New.bff"), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	m_Gems.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_Gems.at(3)->setPlayerBoundingBox(m_player->getAABBPtr()); 

	// Gem 5
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[4]);
	++nrOfCurrentLModels;
	addLeverToRoom(&m_models->at("vGem5_New.bff"), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	m_Gems.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_Gems.at(4)->setPlayerBoundingBox(m_player->getAABBPtr()); 

	// Gem Slots
	pos = DirectX::XMVectorSet(0.8f, 2.9f, 2.5f, 1.f);
	addLeverToRoom(&m_models->at("vGem5_New.bff"), pos, rot, XMFLOAT3(2.f, 2.f, 4.f)); // fix bounding box so it match the gems
	m_gemSlots.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_gemSlots.at(0)->setPlayerBoundingBox(m_player->getAABBPtr()); 
	m_gemSlots.at(0)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); // 0.01

	pos = DirectX::XMVectorSet(-0.8f, 1.1f, -2.5f, 1.f);
	addLeverToRoom(&m_models->at("vGem5_New.bff"), pos, rot, XMFLOAT3(2, 2, 4)); // fix bounding box so it match the gems
	m_gemSlots.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_gemSlots.at(1)->setPlayerBoundingBox(m_player->getAABBPtr()); 
	m_gemSlots.at(1)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 

	pos = DirectX::XMVectorSet(2.5f, 0.9f, -1.9f, 1.f);
	addLeverToRoom(&m_models->at("vGem5_New.bff"), pos, rot, XMFLOAT3(4, 2, 2)); // fix bounding box so it match the gems
	m_gemSlots.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_gemSlots.at(2)->setPlayerBoundingBox(m_player->getAABBPtr()); 
	m_gemSlots.at(2)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 

	pos = DirectX::XMVectorSet(2.5f, 4.6f, 0.8f, 1.f);
	addLeverToRoom(&m_models->at("vGem5_New.bff"), pos, rot, XMFLOAT3(4.f, 2.f, 2.f)); // fix bounding box so it match the gems
	m_gemSlots.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_gemSlots.at(3)->setPlayerBoundingBox(m_player->getAABBPtr()); 
	m_gemSlots.at(3)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 

	pos = DirectX::XMVectorSet(-2.5f, 2.9f, -0.85f, 1.f);
	addLeverToRoom(&m_models->at("vGem5_New.bff"), pos, rot, XMFLOAT3(4, 2, 2)); // fix bounding box so it match the gems
	m_gemSlots.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); 
	m_gemSlots.at(4)->setPlayerBoundingBox(m_player->getAABBPtr()); 
	m_gemSlots.at(4)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 

	// Pedistal for gems
	++nrOfCurrentLModels;
	pos = m_PreFixedSpawnpoints[5] + DirectX::XMVectorSet(0, -3, 0, 0);
	addGameObjectToRoom(true, false, 1, &m_models->at("vPedistal_New.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[6] + DirectX::XMVectorSet(0, -3, 0, 0);
	addGameObjectToRoom(true, false, 1, &m_models->at("vPedistal_New.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[7] + DirectX::XMVectorSet(0, -3, 0, 0);
	addGameObjectToRoom(true, false, 1, &m_models->at("vPedistal_New.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	
	// Ramp to back room
	++nrOfCurrentLModels;
	pos = DirectX::XMVectorSet(0, -1.1f, 30, 1);
	scale = DirectX::XMVectorSet(1, 1, 1, 1);
	rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	addGameObjectToRoom(true, false, 1, &m_models->at("vRamp_New.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

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
	if (m_gemInPlace == -1) {
		m_gemInPlace = 0;
	}

	// Pick up gem
	m_Gems[0]->collidesWithPlayer();
	if (m_Gems[0]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == false)
	{
		m_Gems[0]->setPosition(m_player->getPosition() + (DirectX::XMVectorSet(0, -100, 0, 0)));
		m_Gems[0]->setBoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
		m_Gems[0]->toggleActivateLever();
		m_holdingGem = true;
		m_gemInPlace = 1;
		
	}
	m_Gems[1]->collidesWithPlayer();
	if (m_Gems[1]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == false)
	{
		m_Gems[1]->setPosition(m_player->getPosition() + (DirectX::XMVectorSet(0, -100, 0, 0)));
		m_Gems[1]->setBoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
		m_Gems[1]->toggleActivateLever();
		m_holdingGem = true;
		m_gemInPlace = 2;
	}
	m_Gems[2]->collidesWithPlayer();
	if (m_Gems[2]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == false)
	{
		m_Gems[2]->setPosition(m_player->getPosition() + (DirectX::XMVectorSet(0, -100, 0, 0)));
		m_Gems[2]->setBoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
		m_Gems[2]->toggleActivateLever();
		m_holdingGem = true;
		m_gemInPlace = 3;
	}
	m_Gems[3]->collidesWithPlayer();
	if (m_Gems[3]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == false)
	{
		m_Gems[3]->setPosition(m_player->getPosition() + (DirectX::XMVectorSet(0, -100, 0, 0)));
		m_Gems[3]->setBoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
		m_Gems[3]->toggleActivateLever();
		m_holdingGem = true;
		m_gemInPlace = 4;
	}
	m_Gems[4]->collidesWithPlayer();
	if (m_Gems[4]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == false)
	{
		m_Gems[4]->setPosition(m_player->getPosition() + (DirectX::XMVectorSet(0, -100, 0, 0)));
		m_Gems[4]->setBoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
		m_Gems[4]->toggleActivateLever();
		m_holdingGem = true;
		m_gemInPlace = 5;
	}

	// Place Gem in Slot
	m_gemSlots[0]->collidesWithPlayer();
	if (m_gemSlots[0]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == true && m_GameSlotFilled_0 == false)
	{
		m_Gems[(int)m_gemInPlace - 1]->setPosition(m_gemSlots[0]->getPosition());
		m_Gems[(int)m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		m_Gems[(int)m_gemInPlace - 1]->setIfCollidable(false);
		m_Gems[(int)m_gemInPlace - 1]->setCnaUseLever(false);
		m_holdingGem = false;
		m_gemInPlace = 0;
		m_GameSlotFilled_0 = true;
	}
	m_gemSlots[1]->collidesWithPlayer();
	if (m_gemSlots[1]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == true && m_GameSlotFilled_1 == false)
	{
		m_Gems[(int)m_gemInPlace - 1]->setPosition(m_gemSlots[1]->getPosition());
		m_Gems[(int)m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		m_Gems[(int)m_gemInPlace - 1]->setIfCollidable(false);
		m_Gems[(int)m_gemInPlace - 1]->setCnaUseLever(false);
		m_holdingGem = false;
		m_gemInPlace = 0;
		m_GameSlotFilled_1 = true;
	}
	m_gemSlots[2]->collidesWithPlayer();
	if (m_gemSlots[2]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == true && m_GameSlotFilled_2 == false)
	{
		m_Gems[(int)m_gemInPlace - 1]->setPosition(m_gemSlots[2]->getPosition());
		m_Gems[(int)m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		m_Gems[(int)m_gemInPlace - 1]->setIfCollidable(false);
		m_Gems[(int)m_gemInPlace - 1]->setCnaUseLever(false);
		m_holdingGem = false;
		m_gemInPlace = 0;
		m_GameSlotFilled_2 = true;
	}
	m_gemSlots[3]->collidesWithPlayer();
	if (m_gemSlots[3]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == true && m_GameSlotFilled_3 == false)
	{
		m_Gems[(int)m_gemInPlace - 1]->setPosition(m_gemSlots[3]->getPosition());
		m_Gems[(int)m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		m_Gems[(int)m_gemInPlace - 1]->setIfCollidable(false);
		m_Gems[(int)m_gemInPlace - 1]->setCnaUseLever(false);
		m_holdingGem = false;
		m_gemInPlace = 0;
		m_GameSlotFilled_3 = true;
	}
	m_gemSlots[4]->collidesWithPlayer();
	if (m_gemSlots[4]->getCanUseLever() == true && m_player->getinUse() == true && m_holdingGem == true && m_GameSlotFilled_4 == false)
	{
		m_Gems[(int)m_gemInPlace - 1]->setPosition(m_gemSlots[4]->getPosition());
		m_Gems[(int)m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		m_Gems[(int)m_gemInPlace - 1]->setIfCollidable(false);
		m_Gems[(int)m_gemInPlace - 1]->setCnaUseLever(false);
		m_holdingGem = false;
		m_gemInPlace = 0;
		m_GameSlotFilled_4 = true;
	}

	// Check gate
	if (m_GameSlotFilled_0 == true && m_GameSlotFilled_1 == true && m_GameSlotFilled_2 == true && m_GameSlotFilled_3 == true && m_GameSlotFilled_4 == true) {
		m_MoveGateUp = true;
	}

	// Use Lever
	m_Lever[0]->collidesWithPlayer(); 
	if (m_Lever[0]->getCanUseLever() == true && m_player->getinUse() == true)
	{
		//m_Lever[0]->getMoveCompPtr()->rotation = m_Lever[0]->getMoveCompPtr()->rotation + DirectX::XMVectorSet(0, 0, 2 * dt, 0);
		m_Lever[0]->setPosition(DirectX::XMVectorSet(0, -1.5f, 43.1f, 1) + m_worldPosition + DirectX::XMVectorSet(-1.2f, 0, 0, 0));
		m_Lever[0]->setRotation(DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-45), 0.f));
		m_portal->setActiveStatus(true);
		StatusTextHandler::get().sendText("You hear distant rumbles...", 4);
	}

	// Move gate
	if (m_MoveGateUp) // If all 5 gems in center pillar
	{
		if (XMVectorGetY(gatePos) <= 20.f)
		{
			gate->getMoveCompPtr()->position = gate->getMoveCompPtr()->position + DirectX::XMVectorSet(0, 3 * dt, 0, 0);
			gatePos += XMVectorSetY(gatePos, 3 * dt);
			m_gemInPlace = -1;
			m_GateBB->Center.y = XMVectorGetY(gatePos);
		}
	}

	m_currentRoomUIPath = getRoomUITexturePath();
}

void FindGemsRoom::onEntrance()
{
	Room::onEntrance();
	StatusTextHandler::get().sendText("Find and return all five gems \n       Press E to interact", 5);
}

std::wstring FindGemsRoom::getRoomUITexturePath()
{
	std::wstring temp = L"";
	if (m_gemInPlace == -1) {
		temp = L"";
	}
	else if (m_gemInPlace == 0) {
		 temp = L"GemsUI_null";
	}
	else if (m_gemInPlace == 1) {
		temp = L"GemsUI_1";
	}
	else if (m_gemInPlace == 2) {
		temp = L"GemsUI_2";
	}
	else if (m_gemInPlace == 3) {
		temp = L"GemsUI_3";
	}
	else if (m_gemInPlace == 4) {
		temp = L"GemsUI_4";
	}
	else if (m_gemInPlace == 5) {
		temp = L"GemsUI_5";
	}
	return temp;
}

void FindGemsRoom::init()
{	
	m_holdingGem = false;
	m_gemInPlace = -1;
	m_GameSlotFilled_0 = false;
	m_GameSlotFilled_1 = false;
	m_GameSlotFilled_2 = false;
	m_GameSlotFilled_3 = false;
	m_GameSlotFilled_4 = false;
	m_entrencePosition = { 0, 0, -25, 1 };

	srand((unsigned)time(0)); // Rand the Index for the pre fixed spawnpoints
	for (int i = 0; i < 8; i++)
	{
		unsigned int index = rand() % 8;
		std::swap(m_SpawnIndex[i], m_SpawnIndex[index]);	
	}
	
	// Init Gems spawns, 8
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(18.5f, -0.7f, -23.5f, 1));  // Chest
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(17.5f, -0.4f, 22.5f, 1));   // Front Right Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-13, -0.8f, 19.6f, 1));     // Front Left Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-17.5f, -0.4f, -3, 1));     // Left Sarcophagus
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-4.2f, -0.6f, -26.9f, 1));  // Next to portal
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(28.2f, -2, -25, 1));        // Bottom right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(25, -2, 28.7f, 1));		 // Front right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-28.2f, -2, 26, 1));        // Front left moat

	// Gem UI - Test  
	m_gemUI0_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(resourceHandler->getTexture(L"Textures/GemsUI_null.png"));
	m_gemUI1_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(resourceHandler->getTexture(L"Textures/GemsUI_1.png"));
	m_gemUI2_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(resourceHandler->getTexture(L"Textures/GemsUI_2.png"));
	m_gemUI3_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(resourceHandler->getTexture(L"Textures/GemsUI_3.png"));
	m_gemUI4_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(resourceHandler->getTexture(L"Textures/GemsUI_4.png"));
	m_gemUI5_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(resourceHandler->getTexture(L"Textures/GemsUI_5.png"));
	int gemsUI_Xpos = 40;
	int gemsUI_Ypos = (option.height) - 140;
	m_gemUI_Position = DirectX::XMFLOAT2((float)gemsUI_Xpos, (float)gemsUI_Ypos);

	createSceneObjects();
	createBoundingBoxes();
	m_player->addAABBFromVector(&m_boundingBoxes);
	
	m_Gems[0]->setCnaUseLever(true);
	m_Gems[1]->setCnaUseLever(true);
	m_Gems[2]->setCnaUseLever(true);
	m_Gems[3]->setCnaUseLever(true);
	m_Gems[4]->setCnaUseLever(true);

	m_player->setSpawnPosition(m_worldPosition);
	
	//XMFLOAT3 Pos;
	//XMStoreFloat3(&Pos, (m_worldPosition + DirectX::XMVectorSet(0, 0, -20 ,0)));
	//m_PortalLight.plPosition = (Pos);
	//m_PortalLight.att = { 1, 0, 0.5 };
	//m_PortalLight.plDiffuse = { 0,1,0,0 };
	//m_PortalLight.plAmbient = { 0,0,0,1 };

	//m_PortalLight.plRange = 20;

	//int lightIndex = createLight(m_PortalLight);
	//createLight(m_PortalLight.plPosition, m_PortalLight.plRange, m_PortalLight.plAmbient, m_PortalLight.plDiffuse, m_PortalLight.att);
}

void FindGemsRoom::portals()
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 3.5, -31, 1);
	DirectX::XMVECTOR pos2 = DirectX::XMVectorSet(0, 0, 0, 1);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(180), 0.f, 0.f);
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);

	addPortalToRoom(pos2, &m_models->at("PortalGate.bff"), pos, scale, DirectX::XMFLOAT3(5, 6, 4), 0, true);
	m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	m_portal = dynamic_cast<Portal*>(m_gameObjects.back());
	m_portal->setActiveStatus(false);
}

void FindGemsRoom::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{
	if (m_currentRoomUIPath == L"GemsUI_null") {
		spriteBatchPtr->Draw(m_gemUI0_SRV.Get(), m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (m_currentRoomUIPath == L"GemsUI_1") {
		spriteBatchPtr->Draw(m_gemUI1_SRV.Get(), m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (m_currentRoomUIPath == L"GemsUI_2") {
		spriteBatchPtr->Draw(m_gemUI2_SRV.Get(), m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (m_currentRoomUIPath == L"GemsUI_3") {
		spriteBatchPtr->Draw(m_gemUI3_SRV.Get(), m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (m_currentRoomUIPath == L"GemsUI_4") {
		spriteBatchPtr->Draw(m_gemUI4_SRV.Get(), m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (m_currentRoomUIPath == L"GemsUI_5") {
		spriteBatchPtr->Draw(m_gemUI5_SRV.Get(), m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
}
