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
	if (!m_player->getIsSpawning())
	{
		Room::update(dt, camera, activeRoom, activeRoomChanged);
		//Så boundingBoxen följer skorpionen.

		float scorpionX = XMVectorGetX(scorpion->getPosition());
		float scorpionY = XMVectorGetY(scorpion->getPosition());
		float scorpionZ = XMVectorGetZ(scorpion->getPosition());
		scorpionZ -= 0.6f;

		XMFLOAT3 scorpionPos = XMFLOAT3(scorpionX, scorpionY, scorpionZ);



		/////////////////////////////////////////

		if (triggerBB[0].Intersects(m_player->getAABB()))
		{
			if (trapActive1 == true)
			{
				dartFly1 = true;
			}
		}

		if (triggerBB[1].Intersects(m_player->getAABB()))
		{
			if (trapActive2 == true)
			{
				dartFly2 = true;
			}
		}

		if (triggerBB[3].Intersects(m_player->getAABB()) && bridgeStop > 0)
		{
			bridgeStop -= (3 * dt);
			expandingBridge[0]->getMoveCompPtr()->position = expandingBridge[0]->getMoveCompPtr()->position + (XMVectorSet(0.0f, 0.f, 3 * dt, 0.f));
		}

		//Scorpion walking bounds
		if (triggerBB[2].Intersects(*scorpionBB) || triggerBB[5].Intersects(*scorpionBB))
		{
			scorpion->setReachedEdge(true);
		}
		bool doOnce = false;
		if (scorpionBB->Intersects(m_player->getAABB()))
		{
			m_player->respawn();
			m_player->setPosition(getEntrancePosition());
			StatusTextHandler::get().sendText("The scorpion caught you!\n    5 sec added to timer", 5);
			m_gameTimerPointer->addTime(5);
		}
	
		for (size_t i = 0; i < deathTrapBB.size(); i++)
		{
			if (deathTrapBB[i].Intersects(m_player->getAABB()))
			{
				void looseLife();
				m_player->respawn();
				m_player->setPosition(getEntrancePosition());
				StatusTextHandler::get().sendText("You fell on the spikes!\n 5 sec added to timer", 5);
				m_gameTimerPointer->addTime(5);
			}
		}


		//Scorpion walking bounds
		if (triggerBB[2].Intersects(*scorpionBB) || triggerBB[5].Intersects(*scorpionBB))
		{
			scorpion->setReachedEdge(true);
		}
		doOnce = false;
		if (scorpionBB->Intersects(m_player->getAABB()))
		{
			m_player->respawn();
			m_player->setPosition(getEntrancePosition());
			StatusTextHandler::get().sendText("The scorpion caught you!\n    5 sec added to timer", 5);
			//Add Time
			m_gameTimerPointer->addTime(5);
		}

		for (size_t i = 0; i < deathTrapBB.size(); i++)
		{
			if (deathTrapBB[i].Intersects(m_player->getAABB()))
			{
				void looseLife();
				m_player->respawn();
				m_player->setPosition(getEntrancePosition());
				StatusTextHandler::get().sendText("You fell on the spikes!\n 5 sec added to timer", 5);
				m_gameTimerPointer->addTime(5);
			}
		}

		for (size_t i = 0; i < dartTrap.size(); i++)
		{
			if (dartTrap[i]->getAABB().Intersects(m_player->getAABB()))
			{
				void looseLife();
				m_player->respawn();
				m_player->setPosition(getEntrancePosition());
				//Add Time
				StatusTextHandler::get().sendText("You got hit by a dart trap!\n   5 sec added to timer", 5);
				m_gameTimerPointer->addTime(5);
			}
		}

		if (dartFly1 == true)
		{
			if (dartPosition1 <= 0)
			{
				dartPosition1 = 110.f;
				dartTrap[0]->getMoveCompPtr()->position = getRelativePosition(DirectX::XMVectorSet(10.f, 27, -80 + 140.f, 0.f));
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
				dartTrap[1]->getMoveCompPtr()->position = getRelativePosition(DirectX::XMVectorSet(10.f, 27.f, -50.f + 140.f, 0.f));
				dartFly2 = false;
			}
			else
			{
				dartTrap[1]->getMoveCompPtr()->position = dartTrap[1]->getMoveCompPtr()->position + DirectX::XMVectorSet(-110.f * dt, 0, 0, 0.f);
				dartPosition2 -= 110.f * dt;
			}
		}

		//Lever
		lever[0]->collidesWithPlayer();

		if (lever[0]->getCanUseLever() == true)
		{
			if (m_player->getinUse() == true && trapActive1 == true)
			{
				lever[0]->getMoveCompPtr()->rotation -= DirectX::XMVectorSet(XMConvertToRadians(180.f), 0.f, 0.f, 0.f);
				trapActive1 = false;
			}
		}

		lever[1]->collidesWithPlayer();
		if (lever[1]->getCanUseLever() == true && trapActive2 == true)
		{
			if (m_player->getinUse() == true)
			{
				lever[1]->getMoveCompPtr()->rotation -= DirectX::XMVectorSet(XMConvertToRadians(180.f), 0.f, 0.f, 0.f);
				trapActive2 = false;
			}
		}

		wonPuzzleObject[0]->collidesWithPlayer();
		if (wonPuzzleObject[0]->getCanUseLever() == true)
		{

			if (m_player->getinUse() == true)
			{
				if (!m_completed)
					onCompleted();
			}
		}
	}
	else
	{
		m_player->setPosition(getEntrancePosition());
	}
}

void KevinsRoom::looseLife()
{
	lives--;
}

void KevinsRoom::checkNrOfLives()
{
	if (lives <= 0)
	{
		lostThePuzzle = true;
	}
}

void KevinsRoom::init()
{
	createSceneObjects();
	createBoundingBoxes();

	m_entrencePosition = XMVectorSet(-9.f, 5, -15.f, 0.f);
}

void KevinsRoom::portals()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Add portals here
	vec = DirectX::XMVectorSet(-9.f, 8.f, -29.f, 1.f);
	XMVECTOR vecScale = DirectX::XMVectorSet(1.3f, 1.3f, -1.3f, 1.f);
	addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), &m_models->at("PortalGate.bff"), vec, NormalScale, DirectX::XMFLOAT3(3.f, 8.f, 0.6f), 0, false);
	m_gameObjects.back()->setRotation(XMVectorSet(0.0f, XMConvertToRadians(180), 0.0f, 0.0f));
	portalPtr = dynamic_cast<Portal*>(m_gameObjects.back());
}

void KevinsRoom::onEntrance()
{
	Room::onEntrance();
	portalPtr->setActiveStatus(false);
}
void KevinsRoom::createBoundingBoxes()
{
	//Add all boundingboxes that are not part of a gameobject(Walls and ground for rooms etc)
	addBoundingBox({ -10.f, 1.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 2.5f, 20.f));
	addBoundingBox({ -10.f, 1.0f, -154.0f + 140.f, 1}, DirectX::XMFLOAT3(20.f, 2.5f, 20.8f));
	addBoundingBox({ -10.f, 11.f, -86.f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 13.f, 1.5f));
	addBoundingBox({ -10.f, 21.5f, -77.f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 2.5f, 10.8f));
	addBoundingBox({ -10.f, 21.5f, -45.4f + 140.f, 1}, DirectX::XMFLOAT3(20.f, 2.5f, 11.8f));
	addBoundingBox({ 9.5f, 2.f, -120.f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 40.f, 85.f));
	addBoundingBox({ -28.f, 2.f, -120.f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 40.f, 85.f));


	addBoundingBox({ -10.f, 4.0f, -170.0f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 20.5f, 0.8f));
	addBoundingBox({ -10.f, 27.0f, -36.0f + 140.f, 1 }, DirectX::XMFLOAT3(20.f, 20.5f, 0.8f));

	//addBoundingBox({ -15.f, 10.f, -78.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
	

	//addBoundingBox({ 3.6f, 5.f, -93.3f + 140.f, 1.f }, DirectX::XMFLOAT3(2.7f, 2.7f, 2.7f));
	
	//scorpionEDGE ONLY FOR VISUAL
	//addBoundingBox({ -15.f, 10.f, -126.f + 140.f, 1}, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));


	triggerBB.emplace_back();
	addTriggerBB({ -10.f, 27.f, -80.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 4.5f));
	triggerBB.back() = m_triggerBoundingBoxes.back();

	triggerBB.emplace_back();
	addTriggerBB({ -10.f, 27.f, -50.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 4.5f));
	triggerBB.back() = m_triggerBoundingBoxes.back();

	//scorpionEdge
	triggerBB.emplace_back();
	addTriggerBB({ -15.f, 10.f, -126.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
	triggerBB.back() = m_triggerBoundingBoxes.back();

	triggerBB.emplace_back();
	addTriggerBB({ -23.5f, 5.f, -93.3f + 140.f, 1.f }, DirectX::XMFLOAT3(2.7f, 2.7f, 2.7f));
	triggerBB.back() = m_triggerBoundingBoxes.back();

	//Player activate Scorpion trigger
	triggerBB.emplace_back();
	addTriggerBB({ -11.6f, 2.f, -105.3f + 140.f, 1.f }, DirectX::XMFLOAT3(19.f, 3.f, 19.f));
	triggerBB.back() = m_triggerBoundingBoxes.back();

	//scorpionEdge2
	triggerBB.emplace_back();
	addTriggerBB({ -15.f, 10.f, -78.f + 140.f }, DirectX::XMFLOAT3(20.f, 10.f, 2.5f));
	triggerBB.back() = m_triggerBoundingBoxes.back();

	deathTrapBB.emplace_back();
	addTriggerBB({ -10.f, -1.5f, -129.f + 140.f }, DirectX::XMFLOAT3(27.f, 3.f, 4.f));
	deathTrapBB.back() = m_triggerBoundingBoxes.back();
	
	deathTrapBB.emplace_back();
	addTriggerBB({ -10.f, -0.5f, -63.f + 140.f }, DirectX::XMFLOAT3(27.f, 3.f, 5.f));
	deathTrapBB.back() = m_triggerBoundingBoxes.back();
}
void KevinsRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Kevin room platform
	//vec = DirectX::XMVectorSet(5.f, 25.f, -85.f + 140.f, 1.f);
	//addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	//Puzzle Room (Kevins Lever room)
	vec = DirectX::XMVectorSet(-10.f, 2, -100 + 140.f, 1.f);
	addGameObjectToRoom(false, false, 2, &m_models->at("LeverRoom2.obj"), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	m_gameObjects.back()->setRotation({ 0.0f, XMConvertToRadians(180), 0.0f, 0.f });
	

	//platform
	vec = DirectX::XMVectorSet(-9.f, 23.5f, -88.5f + 140.f, 1.f);
	addPlatformToRoom(&m_models->at("platform.obj"), vec, DirectX::XMFLOAT3(4.f, 0.5f, 2.5f));
	m_gameObjects.back()->setScale({ 0.0f,0.0f,0.0f, 0.0f });

	//Lever
	vec = DirectX::XMVectorSet(-15.f, 5.f, -87.5f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(-90), pMath::convertDegreesToRadians(-270), 1.f);
	addLeverToRoom(&m_models->at("Lever.obj"), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));

	//Lever
	vec = DirectX::XMVectorSet(8.8f, 4.f, -110.f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-270), 1.f);
	addLeverToRoom(&m_models->at("Lever.obj"), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	lever.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));

	//Lever
	vec = DirectX::XMVectorSet(-8.f, 30.f, -39.8f + 140.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, 0.f, pMath::convertDegreesToRadians(-270), 1.f);
	addLeverToRoom(&m_models->at("Lever.obj"), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	//Win object
	wonPuzzleObject.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	wonPuzzleObject[0]->setScale(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	//m_gameObjects.back()->setPosition(XMVectorSet(-6.f, 25.f, -70 + 140.f, 1.f));

	//DARTS
	vec = DirectX::XMVectorSet(10.f, 27, -80 + 140.f, 1.f);
	addGameObjectToRoom(true, false, 2, &m_models->at("dartTrapWallDart.obj"), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 5.f, 5.5f));
	m_gameObjects.back()->setDrawBB(true);
	trapBB.emplace_back(m_gameObjects.back()->getAABBPtr());

	dartTrap.emplace_back(m_gameObjects.back());

	vec = DirectX::XMVectorSet(10.f, 27, -50 + 140.f, 1.f);
	addGameObjectToRoom(true, false, 2, &m_models->at("dartTrapWallDart.obj"), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f), DirectX::XMFLOAT3(2.f, 5.f, 5.5f));
	m_gameObjects.back()->setDrawBB(true);
	trapBB.emplace_back(m_gameObjects.back()->getAABBPtr());

	dartTrap.emplace_back(m_gameObjects.back());

	//Lever function
	lever[0]->setPlayerBoundingBox(m_player->getAABBPtr());
	lever[1]->setPlayerBoundingBox(m_player->getAABBPtr());
	wonPuzzleObject[0]->setPlayerBoundingBox(m_player->getAABBPtr());

	m_wvpCBuffers->emplace_back();

	m_wvpCBuffers->back().init(m_device, m_dContext);


	//SCORPION

	scorpion = new followingEnemy();

	scorpion->init(true, (int)m_wvpCBuffers->size() - 1, &m_models->at("scorpionDone.obj"), m_player);

	scorpion->getMoveCompPtr()->position = XMVectorSet(-11.f, 2.9f, -100.f + 140.f, 1.f) + m_worldPosition;

	m_gameObjects.emplace_back(scorpion);
	
	scorpionBB = scorpion->getBB();

	scorpionObject.emplace_back(m_gameObjects.back());

	//dartTrapWall
	vec = DirectX::XMVectorSet(7.f, 30.f, -80.f + 140.f, 1.f);
	addGameObjectToRoom(false, false, 2, &m_models->at("dartTrapWall.obj"), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	dartTrapWall.emplace_back(m_gameObjects.back());
	m_gameObjects.back()->setRotation(XMVectorSet(0.0f, XMConvertToRadians(180.0f), 0.0f, 0.0f));

	//dartTrapWall2
	vec = DirectX::XMVectorSet(7.f, 30.f, -50.f + 140.f, 1.f);
	addGameObjectToRoom(false, false, 2, &m_models->at("dartTrapWall.obj"), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	dartTrapWall.emplace_back(m_gameObjects.back());
	m_gameObjects.back()->setRotation(XMVectorSet(0.0f, XMConvertToRadians(180.0f), 0.0f, 0.0f));

	//ExpandingBridge
	vec = DirectX::XMVectorSet(-14.f, 22.5f, -76.f + 140.f, 1.f);
	addGameObjectToRoom(true, true, 2, &m_models->at("expandingBridge.obj"), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(5.f, 1.f, 10.f));
	
	expandingBridge.emplace_back(m_gameObjects.back());

}
void KevinsRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
	m_completed = true;

	m_player->getphysicsCompPtr()->setVelocity({0, 0, 0 });
	m_player->getMoveCompPtr()->position = getEntrancePosition();
	m_player->getphysicsCompPtr()->setVelocity({ 0, 0, 0 });
	portalPtr->setActiveStatus(true);
}