#include "pch.h"
#include"KevinsRoom.h"

KevinsRoom::KevinsRoom()
{
	Room::initParent();
}
KevinsRoom::~KevinsRoom()
{
	Room::~Room();
}

void KevinsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);

	if (triggerBB[0].Intersects(this->m_player->getAABB()))
	{
		if (trapActive1 == true)
		{
			dartFly1 = true;
		}
	}

	if (triggerBB[1].Intersects(this->m_player->getAABB()))
	{
		if (trapActive2 == true)
		{
			dartFly2 = true;
		}
	}

	for (int i = 0; i < deathTrapBB.size(); i++)
	{
		if (deathTrapBB[i].Intersects(this->m_player->getAABB()))
		{
			this->m_player->getMoveCompPtr()->position =  this->getRelativePosition(DirectX::XMVectorSet(-20.f, 20.f, -165.f + 140.f, 1.f));
		}
	}


	for (int i = 0; i < dartTrap.size(); i++)
	{
		if (dartTrap[i]->getAABB().Intersects(this->m_player->getAABB()))
		{
			this->m_player->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(-20.f, -50.f, -165.f + 140.f, 1.f));
		}
	}

	if (dartFly1 == true)
	{
		if (dartPosition1 <= 0)
		{
			dartPosition1 = 40.f;
			dartTrap[0]->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(10.f, 27, -80 + 140.f, 0.f));
			dartFly1 = false;
		}
		else
		{
			dartTrap[0]->getMoveCompPtr()->position = dartTrap[0]->getMoveCompPtr()->position + DirectX::XMVectorSet(-40.f * dt, 0, 0, 0.f);
			dartPosition1 -= 40.f * dt;
		}
	}

	if (dartFly2 == true)
	{
		if (dartPosition2 <= 0)
		{
			dartPosition2 = 40.f;
			dartTrap[1]->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(10.f, 27.f, -60.f + 140.f, 0.f));
			dartFly2 = false;
		}
		else
		{
			dartTrap[1]->getMoveCompPtr()->position = dartTrap[1]->getMoveCompPtr()->position + DirectX::XMVectorSet(-40.f * dt, 0, 0, 0.f);
			dartPosition2 -= 40.f * dt;
		}
	}

	//Lever
	this->lever[0]->collidesWithPlayer();

	if (this->lever[0]->getCanUseLever() == true)
	{
		if (this->m_player->getinUse() == true)
		{
			trapActive1 = false;
		}
	}

	this->lever[1]->collidesWithPlayer();
	if (this->lever[1]->getCanUseLever() == true)
	{
		if (this->m_player->getinUse() == true)
		{
			trapActive2 = false;
		}
	}

	this->wonPuzzleObject[0]->collidesWithPlayer();
	if (this->wonPuzzleObject[0]->getCanUseLever() == true)
	{
		
		if (this->m_player->getinUse() == true)
		{
			if(!m_completed)
				this->onCompleted();
		}
	}
}

void KevinsRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);

	this->m_entrencePosition = XMVectorSet(-13, 4, -20, 0);
}

void KevinsRoom::portals()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Add portals here
	vec = DirectX::XMVectorSet(-13.f, 8.f, -30.f, 1.f);
	XMVECTOR vecScale = DirectX::XMVectorSet(1.3f, 1.3f, -1.3f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 10, &m_models->at(10), vec, NormalScale, DirectX::XMFLOAT3(3.f, 8.f, 0.6f), 0, false);
}

void KevinsRoom::onEntrance()
{
	//What should happen when player enter room
}
void KevinsRoom::createBoundingBoxes()
{
	//Add all boundingboxes that are not part of a gameobject(Walls and ground for rooms etc)
	this->addBoundingBox({ -10.f, 1.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 2.5f, 20.f));
	this->addBoundingBox({ -10.f, 1.0f, -154.0f + 140.f, 1}, DirectX::XMFLOAT3(20.f, 2.5f, 20.8f));
	this->addBoundingBox({ -10.f, 11.f, -86.f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 13.f, 1.5f));
	this->addBoundingBox({ -10.f, 23.f, -77.f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 2.5f, 10.8f));
	this->addBoundingBox({ -10.f, 23.f, -45.4f + 140.f, 1}, DirectX::XMFLOAT3(20.f, 2.5f, 11.8f));
	this->addBoundingBox({ 9.5f, 2.f, -120.f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 40.f, 85.f));
	this->addBoundingBox({ -31.f, 2.f, -120.f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 40.f, 85.f));

	triggerBB.emplace_back();
	this->addTriggerBB({ -10.f, 27.f, -80.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
	triggerBB.back() = this->m_triggerBoundingBoxes.back();

	triggerBB.emplace_back();
	this->addTriggerBB({ -10.f, 27.f, -60.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
	triggerBB.back() = this->m_triggerBoundingBoxes.back();

	deathTrapBB.emplace_back();
	this->addTriggerBB({ -10.f, -1.5f, -129.f + 140.f }, DirectX::XMFLOAT3(27.f, 3.f, 4.f));
	deathTrapBB.back() = this->m_triggerBoundingBoxes.back();
	
	deathTrapBB.emplace_back();
	this->addTriggerBB({ -10.f, -0.5f, -63.f + 140.f }, DirectX::XMFLOAT3(27.f, 3.f, 5.f));
	deathTrapBB.back() = this->m_triggerBoundingBoxes.back();

}
void KevinsRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Kevin room platform
	vec = DirectX::XMVectorSet(5.f, 25.f, -85.f + 140.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	//Puzzle Room (Kevins Lever room)
	vec = DirectX::XMVectorSet(-10.f, 2, -100 + 140.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 6, &m_models->at(6), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	//Lever
	vec = DirectX::XMVectorSet(-15.f, 5.f, -88.f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(-90), pMath::convertDegreesToRadians(-270), 1.f);
	this->addLeverToRoom(7, &m_models->at(7), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));

	//Lever
	vec = DirectX::XMVectorSet(7.5f, 5.f, -138.f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-270), 1.f);
	this->addLeverToRoom(7, &m_models->at(7), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));

	//Lever
	vec = DirectX::XMVectorSet(-11.f, 30.f, -39.8f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-270), 1.f);
	this->addLeverToRoom(7, &m_models->at(7), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	//Win object
	wonPuzzleObject.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	wonPuzzleObject[0]->setScale(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));

	vec = DirectX::XMVectorSet(10.f, 27, -80 + 140.f, 1.f);
	this->addGameObjectToRoom(true, false, 2, 3, &m_models->at(3), vec, DirectX::XMVectorSet(.1f, .1f, .1f, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setDrawBB(true);
	this->trapBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	this->dartTrap.emplace_back(this->m_gameObjects.back());

	vec = DirectX::XMVectorSet(10.f, 27, -60 + 140.f, 1.f);
	this->addGameObjectToRoom(true, false, 2, 3, &m_models->at(3), vec, DirectX::XMVectorSet(0.1f, 0.1f, 0.1f, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setDrawBB(true);
	this->trapBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	this->dartTrap.emplace_back(this->m_gameObjects.back());

	//Lever function
	this->lever[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	this->lever[1]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	this->wonPuzzleObject[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());

}
void KevinsRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
	this->m_completed = true;

	this->m_player->getphysicsCompPtr()->setVelocity({0, 0, 0 });
	this->m_player->getMoveCompPtr()->position = this->getEntrancePosition();
	this->m_player->getphysicsCompPtr()->setVelocity({ 0, 0, 0 });

}