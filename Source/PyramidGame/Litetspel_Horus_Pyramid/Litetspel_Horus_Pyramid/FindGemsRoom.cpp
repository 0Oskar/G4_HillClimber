#include "pch.h"
#include "FindGemsRoom.h"

void FindGemsRoom::createBoundingBoxes()
{
	this->addBoundingBox({ 0.f, -1.f, 0.f, 1.f }, DirectX::XMFLOAT3(30.f, 1.f, 55.f));   // Ground
	this->addBoundingBox({ 30.5f, 0.f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 15.f, 35.f));   // Right wall
	this->addBoundingBox({ -30.5f, 0.f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 15.f, 35.f));  // Left wall

	this->addBoundingBox({ 0.f, 0.f, -31.f, 1.f }, DirectX::XMFLOAT3(30.f, 15.f, 2.f));    // Back wall
	this->addBoundingBox({ 0.f, 4.f, -29.5f, 1.f }, DirectX::XMFLOAT3(5.f, 6.f, 2.f));    // Portal wall
	m_PortalBB = &this->m_boundingBoxes.back();


	this->addBoundingBox({ 20.f, 0.f, 31.f, 1.f }, DirectX::XMFLOAT3(13.f, 15.f, 2.f));    // Front wall right
	this->addBoundingBox({ -20.f, 0.f, 31.f, 1.f }, DirectX::XMFLOAT3(13.f, 15.f, 2.f));   // Front wall left

	this->addBoundingBox({ 0.f, 4.f, 50.f, 1.f }, DirectX::XMFLOAT3(10.f, 6.f, 2.f));      // Small room back wall
	this->addBoundingBox({ 0.f, 10.f, 45.f, 1.f }, DirectX::XMFLOAT3(10.f, 1.f, 10.f));      // Small room roof
	this->addBoundingBox({ 9.5f, 2.f, 41, 1.f }, DirectX::XMFLOAT3(2.f, 7.f, 10.f));    // Small room right wall
	this->addBoundingBox({ -9.5f, 2.f, 41.f, 1.f }, DirectX::XMFLOAT3(2.f, 7.f, 10.f));   // Small room left wall

	this->addBoundingBox({ 0.f, 5.f, 0.f, 1.f }, DirectX::XMFLOAT3(4.5, 7.f, 4.5f));    // Center pillar
	this->addBoundingBox({ 15.f, 1.f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 2.f, 4.5f));     // Sarcophagus right
	this->addBoundingBox({ -15, 1.f, 0.f, 1.f }, DirectX::XMFLOAT3(2.f, 2.f, 4.5f));    // Sarcophagus left

	this->addBoundingBox({ 14.6f, 6.f, 22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));   // Pillar front right
	this->addBoundingBox({ -14.6f, 6.f, 22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));  // Pillar front left
	this->addBoundingBox({ 14.6f, 6.f, -22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));   // Pillar back right
	this->addBoundingBox({ -14.6f, 6.f, -22.2f, 1.f }, DirectX::XMFLOAT3(2.f, 12.f, 2.f));  // Pillar back left

	this->addBoundingBox({ 18.f, 0.f, -23.f, 1.f }, DirectX::XMFLOAT3(1.f, 1.f, 1.5f));  // Chest "Right side"

	this->addBoundingBox({ 0.f, 3.f, 35.f, 1.f }, DirectX::XMFLOAT3(12.f, 9.f, 2.f)); // Gate
	m_GateBB = &this->m_boundingBoxes.back();

	//this->addBoundingBox({ 0, -2, 33, 1 }, DirectX::XMFLOAT3(4, 1, 4)); // Ramp
	
}

void FindGemsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	int nrOfCurrentLModels = 35; // Get the real number later

	// Room
	pos = DirectX::XMVectorSet(0.f, 5.5f, 7.8f, 1.f);
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Trasure Gate
	++nrOfCurrentLModels;
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), this->gatePos, scale, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->gate = this->m_gameObjects.back();

	// Lever top part
	pos = DirectX::XMVectorSet(0, -1.5f, 43.1f, 1.f) + DirectX::XMVectorSet(1.2f, 0.f, 0.f, 0.f); // extra offset bc of Rot
	rot = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(45.0f), 0.f);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.f, 3.f, 1.f)); // fix bounding box so it match my lever
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	this->m_Lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	this->m_Lever[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// -- 5 gems --
	// Gem 1
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[0]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_Gems.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr());

	//DirectX::XMFLOAT3 gemPos;
	//XMStoreFloat3(&gemPos, this->m_Gems.at(0)->getPosition());  // FIX
	//this->m_Gems.at(0)->setBoundingBox(gemPos - DirectX::XMFLOAT3(0, 5, 0));
	

	// Gem 2
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[1]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_Gems.at(1)->setPlayerBoundingBox(this->m_player->getAABBPtr());

	// Gem 3
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[2]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_Gems.at(2)->setPlayerBoundingBox(this->m_player->getAABBPtr());

	// Gem 4
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[3]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_Gems.at(3)->setPlayerBoundingBox(this->m_player->getAABBPtr()); 

	// Gem 5
	pos = m_PreFixedSpawnpoints.at(m_SpawnIndex[4]);
	++nrOfCurrentLModels;
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.5f, 3.5f, 2.5f)); // fix bounding box so it match the gems
	this->m_Gems.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_Gems.at(4)->setPlayerBoundingBox(this->m_player->getAABBPtr()); 

	// Gem Slots
	/* */
	pos = DirectX::XMVectorSet(0.8f, 3.f, 2.5f, 1.f);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2.f, 2.f, 4.f)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_gemSlots.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); 
	this->m_gemSlots.at(0)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); // 0.01

	pos = DirectX::XMVectorSet(-0.8f, 1.2f, -2.5f, 1.f);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(2, 2, 4)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_gemSlots.at(1)->setPlayerBoundingBox(this->m_player->getAABBPtr()); 
	this->m_gemSlots.at(1)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 

	pos = DirectX::XMVectorSet(2.5f, 1.f, -1.9f, 1.f);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(4, 2, 2)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_gemSlots.at(2)->setPlayerBoundingBox(this->m_player->getAABBPtr()); 
	this->m_gemSlots.at(2)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 

	pos = DirectX::XMVectorSet(2.5f, 4.7f, 0.8f, 1.f);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(4.f, 2.f, 2.f)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_gemSlots.at(3)->setPlayerBoundingBox(this->m_player->getAABBPtr()); 
	this->m_gemSlots.at(3)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 

	pos = DirectX::XMVectorSet(-2.5f, 3.f, -0.85f, 1.f);
	this->addLeverToRoom(nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, rot, XMFLOAT3(4, 2, 2)); // fix bounding box so it match the gems
	this->m_gemSlots.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); 
	this->m_gemSlots.at(4)->setPlayerBoundingBox(this->m_player->getAABBPtr()); 
	this->m_gemSlots.at(4)->setScale(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 1.f)); 
	// */

	// Pedistal for gems
	++nrOfCurrentLModels;
	pos = m_PreFixedSpawnpoints[5] + DirectX::XMVectorSet(0, -3, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[6] + DirectX::XMVectorSet(0, -3, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	pos = m_PreFixedSpawnpoints[7] + DirectX::XMVectorSet(0, -3, 0, 0);
	this->addGameObjectToRoom(true, false, 1, nrOfCurrentLModels, &m_models->at(nrOfCurrentLModels), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	
	// Ramp to back room
	++nrOfCurrentLModels;
	pos = DirectX::XMVectorSet(0, -1.1f, 30, 1);
	scale = DirectX::XMVectorSet(1, 1, 1, 1);
	rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
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
	//if(m_Gems[0]->collidable())
	this->m_Gems[0]->collidesWithPlayer();

	if (this->m_Gems[0]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[0]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 1;
	}
	//if (m_Gems[1]->collidable())
	this->m_Gems[1]->collidesWithPlayer();

	if (this->m_Gems[1]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[1]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 2;
	}
	//if (m_Gems[2]->collidable())
	this->m_Gems[2]->collidesWithPlayer();

	if (this->m_Gems[2]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[2]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 3;
	}
	//if (m_Gems[3]->collidable())
	this->m_Gems[3]->collidesWithPlayer();

	if (this->m_Gems[3]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == false)
	{
		this->m_Gems[3]->setPosition(this->m_player->getPosition() + (DirectX::XMVectorSet(0, -10, 0, 0)));
		this->m_holdingGem = true;
		this->m_gemInPlace = 4;
	}
	//if (m_Gems[4]->collidable())
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
		this->m_Gems[(int)this->m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setIfCollidable(false);
		this->m_Gems[(int)this->m_gemInPlace - 1]->setCnaUseLever(false);
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_0 = true;
	}
	this->m_gemSlots[1]->collidesWithPlayer();
	if (this->m_gemSlots[1]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_1 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[1]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(180), pMath::convertDegreesToRadians(180), pMath::convertDegreesToRadians(180), 0.f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setIfCollidable(false);
		this->m_Gems[(int)this->m_gemInPlace - 1]->setCnaUseLever(false);
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_1 = true;
	}
	this->m_gemSlots[2]->collidesWithPlayer();
	if (this->m_gemSlots[2]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_2 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[2]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(75), pMath::convertDegreesToRadians(90), 0.f, 1.f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setIfCollidable(false);
		this->m_Gems[(int)this->m_gemInPlace - 1]->setCnaUseLever(false);
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_2 = true;
	}
	this->m_gemSlots[3]->collidesWithPlayer();
	if (this->m_gemSlots[3]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_3 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[3]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(75), pMath::convertDegreesToRadians(90), 0.f, 1.f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setIfCollidable(false);
		this->m_Gems[(int)this->m_gemInPlace - 1]->setCnaUseLever(false);
		this->m_holdingGem = false;
		this->m_gemInPlace = 0;
		this->m_GameSlotFilled_3 = true;
	}
	this->m_gemSlots[4]->collidesWithPlayer();
	if (this->m_gemSlots[4]->getCanUseLever() == true && this->m_player->getinUse() == true && this->m_holdingGem == true && this->m_GameSlotFilled_4 == false)
	{
		this->m_Gems[(int)this->m_gemInPlace - 1]->setPosition(m_gemSlots[4]->getPosition());
		this->m_Gems[(int)this->m_gemInPlace - 1]->setRotation(DirectX::XMVectorSet(pMath::convertDegreesToRadians(75), pMath::convertDegreesToRadians(-90), 0.f, 1.f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setBoundingBox(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		this->m_Gems[(int)this->m_gemInPlace - 1]->setIfCollidable(false);
		this->m_Gems[(int)this->m_gemInPlace - 1]->setCnaUseLever(false);
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
		this->m_Lever[0]->setPosition(DirectX::XMVectorSet(0, -1.5f, 43.1f, 1) + this->m_worldPosition + DirectX::XMVectorSet(-1.2f, 0, 0, 0));
		this->m_Lever[0]->setRotation(DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-45), 0.f));
		this->m_portal->setActiveStatus(true);
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
	//this->m_portal->setActiveStatus(false);
	StatusTextHandler::get().sendText("Find and return all five gems", 5);
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
	
	srand((unsigned)time(0)); // Rand the Index for the pre fixed spawnpoints
	for (int i = 0; i < 8; i++)
	{
		unsigned int index = rand() % 8;
		std::swap(m_SpawnIndex[i], m_SpawnIndex[index]);	
	}
	
	// Init Gems spawns, 8
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(18.5f, -0.8f, -23.5f, 1));  // Chest
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(17.5f, -0.4f, 22.5f, 1));   // Front Right Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-13, -0.8f, 19.6f, 1));    // Front Left Pillar
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-17.5f, -0.4f, -3, 1));    // Left Sarcophagus
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-4.2f, -0.6f, -26.9f, 1));  // Next to portal
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(28.2f, -2, -25, 1));        // Bottom right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(25, -2, 28.7f, 1));		  // Front right moat
	m_PreFixedSpawnpoints.push_back(DirectX::XMVectorSet(-28.2f, -2, 26, 1));        // Front left moat

	// Gem UI - Test  
	this->m_gemUI0_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(this->resourceHandler->getTexture(L"Textures/GemsUI_null.png"));
	this->m_gemUI1_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(this->resourceHandler->getTexture(L"Textures/GemsUI_1.png"));
	this->m_gemUI2_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(this->resourceHandler->getTexture(L"Textures/GemsUI_2.png"));
	this->m_gemUI3_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(this->resourceHandler->getTexture(L"Textures/GemsUI_3.png"));
	this->m_gemUI4_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(this->resourceHandler->getTexture(L"Textures/GemsUI_4.png"));
	this->m_gemUI5_SRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>(this->resourceHandler->getTexture(L"Textures/GemsUI_5.png"));
	int gemsUI_Xpos = 40;
	int gemsUI_Ypos = (this->option.height) - 140;
	this->m_gemUI_Position = DirectX::XMFLOAT2((float)gemsUI_Xpos, (float)gemsUI_Ypos);

	this->createSceneObjects();
	this->createBoundingBoxes();
	//this->portals();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
	
	m_Gems[0]->setCnaUseLever(true);
	m_Gems[1]->setCnaUseLever(true);
	m_Gems[2]->setCnaUseLever(true);
	m_Gems[3]->setCnaUseLever(true);
	m_Gems[4]->setCnaUseLever(true);
	
	
	XMFLOAT3 Pos;
	XMStoreFloat3(&Pos, (this->m_worldPosition + DirectX::XMVectorSet(0, 0, -20 ,0)));
	this->m_PortalLight.plPosition = (Pos);
	this->m_PortalLight.att = { 1, 0, 0.5 };
	this->m_PortalLight.plDiffuse = { 0,1,0,0 };
	this->m_PortalLight.plAmbient = { 0,0,0,1 };

	this->m_PortalLight.plRange = 20;

	int lightIndex = this->createLight(this->m_PortalLight);
	this->createLight(this->m_PortalLight.plPosition, this->m_PortalLight.plRange, this->m_PortalLight.plAmbient, this->m_PortalLight.plDiffuse, this->m_PortalLight.att);


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
	this->m_portal->setActiveStatus(false);
	//this->addGameObjectToRoom(true, false, 1, 10, &m_models->at(10), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
}

void FindGemsRoom::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{

	if (this->m_currentRoomUIPath == L"GemsUI_null") {
		spriteBatchPtr->Draw(this->m_gemUI0_SRV.Get(), this->m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (this->m_currentRoomUIPath == L"GemsUI_1") {
		spriteBatchPtr->Draw(this->m_gemUI1_SRV.Get(), this->m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (this->m_currentRoomUIPath == L"GemsUI_2") {
		spriteBatchPtr->Draw(this->m_gemUI2_SRV.Get(), this->m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (this->m_currentRoomUIPath == L"GemsUI_3") {
		spriteBatchPtr->Draw(this->m_gemUI3_SRV.Get(), this->m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (this->m_currentRoomUIPath == L"GemsUI_4") {
		spriteBatchPtr->Draw(this->m_gemUI4_SRV.Get(), this->m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
	else if (this->m_currentRoomUIPath == L"GemsUI_5") {
		spriteBatchPtr->Draw(this->m_gemUI5_SRV.Get(), this->m_gemUI_Position, nullptr, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(0.2f, 0.2f));
	}
}
