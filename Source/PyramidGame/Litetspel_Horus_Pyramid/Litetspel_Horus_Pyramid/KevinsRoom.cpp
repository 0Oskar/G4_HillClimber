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
	//Så boundingBoxen följer skorpionen.w

	float scorpionX = XMVectorGetX(this->scorpion->getPosition());
	float scorpionY = XMVectorGetY(this->scorpion->getPosition());
	float scorpionZ = XMVectorGetZ(this->scorpion->getPosition());
	scorpionZ -= 0.6f;

	XMFLOAT3 scorpionPos = XMFLOAT3(scorpionX, scorpionY, scorpionZ);



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

	if (triggerBB[3].Intersects(this->m_player->getAABB()) && bridgeStop > 0)
	{
		bridgeStop -= (3 * dt);
		this->expandingBridge[0]->getMoveCompPtr()->position = this->expandingBridge[0]->getMoveCompPtr()->position + (XMVectorSet(0.0f, 0.f, 3 * dt, 0.f));
	}

	if (triggerBB[4].Intersects(this->m_player->getAABB()))
	{
		this->scorpion->setReachedEdge(false);
	}
	

	//Scorpion walking bounds
	if (triggerBB[2].Intersects(*scorpionBB) || triggerBB[5].Intersects(*scorpionBB))
	{
		this->scorpion->setReachedEdge(true);
	}

	if (scorpionBB->Intersects(this->m_player->getAABB()))
	{
		this->m_player->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(-12.f, 15.f, -155.f + 140.f, 1.f));
	}
	
	for (int i = 0; i < deathTrapBB.size(); i++)
	{
		if (deathTrapBB[i].Intersects(this->m_player->getAABB()))
		{
			void looseLife();
			//this->m_player->resetVelocity();
			this->m_player->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(-12.f, 20.f, -155.f + 140.f, 1.f));
		}
	}

	for (int i = 0; i < dartTrap.size(); i++)
	{
		if (dartTrap[i]->getAABB().Intersects(this->m_player->getAABB()))
		{
			void looseLife();
			//this->m_player->resetVelocity();
			this->m_player->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(-12.f, -20.f, -155.f + 140.f, 1.f));
		}
	}

	if (dartFly1 == true)
	{
		if (dartPosition1 <= 0)
		{
			dartPosition1 = 110.f;
			dartTrap[0]->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(10.f, 27, -80 + 140.f, 0.f));
			dartFly1 = false;
		}
		else
		{
			dartTrap[0]->getMoveCompPtr()->position = dartTrap[0]->getMoveCompPtr()->position + DirectX::XMVectorSet(-110.f * dt, 0, 0, 0.f);
			dartPosition1 -= 110.f * dt;
		}
	}

	if (dartFly2 == true)
	{
		if (dartPosition2 <= 0)
		{
			dartPosition2 = 110.f;
			dartTrap[1]->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(10.f, 27.f, -50.f + 140.f, 0.f));
			dartFly2 = false;
		}
		else
		{
			dartTrap[1]->getMoveCompPtr()->position = dartTrap[1]->getMoveCompPtr()->position + DirectX::XMVectorSet(-110.f * dt, 0, 0, 0.f);
			dartPosition2 -= 110.f * dt;
		}
	}

	//Lever
	this->lever[0]->collidesWithPlayer();

	if (this->lever[0]->getCanUseLever() == true)
	{
		if (this->m_player->getinUse() == true && this->trapActive1 == true)
		{
			this->lever[0]->getMoveCompPtr()->rotation -= DirectX::XMVectorSet(XMConvertToRadians(180.f), 0.f, 0.f, 0.f);
			trapActive1 = false;
		}
	}

	this->lever[1]->collidesWithPlayer();
	if (this->lever[1]->getCanUseLever() == true && this->trapActive2 == true)
	{
		if (this->m_player->getinUse() == true)
		{
			this->lever[1]->getMoveCompPtr()->rotation -= DirectX::XMVectorSet(XMConvertToRadians(180.f), 0.f, 0.f, 0.f);
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

void KevinsRoom::looseLife()
{
	this->lives--;
}

void KevinsRoom::checkNrOfLives()
{
	if (this->lives <= 0)
	{
		this->lostThePuzzle = true;
	}
}

void KevinsRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();

	this->m_entrencePosition = XMVectorSet(-13, 4, -20, 0);
}

void KevinsRoom::portals()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Add portals here
	vec = DirectX::XMVectorSet(-13.f, 8.f, -29.f, 1.f);
	XMVECTOR vecScale = DirectX::XMVectorSet(1.3f, 1.3f, -1.3f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 10, &m_models->at(10), vec, NormalScale, DirectX::XMFLOAT3(3.f, 8.f, 0.6f), 0, false);
	m_gameObjects.back()->setRotation(XMVectorSet(0.0f, XMConvertToRadians(180), 0.0f, 0.0f));
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
	this->addBoundingBox({ -10.f, 21.5f, -77.f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 2.5f, 10.8f));
	this->addBoundingBox({ -10.f, 21.5f, -45.4f + 140.f, 1}, DirectX::XMFLOAT3(20.f, 2.5f, 11.8f));
	this->addBoundingBox({ 9.5f, 2.f, -120.f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 40.f, 85.f));
	this->addBoundingBox({ -28.f, 2.f, -120.f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 40.f, 85.f));


	this->addBoundingBox({ -10.f, 4.0f, -170.0f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 20.5f, 0.8f));
	this->addBoundingBox({ -10.f, 27.0f, -36.0f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 20.5f, 0.8f));

	//this->addBoundingBox({ -15.f, 10.f, -78.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
	

	//this->addBoundingBox({ 3.6f, 5.f, -93.3f + 140.f, 1.f }, DirectX::XMFLOAT3(2.7f, 2.7f, 2.7f));
	
	//scorpionEDGE ONLY FOR VISUAL
	//this->addBoundingBox({ -15.f, 10.f, -126.f + 140.f, 1}, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));


	triggerBB.emplace_back();
	this->addTriggerBB({ -10.f, 27.f, -80.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 4.5f));
	triggerBB.back() = this->m_triggerBoundingBoxes.back();

	triggerBB.emplace_back();
	this->addTriggerBB({ -10.f, 27.f, -50.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 4.5f));
	triggerBB.back() = this->m_triggerBoundingBoxes.back();

	//scorpionEdge
	triggerBB.emplace_back();
	this->addTriggerBB({ -15.f, 10.f, -126.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
	triggerBB.back() = this->m_triggerBoundingBoxes.back();

	triggerBB.emplace_back();
	this->addTriggerBB({ -23.5f, 5.f, -93.3f + 140.f, 1.f }, DirectX::XMFLOAT3(2.7f, 2.7f, 2.7f));
	triggerBB.back() = this->m_triggerBoundingBoxes.back();

	//Player activate Scorpion trigger
	triggerBB.emplace_back();
	this->addTriggerBB({ -11.6f, 2.f, -105.3f + 140.f, 1.f }, DirectX::XMFLOAT3(19.f, 3.f, 19.f));
	triggerBB.back() = this->m_triggerBoundingBoxes.back();

	//scorpionEdge2
	triggerBB.emplace_back();
	this->addTriggerBB({ -15.f, 10.f, -78.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
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
	//vec = DirectX::XMVectorSet(5.f, 25.f, -85.f + 140.f, 1.f);
	//this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	//Puzzle Room (Kevins Lever room)
	vec = DirectX::XMVectorSet(-10.f, 2, -100 + 140.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 6, &m_models->at(6), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ 0.0f, XMConvertToRadians(180), 0.0f, 0.f });
	
	//platform
	vec = DirectX::XMVectorSet(-9.f, 23.5f, -88.5f + 140.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(4.f, 0.5f, 2.5f));
	this->m_gameObjects.back()->setScale({ 0.0f,0.0f,0.0f, 0.0f });

	//Lever
	vec = DirectX::XMVectorSet(-15.f, 5.f, -87.5f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(-90), pMath::convertDegreesToRadians(-270), 1.f);
	this->addLeverToRoom(7, &m_models->at(7), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));

	//Lever
	vec = DirectX::XMVectorSet(8.8f, 4.f, -110.f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-270), 1.f);
	this->addLeverToRoom(7, &m_models->at(7), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));

	//Lever
	vec = DirectX::XMVectorSet(-8.f, 30.f, -39.8f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-270), 1.f);
	this->addLeverToRoom(7, &m_models->at(7), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	//Win object
	wonPuzzleObject.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	wonPuzzleObject[0]->setScale(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	//this->m_gameObjects.back()->setPosition(XMVectorSet(-6.f, 25.f, -70 + 140.f, 1.f));

	//DARTS
	vec = DirectX::XMVectorSet(10.f, 27, -80 + 140.f, 1.f);
	this->addGameObjectToRoom(true, false, 2, 25, &m_models->at(25), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 5.f, 5.5f));
	this->m_gameObjects.back()->setDrawBB(true);
	this->trapBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	this->dartTrap.emplace_back(this->m_gameObjects.back());

	vec = DirectX::XMVectorSet(10.f, 27, -50 + 140.f, 1.f);
	this->addGameObjectToRoom(true, false, 2, 25, &m_models->at(25), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f), DirectX::XMFLOAT3(2.f, 5.f, 5.5f));
	this->m_gameObjects.back()->setDrawBB(true);
	this->trapBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	this->dartTrap.emplace_back(this->m_gameObjects.back());

	//Lever function
	this->lever[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	this->lever[1]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	this->wonPuzzleObject[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());

	this->m_wvpCBuffers->emplace_back();

	this->m_wvpCBuffers->back().init(this->m_device, this->m_dContext);


	//SCORPION

	this->scorpion = new followingEnemy();

	this->scorpion->init(true, 23, this->m_wvpCBuffers->size() - 1, &m_models->at(23), m_player);

	this->scorpion->getMoveCompPtr()->position = XMVectorSet(-11.f, 2.9f, -100.f + 140.f, 1.f) + this->m_worldPosition;

	this->m_gameObjects.emplace_back(this->scorpion);
	
	this->scorpionBB = this->scorpion->getBB();

	this->scorpionObject.emplace_back(this->m_gameObjects.back());

	//dartTrapWall
	vec = DirectX::XMVectorSet(7.f, 30.f, -80.f + 140.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 24, &m_models->at(24), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	this->dartTrapWall.emplace_back(this->m_gameObjects.back());
	this->m_gameObjects.back()->setRotation(XMVectorSet(0.0f, XMConvertToRadians(180.0f), 0.0f, 0.0f));

	//dartTrapWall2
	vec = DirectX::XMVectorSet(7.f, 30.f, -50.f + 140.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 24, &m_models->at(24), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	this->dartTrapWall.emplace_back(this->m_gameObjects.back());
	this->m_gameObjects.back()->setRotation(XMVectorSet(0.0f, XMConvertToRadians(180.0f), 0.0f, 0.0f));

	//ExpandingBridge
	vec = DirectX::XMVectorSet(-14.f, 22.5f, -76.f + 140.f, 1.f);
	this->addGameObjectToRoom(true, true, 2, 26, &m_models->at(26), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(5.f, 1.f, 10.f));
	
	this->expandingBridge.emplace_back(this->m_gameObjects.back());

}
void KevinsRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
	this->m_completed = true;

	this->m_player->getphysicsCompPtr()->setVelocity({0, 0, 0 });
	this->m_player->getMoveCompPtr()->position = this->getEntrancePosition();
	this->m_player->getphysicsCompPtr()->setVelocity({ 0, 0, 0 });

}