#include "pch.h"
#include "TristansRoom.h"

void TristansRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	addBoundingBox({ 69, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Left Wall
	addBoundingBox({ -69, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Right Wall
	addBoundingBox({ 0, 4.5f, 110 }, DirectX::XMFLOAT3(100, 75, 1)); //Back Wall
	addBoundingBox({ 0, 4.5f, -100 }, DirectX::XMFLOAT3(100, 75, 1)); //Front Wall

	addBoundingBox({ 50, 4.5f, 92 }, DirectX::XMFLOAT3(10, 75, 10)); //LeftCorner



	addBoundingBox({ 0, 45, 90 }, DirectX::XMFLOAT3(100, 1, 30)); //Floor 1
	addBoundingBox({ 0, 45, -52 }, DirectX::XMFLOAT3(100, 1, 50)); //Floor 2

	addBoundingBox({ 0, 46, 89 }, DirectX::XMFLOAT3(1, 4, 1)); //Front Lever
	//Back Levers
	addBoundingBox({ 15, 46, 73 }, DirectX::XMFLOAT3(1, 4, 1)); //Lever 1
	addBoundingBox({ 0, 46, 73 }, DirectX::XMFLOAT3(1, 4, 1)); //Lever 2
	addBoundingBox({ -15, 46, 73 }, DirectX::XMFLOAT3(1, 4, 1)); //Lever 3

	//Spikes
	SpikesBB.emplace_back();
	addTriggerBB({ 0, 20, 0 }, DirectX::XMFLOAT3(100, 1, 200));
	SpikesBB.back() = m_triggerBoundingBoxes.back();



}

void TristansRoom::addBellToRoom(Model* mdl, DirectX::XMVECTOR position)
{
	m_wvpCBuffers->emplace_back();
	m_wvpCBuffers->back().init(m_device, m_dContext);
	m_gameObjects.emplace_back(new Bell());
	dynamic_cast<Bell*>(m_gameObjects.back())->initialize((int)m_wvpCBuffers->size() - 1, mdl);

	XMVECTOR pos = m_worldPosition + position;
	m_gameObjects.back()->setPosition(pos);
}

void TristansRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);


	// PuzzleRoom
	pos = DirectX::XMVectorSet(0, 45, 110, 1); // World pos
	addGameObjectToRoom(true, false, 1, &m_models->at("TristansNewPuzzleRoom.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));

	// Diamond
	pos = DirectX::XMVectorSet(0, 50, -30, 1); // World pos
	addGameObjectToRoom(true, false, 1, &m_models->at("Diamond.bff"), pos, DirectX::XMVectorSet(2, 2, 2, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));
	m_gameObjects.back()->setRotation({ XMConvertToRadians(180),0.0f, 0.0f, 0.f });
	diamond = m_gameObjects.back();

	//Bell 1
	pos = DirectX::XMVectorSet(0 + BELL_OFFSET, 45 + BELL_OFFSET, 100, 1); // World pos
	addBellToRoom(&m_models->at("Bell2.bff"), pos);
	bell1 = dynamic_cast<Bell*>(m_gameObjects.back());

	//Bell 2
	pos = DirectX::XMVectorSet(0, 45 + BELL_OFFSET, 100, 1); // World pos
	addBellToRoom(&m_models->at("Bell2.bff"), pos);
	bell2 = dynamic_cast<Bell*>(m_gameObjects.back());

	//Bell 3
	pos = DirectX::XMVectorSet(0 - BELL_OFFSET, 45 + BELL_OFFSET, 100, 1); // World pos
	addBellToRoom(&m_models->at("Bell2.bff"), pos);
	bell3 = dynamic_cast<Bell*>(m_gameObjects.back());

	//Front Lever Grip
	pos = DirectX::XMVectorSet(0, 48.8f, 89.15f, 1); // World pos
	addLeverToRoom(&m_models->at("TristansLeverGrip.bff"), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	leverGrip[0]->setPlayerBoundingBox(m_player->getAABBPtr());
	//leverGrip[0]->toggleActivateLever();

	//Back Lever Grip 1
	pos = DirectX::XMVectorSet(15, 48.8f, 73.25f, 1); // World pos
	addLeverToRoom(&m_models->at("TristansLeverGrip.bff"), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	leverGrip[1]->setPlayerBoundingBox(m_player->getAABBPtr());
	leverGrip[1]->toggleActivateLever();


	//Back Lever Grip 2
	pos = DirectX::XMVectorSet(0, 48.8f, 73.25f, 1); // World pos
	addLeverToRoom(&m_models->at("TristansLeverGrip.bff"), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	leverGrip[2]->setPlayerBoundingBox(m_player->getAABBPtr());
	leverGrip[2]->toggleActivateLever();


	//Back Lever Grip 3
	pos = DirectX::XMVectorSet(-15, 48.8f, 73.25f, 1); // World pos
	addLeverToRoom(&m_models->at("TristansLeverGrip.bff"), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	leverGrip[3]->setPlayerBoundingBox(m_player->getAABBPtr());
	leverGrip[3]->toggleActivateLever();

}

void TristansRoom::onCompleted()
{

}

TristansRoom::TristansRoom()
{
	Room::initParent();
	m_entrencePosition = { 0, 50, 95, 1 };
	srand((unsigned)time(0));
	isAnimationGoing = false;
}

TristansRoom::~TristansRoom()
{
	Room::~Room();
}

void TristansRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	if (!m_player->getIsSpawning())
	{
		Room::update(dt, camera, activeRoom, activeRoomChanged);
		//Spikes
		for (size_t i = 0; i < SpikesBB.size(); i++)
		{
			if (SpikesBB[i].Intersects(m_player->getAABB()))
			{
				m_player->respawn();
				m_player->setPosition(getEntrancePosition());
			}
		}

		//Front Lever
		leverGrip[0]->collidesWithPlayer();
		if (leverGrip[0]->getCanUseLever() == true)
		{
			if (m_player->getinUse() == true) //press E
			{
				leverTimer[0].restart();
				moveLever0 = true;


				//OutputDebugString(L"LEVER PULLED\n");

				bellsShallLower = true;
			}
		}

		if (bellsShallLower == true)
		{
			moveTimer.restart();
			bellsShallLower = false;
			moveBellsDown(0);
			StatusTextHandler::get().sendText("Keep an eye on where the diamond is!", 5);
		}
		if (moveTimer.isActive())
		{
			if (isAnimationGoing == false)
			{
				isAnimationGoing = true;
				randNr = (rand() % 3);
				//OutputDebugString(L" randNr choosen \n ");
				//OutputDebugStringA(std::to_string(randNr).c_str());
				//OutputDebugString(L"\n ");
				if (randNr == 0)
				{
					Bellmove1(1);
				}
				if (randNr == 1)
				{
					Bellmove2(1);
				}
				if (randNr == 2)
				{
					Bellmove3(1);
				}
			}
			else
			{
				if (randNr == 0)
				{
					//OutputDebugString(L" ranNr 01 \n ");
					Bellmove1(1);
					//OutputDebugString(L" ranNr 02 \n ");
				}
				if (randNr == 1)
				{
					//OutputDebugString(L" ranNr 11 \n ");
					Bellmove2(1);
					//OutputDebugString(L" ranNr 12 \n ");
				}
				if (randNr == 2)
				{
					//OutputDebugString(L" ranNr 21 \n ");
					Bellmove3(1);
					//OutputDebugString(L" ranNr 22 \n ");
				}
			}
		}


		if (leverTimer[0].timeElapsed() < 1)
		{
			if (moveLever0 == true)
			{
				leverGrip[0]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
			}
		}
		if (leverTimer[0].timeElapsed() >= 1)
		{
			moveLever0 = false;
		}

		//Back Lever 1
		leverGrip[1]->collidesWithPlayer();
		if (leverGrip[1]->getCanUseLever() == true)
		{
			if (m_player->getinUse() == true) //press E
			{

				leverTimer[1].restart();
				moveLever1 = true;


				//OutputDebugString(L"LEVER PULLED\n");

				bellsShallRase = true;
				if (randNr == 0)
				{
					//guessedRight = true;
					onWin();
				}
				if (randNr != 0)
				{
					//guessedRight = false;
					onFail();
				}
			}
		}
		if (bellsShallRase == true)
		{
			RaiseBellTimer.restart();
			bellsShallRase = false;
			moveBellsUp();
		}

		if (leverTimer[1].timeElapsed() < 1)
		{
			if (moveLever1 == true)
			{
				leverGrip[1]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
			}
		}

		if (leverTimer[1].timeElapsed() >= 1)
		{
			moveLever1 = false;
		}

		//Back Lever 2
		leverGrip[2]->collidesWithPlayer();
		if (leverGrip[2]->getCanUseLever() == true)
		{
			if (m_player->getinUse() == true) //press E
			{
				leverTimer[2].restart();
				moveLever2 = true;

				bellsShallRase = true;
				if (randNr == 1)
				{
					//guessedRight = true;
					onWin();
				}
				if (randNr != 1)
				{
					//guessedRight = false;
					onFail();
				}
			}
		}
		if (bellsShallRase == true)
		{
			RaiseBellTimer.restart();
			bellsShallRase = false;
			moveBellsUp();
		}
		if (leverTimer[2].timeElapsed() < 1)
		{
			if (moveLever2 == true)
			{
				leverGrip[2]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
			}
		}

		if (leverTimer[2].timeElapsed() >= 1)
		{
			moveLever2 = false;
		}

		//Back Lever 3
		leverGrip[3]->collidesWithPlayer();
		if (leverGrip[3]->getCanUseLever() == true)
		{
			if (m_player->getinUse() == true) //press E
			{
				leverTimer[3].restart();
				moveLever3 = true;

				bellsShallRase = true;
				if (randNr == 2)
				{
					//guessedRight = true;
					onWin();
				}
				if (randNr != 2)
				{
					//guessedRight = false;
					onFail();
				}
			}
		}
		if (bellsShallRase == true)
		{
			RaiseBellTimer.restart();
			bellsShallRase = false;
			moveBellsUp();
		}
		if (leverTimer[3].timeElapsed() < 1)
		{
			if (moveLever3 == true)
			{
				leverGrip[3]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
			}
		}
		if (leverTimer[3].timeElapsed() >= 1)
		{
			moveLever3 = false;
		}
	}
}

void TristansRoom::onEntrance()
{
	Room::onEntrance();
	m_player->setSpawnPosition(getEntrancePosition());
	m_player->setRotation({ 0.f, XM_PI, 0.f, 1.f });
}

void TristansRoom::init()
{
	createSceneObjects();
	createBoundingBoxes();
}

void TristansRoom::portals()
{
	addPortalToRoom(XMVectorSet(0, 0, 0, 1), &m_models->at("Button.bff"),
		XMVectorSet(0, 48.8f, 113, 1), //Pos
		XMVectorSet(1, 1, 1, 1),    //Scale
		DirectX::XMFLOAT3(3, 20, 5), //BBSize
		0, false);
	portal = dynamic_cast<Portal*>(m_gameObjects.back());
	portal->setActiveStatus(false);
}

void TristansRoom::moveBellsUp()
{
	if (RaiseBellTimer.timeElapsed() >= 0.f && RaiseBellTimer.timeElapsed() <= 1.f)
	{
		if (bell1->GetAnimating() == false)
		{
			bell1->raiseBell();
			bell2->raiseBell();
			bell3->raiseBell();
		}
	}
}

void TristansRoom::moveBellsDown(float startTime)
{
	if (moveTimer.timeElapsed() >= startTime && moveTimer.timeElapsed() <= startTime + 1)
	{
		if (bell1->GetAnimating() == false)
		{
			bell1->lowerBell();
			bell2->lowerBell();
			bell3->lowerBell();
		}
	}
}

void TristansRoom::Bellmove1(float startTime)
{
	//Step1
	if (moveTimer.timeElapsed() >= startTime && moveTimer.timeElapsed() <= startTime + 1.f)
	{
		//OutputDebugString(L"MOVE1");
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//OutputDebugString(L"Step 1 START!\n");
			bell1->moveBellForward();
			//bell2->lowerBell();
			bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 1 END!\n");
			diamond->setPosition(DirectX::XMVectorSet(30.f, 0.f, -30.f, 1) + m_worldPosition);
		}
	}
	//Step2
	if (moveTimer.timeElapsed() >= startTime + 1.f && moveTimer.timeElapsed() <= startTime + 2.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellRight();
			bell2->moveBellRight();
			bell3->moveBellLeft2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 2 \n");
		}
	}
	//Step3
	if (moveTimer.timeElapsed() >= startTime + 2.f && moveTimer.timeElapsed() <= startTime + 3.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//bell1->moveBellBackward2();
			bell2->moveBellForward2();
			bell3->moveBellBackward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 3 \n");
		}
	}
	//Step4
	if (moveTimer.timeElapsed() >= startTime + 3.f && moveTimer.timeElapsed() <= startTime + 4.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//bell1->moveBellBackward2();
			bell2->moveBellLeft();
			bell3->moveBellRight();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 4 \n");
		}
	}
	//Step5
	if (moveTimer.timeElapsed() >= startTime + 4.f && moveTimer.timeElapsed() <= startTime + 5.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellRight();
			//bell3->moveBellRight();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 5 \n");
		}
	}
	//Step6
	if (moveTimer.timeElapsed() >= startTime + 5.f && moveTimer.timeElapsed() <= startTime + 6.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			bell2->moveBellBackward();
			bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 6 \n");
		}
	}
	//Step7
	if (moveTimer.timeElapsed() >= startTime + 6.f && moveTimer.timeElapsed() <= startTime + 7.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//bell1->moveBellBackward();
			bell2->moveBellLeft2();
			bell3->moveBellRight();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 7 \n");
		}
	}
	//Step8
	if (moveTimer.timeElapsed() >= startTime + 7.f && moveTimer.timeElapsed() <= startTime + 8.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellForward();
			bell2->moveBellForward();
			bell3->moveBellBackward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 8 \n");
		}
	}
	//Step9
	if (moveTimer.timeElapsed() >= startTime + 8.f && moveTimer.timeElapsed() <= startTime + 9.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellRight();
			bell3->moveBellLeft2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 9 \n");
		}
	}
	//Step10
	if (moveTimer.timeElapsed() >= startTime + 9.f && moveTimer.timeElapsed() <= startTime + 10.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//bell1->moveBellRight2();
			bell2->moveBellBackward2();
			bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 10 \n");
		}
	}
	//Step11
	if (moveTimer.timeElapsed() >= startTime + 10.f && moveTimer.timeElapsed() <= startTime + 11.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellLeft();
			bell3->moveBellRight2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 11 \n");
		}
	}
	//Step12
	if (moveTimer.timeElapsed() >= startTime + 11.f && moveTimer.timeElapsed() <= startTime + 12.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			//bell2->moveBellLeft();
			bell3->moveBellBackward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 12 \n");
		}
	}
	if (moveTimer.timeElapsed() >= startTime + 12.2f)
	{
		diamond->setPosition(DirectX::XMVectorSet(30.f, 50.f, -30.f, 1) + m_worldPosition);
	}
	if (moveTimer.isActive())
	{
		if (moveTimer.timeElapsed() > 13.2f)
		{
			for (int i = 0; i < 4; i++)
			{
				leverGrip[i]->toggleActivateLever();
				StatusTextHandler::get().sendText("Where is the diamond?", 5);
			}
			moveTimer.stop();
		}
	}
}

void TristansRoom::Bellmove2(float startTime)
{
	//Step1
	if (moveTimer.timeElapsed() >= startTime && moveTimer.timeElapsed() <= startTime + 1.f)
	{
		//OutputDebugString(L"MOVE2");
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//OutputDebugString(L"Step 1 START!\n");
			bell1->moveBellForward();
			bell2->moveBellForward2();
			//bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 1 END!\n");
			diamond->setPosition(DirectX::XMVectorSet(0.f, 0.f, -30.f, 1) + m_worldPosition);
		}
	}
	//Step2
	if (moveTimer.timeElapsed() >= startTime + 1.f && moveTimer.timeElapsed() <= startTime + 2.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellRight();
			//bell2->moveBellRight();
			bell3->moveBellLeft();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 2 \n");
		}
	}
	//Step3
	if (moveTimer.timeElapsed() >= startTime + 2.f && moveTimer.timeElapsed() <= startTime + 3.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellRight();
			//bell2->moveBellForward2();
			//bell3->moveBellBackward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 3 \n");
		}
	}
	//Step4
	if (moveTimer.timeElapsed() >= startTime + 3.f && moveTimer.timeElapsed() <= startTime + 4.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			//bell2->moveBellLeft();
			//bell3->moveBellRight();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 4 \n");
		}
	}
	//Step5
	if (moveTimer.timeElapsed() >= startTime + 4.f && moveTimer.timeElapsed() <= startTime + 5.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//bell1->moveBellLeft();
			//bell2->moveBellRight();
			bell3->moveBellForward();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 5 \n");
		}
	}
	//Step6
	if (moveTimer.timeElapsed() >= startTime + 5.f && moveTimer.timeElapsed() <= startTime + 6.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft2();
			bell2->moveBellRight();
			//bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 6 \n");
		}
	}
	//Step7
	if (moveTimer.timeElapsed() >= startTime + 6.f && moveTimer.timeElapsed() <= startTime + 7.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellForward2();
			bell2->moveBellBackward();
			bell3->moveBellForward();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 7 \n");
		}
	}
	//Step8
	if (moveTimer.timeElapsed() >= startTime + 7.f && moveTimer.timeElapsed() <= startTime + 8.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellBackward2();
			bell2->moveBellLeft();
			bell3->moveBellRight();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 8 \n");
		}
	}
	//Step9
	if (moveTimer.timeElapsed() >= startTime + 8.f && moveTimer.timeElapsed() <= startTime + 9.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellLeft();
			bell3->moveBellLeft2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 9 \n");
		}
	}
	//Step10
	if (moveTimer.timeElapsed() >= startTime + 9.f && moveTimer.timeElapsed() <= startTime + 10.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellForward2();
			bell2->moveBellBackward();
			bell3->moveBellBackward();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 10 \n");
		}
	}
	//Step11
	if (moveTimer.timeElapsed() >= startTime + 10.f && moveTimer.timeElapsed() <= startTime + 11.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft2();
			bell2->moveBellRight();
			bell3->moveBellRight2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 11 \n");
		}
	}
	//Step12
	if (moveTimer.timeElapsed() >= startTime + 11.f && moveTimer.timeElapsed() <= startTime + 12.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellBackward2();
			//bell2->moveBellLeft();
			bell3->moveBellBackward();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 12 \n");
			//diamond->setPosition(DirectX::XMVectorSet(0.f, 50.f, -30.f, 1.f) + m_worldPosition);
		}
	}
	if (moveTimer.timeElapsed() >= startTime + 12.2f)
	{
		diamond->setPosition(DirectX::XMVectorSet(0.f, 50.f, -30.f, 1.f) + m_worldPosition);
	}
	if (moveTimer.isActive())
	{
		if (moveTimer.timeElapsed() > 13.2f)
		{
			for (int i = 0; i < 4; i++)
			{
				leverGrip[i]->toggleActivateLever();
				StatusTextHandler::get().sendText("Where is the diamond?", 5);
			}
			moveTimer.stop();
		}
	}
}

void TristansRoom::Bellmove3(float startTime)
{
	//Step1
	if (moveTimer.timeElapsed() >= startTime && moveTimer.timeElapsed() <= startTime + 1.f)
	{
		//OutputDebugString(L"MOVE3");
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//OutputDebugString(L"Step 1 START!\n");
			bell1->moveBellForward();
			bell2->moveBellForward2();
			//bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 1 END!\n");
			diamond->setPosition(DirectX::XMVectorSet(-30.f, 0.f, -30.f, 1) + m_worldPosition);
		}
	}
	//Step2
	if (moveTimer.timeElapsed() >= startTime + 1.f && moveTimer.timeElapsed() <= startTime + 2.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellLeft();
			bell3->moveBellLeft();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 2 \n");
		}
	}
	//Step3
	if (moveTimer.timeElapsed() >= startTime + 2.f && moveTimer.timeElapsed() <= startTime + 3.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//bell1->moveBellRight();
			bell2->moveBellBackward2();
			bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L", Step 3 \n");
		}
	}
	//Step4
	if (moveTimer.timeElapsed() >= startTime + 3.f && moveTimer.timeElapsed() <= startTime + 4.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellRight();
			//bell3->moveBellRight();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 4 \n");
		}
	}
	//Step5
	if (moveTimer.timeElapsed() >= startTime + 4.f && moveTimer.timeElapsed() <= startTime + 5.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			//bell2->moveBellRight();
			bell3->moveBellRight();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 5 \n");
		}
	}
	//Step6
	if (moveTimer.timeElapsed() >= startTime + 5.f && moveTimer.timeElapsed() <= startTime + 6.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			bell2->moveBellForward2();
			//bell3->moveBellForward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 6 \n");
		}
	}
	//Step7
	if (moveTimer.timeElapsed() >= startTime + 6.f && moveTimer.timeElapsed() <= startTime + 7.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellLeft();
			bell3->moveBellLeft();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 7 \n");
		}
	}
	//Step8
	if (moveTimer.timeElapsed() >= startTime + 7.f && moveTimer.timeElapsed() <= startTime + 8.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellForward2();
			bell2->moveBellBackward();
			bell3->moveBellBackward2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 8 \n");
		}
	}
	//Step9
	if (moveTimer.timeElapsed() >= startTime + 8.f && moveTimer.timeElapsed() <= startTime + 9.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellRight();
			//bell3->moveBellLeft2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 9 \n");
		}
	}
	//Step10
	if (moveTimer.timeElapsed() >= startTime + 9.f && moveTimer.timeElapsed() <= startTime + 10.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			//bell1->moveBellForward2();
			bell2->moveBellRight();
			//bell3->moveBellBackward();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 10 \n");
		}
	}
	//Step11
	if (moveTimer.timeElapsed() >= startTime + 10.f && moveTimer.timeElapsed() <= startTime + 11.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			//bell2->moveBellRight();
			//bell3->moveBellRight2();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 11 \n");
		}
	}
	//Step12
	if (moveTimer.timeElapsed() >= startTime + 11.f && moveTimer.timeElapsed() <= startTime + 12.f)
	{
		if (!bell1->GetAnimating() && !bell2->GetAnimating() && !bell3->GetAnimating())
		{
			bell1->moveBellBackward2();
			bell2->moveBellBackward();
			//bell3->moveBellBackward();
			//OutputDebugStringA(std::to_string(moveTimer.timeElapsed()).c_str());
			//OutputDebugString(L"Step 12 \n");
			//diamond->setPosition(DirectX::XMVectorSet(-30.f, 50.f, -30.f, 1) + m_worldPosition);
		}
	}
	if (moveTimer.timeElapsed() >= startTime + 12.2f)
	{
		diamond->setPosition(DirectX::XMVectorSet(-30.f, 50.f, -30.f, 1) + m_worldPosition);
	}
	if (moveTimer.isActive())
	{
		if (moveTimer.timeElapsed() > 13.2f)
		{
			for (int i = 0; i < 4; i++)
			{
				leverGrip[i]->toggleActivateLever();
				StatusTextHandler::get().sendText("Where is the diamond?", 5);
			}
			moveTimer.stop();
		}
	}
}

void TristansRoom::onWin()
{
	StatusTextHandler::get().sendText("You guessed right!\n  Portal activated", 5);
	//portal->setPosition(XMVectorSet(0, 48.8f, 113, 1));
	portal->setActiveStatus(true);
	moveTimer.stop();
	leverGrip[1]->toggleActivateLever();
	leverGrip[2]->toggleActivateLever();
	leverGrip[3]->toggleActivateLever();
}

void TristansRoom::onFail()
{
	//Reset Player
	m_player->respawn();
	m_player->setPosition(getEntrancePosition());

	for (int i = 0; i < 4; i++)
	{
		leverGrip[i]->toggleActivateLever();
		//leverGrip[i]->getMoveCompPtr()->rotation = XMVectorSet(pMath::convertDegreesToRadians(-90), 0, 0, 10);
		leverGrip[i]->getMoveCompPtr()->rotation = XMVectorSet(0, 0, 0, 1);
	}
	moveLever0 = false;
	moveLever1 = false;
	moveLever2 = false;
	moveLever3 = false;

	isAnimationGoing = false;

	randNr = (rand() % 3);

	bell1->setPosition(XMVectorSet(0 + BELL_OFFSET, 45, 100, 1) + m_worldPosition);
	bell2->setPosition(XMVectorSet(0, 45, 100, 1) + m_worldPosition);
	bell3->setPosition(XMVectorSet(0 - BELL_OFFSET, 45, 100, 1) + m_worldPosition);

	diamond->setPosition(DirectX::XMVectorSet(0.f, 50.f, -30.f, 1.f) + m_worldPosition);
	moveTimer.stop();

	StatusTextHandler::get().sendText("You guessed wrong!\n5 sec added to timer", 5);
	m_gameTimerPointer->addTime(5);
}