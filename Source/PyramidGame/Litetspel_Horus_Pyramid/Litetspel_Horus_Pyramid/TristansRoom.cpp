#include "pch.h"
#include "TristansRoom.h"

void TristansRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	this->addBoundingBox({ 69, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Left Wall
	this->addBoundingBox({ -69, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Right Wall
	this->addBoundingBox({ 0, 4.5f, 110 }, DirectX::XMFLOAT3(100, 75, 1)); //Back Wall
	this->addBoundingBox({ 0, 4.5f, -100 }, DirectX::XMFLOAT3(100, 75, 1)); //Front Wall

	this->addBoundingBox({ 0, 45, 90 }, DirectX::XMFLOAT3(100, 1, 30)); //Floor 1
	this->addBoundingBox({ 0, 45, -52 }, DirectX::XMFLOAT3(100, 1, 50)); //Floor 2
	//this->addBoundingBox({ 0, 45, 0 }, DirectX::XMFLOAT3(50, 1, 100)); //Floor TEMP

	this->addBoundingBox({ 0, 46, 89 }, DirectX::XMFLOAT3(1, 4, 1)); //Front Lever
	//Back Levers
	this->addBoundingBox({ 15, 46, 73 }, DirectX::XMFLOAT3(1, 4, 1)); //Lever 1
	this->addBoundingBox({ 0, 46, 73 }, DirectX::XMFLOAT3(1, 4, 1)); //Lever 2
	this->addBoundingBox({ -15, 46, 73 }, DirectX::XMFLOAT3(1, 4, 1)); //Lever 3

	//Spikes
	SpikesBB.emplace_back();
	this->addTriggerBB({ 0, 20, 0 }, DirectX::XMFLOAT3(100, 1, 200));
	SpikesBB.back() = this->m_triggerBoundingBoxes.back();

}

void TristansRoom::addBellToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position)
{
	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(new Bell());
	dynamic_cast<Bell*>(this->m_gameObjects.back())->initialize(mdlIndex, (int)m_wvpCBuffers->size() - 1, mdl);

	XMVECTOR pos = m_worldPosition + position;
	this->m_gameObjects.back()->setPosition(pos);
}

void TristansRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);


	// PuzzleRoom
	pos = DirectX::XMVectorSet(0, 45, 110, 1); // World pos
	this->addGameObjectToRoom(true, false, 1, 21, &m_models->at(21), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));

	// Diamond
	pos = DirectX::XMVectorSet(0, 50, -30, 1); // World pos
	this->addGameObjectToRoom(true, false, 1, 33, &m_models->at(33), pos, DirectX::XMVectorSet(2, 2, 2, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(180),0.0f, 0.0f, 0.f });
	this->diamond = this->m_gameObjects.back();

	//Bell 1
	pos = DirectX::XMVectorSet(0 + BELL_OFFSET, 45 + BELL_OFFSET, 100, 1); // World pos
	this->addBellToRoom(34, &m_models->at(34), pos);
	this->bell1 = dynamic_cast<Bell*>(this->m_gameObjects.back());

	//Bell 2
	pos = DirectX::XMVectorSet(0, 45 + BELL_OFFSET, 100, 1); // World pos
	this->addBellToRoom(34, &m_models->at(34), pos);
	this->bell2 = dynamic_cast<Bell*>(this->m_gameObjects.back());

	//Bell 3
	pos = DirectX::XMVectorSet(0 - BELL_OFFSET, 45 + BELL_OFFSET, 100, 1); // World pos
	this->addBellToRoom(34, &m_models->at(34), pos);
	this->bell3 = dynamic_cast<Bell*>(this->m_gameObjects.back());

	//Front Lever Grip
	pos = DirectX::XMVectorSet(0, 48.8f, 89.15f, 1); // World pos
	this->addLeverToRoom( 27, &m_models->at(27), pos,XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	//leverGrip[0]->toggleActivateLever();

	//Back Lever Grip 1
	pos = DirectX::XMVectorSet(15, 48.8f, 73.25f, 1); // World pos
	this->addLeverToRoom(27, &m_models->at(27), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[1]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	this->leverGrip[1]->toggleActivateLever();


	//Back Lever Grip 2
	pos = DirectX::XMVectorSet(0, 48.8f, 73.25f, 1); // World pos
	this->addLeverToRoom(27, &m_models->at(27), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[2]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	this->leverGrip[2]->toggleActivateLever();


	//Back Lever Grip 3
	pos = DirectX::XMVectorSet(-15, 48.8f, 73.25f, 1); // World pos
	this->addLeverToRoom(27, &m_models->at(27), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[3]->setPlayerBoundingBox(this->m_player->getAABBPtr());
	this->leverGrip[3]->toggleActivateLever();

}

void TristansRoom::onCompleted()
{

}

TristansRoom::TristansRoom()
{
	Room::initParent();
	this->m_entrencePosition = { 0, 50, 95, 1};
	srand((unsigned)time(0));
	isAnimationGoing = false;
	isMoving = false;
}

TristansRoom::~TristansRoom()
{
	Room::~Room();
}

void TristansRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	if (!this->m_player->getIsSpawning())
	{
		Room::update(dt, camera, activeRoom, activeRoomChanged);
		//Spikes
		for (int i = 0; i < SpikesBB.size(); i++)
		{
			if (SpikesBB[i].Intersects(this->m_player->getAABB()))
			{
				//this->m_player->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(0, 55, 95, 1));
				this->m_player->respawn();
				this->m_player->setPosition(getEntrancePosition());
			}
		}

		//Front Lever
		this->leverGrip[0]->collidesWithPlayer();
		if (this->leverGrip[0]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true) //press E
			{
				this->leverTimer[0].restart();
				this->moveLever0 = true;


				OutputDebugString(L"LEVER PULLED\n");

				this->bellsShallLower = true;
			}
		}

		if (this->bellsShallLower == true)
		{
			moveTimer.restart();
			bellsShallLower = false;
			moveBellsDown(0);
		}
		if (this->moveTimer.isActive())
		{
			if (isAnimationGoing == false)
			{
				isAnimationGoing = true;
				randNr = (rand() % 3);
				isMoving = true;
				OutputDebugString(L" randNr choosen \n ");
				OutputDebugStringA(std::to_string(randNr).c_str());
				OutputDebugString(L"\n ");
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
					OutputDebugString(L" ranNr 01 \n ");
					Bellmove1(1);
					OutputDebugString(L" ranNr 02 \n ");
				}
				if (randNr == 1)
				{
					OutputDebugString(L" ranNr 11 \n ");
					Bellmove2(1);
					OutputDebugString(L" ranNr 12 \n ");
				}
				if (randNr == 2)
				{
					OutputDebugString(L" ranNr 21 \n ");
					Bellmove3(1);
					OutputDebugString(L" ranNr 22 \n ");
				}
			}
		}


		if (this->leverTimer[0].timeElapsed() < 1)
		{
			if (this->moveLever0 == true)
			{
				this->leverGrip[0]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);//__________________________________________
			}
		}
		if (this->leverTimer[0].timeElapsed() >= 1)
		{
			this->moveLever0 = false;
		}
		/*
		if (this->leverTimer[0].timeElapsed() >= 3)
		{
			this->moveLeverAgain0 = true;
		}
		if (this->leverTimer[0].timeElapsed() < 4)
		{
			if (this->moveLeverAgain0 == true)
			{
				this->leverGrip[0]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(-90) * dt, 0, 0, 10);
			}
		}*/
		//if (this->leverTimer[0].timeElapsed() >= 4)
		//{
		//	this->tempLever0 = false;
		//	this->canPullLever0 = true;
		//	this->moveLever0 = false;
		//	this->leverGrip[0]->getCanUseLever() = true;
		//	//this->leverTimer[0].
		//}

		//Back Lever 1
		this->leverGrip[1]->collidesWithPlayer();
		if (this->leverGrip[1]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true) //press E
			{

				this->leverTimer[1].restart();
				this->moveLever1 = true;


				OutputDebugString(L"LEVER PULLED\n");

				this->bellsShallRase = true;
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
		if (this->bellsShallRase == true)
		{
			RaiseBellTimer.restart();
			bellsShallRase = false;
			moveBellsUp();
		}

		if (this->leverTimer[1].timeElapsed() < 1)
		{
			if (this->moveLever1 == true)
			{
				this->leverGrip[1]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);//__________________________________________
			}
		}

		if (this->leverTimer[1].timeElapsed() >= 1)
		{
			this->moveLever1 = false;
		}
		/*
		if (this->leverTimer[1].timeElapsed() >= 3)
		{
			this->moveLeverAgain1 = true;
		}
		if (this->leverTimer[1].timeElapsed() < 4)
		{
			if (this->moveLeverAgain1 == true)
			{
				this->leverGrip[1]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(-90) * dt, 0, 0, 10);
			}
		}
		*/
		//Back Lever 2
		this->leverGrip[2]->collidesWithPlayer();
		if (this->leverGrip[2]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true) //press E
			{
				this->leverTimer[2].restart();
				this->moveLever2 = true;

				this->bellsShallRase = true;
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
		if (this->bellsShallRase == true)
		{
			RaiseBellTimer.restart();
			bellsShallRase = false;
			moveBellsUp();
		}
		if (this->leverTimer[2].timeElapsed() < 1)
		{
			if (this->moveLever2 == true)
			{
				this->leverGrip[2]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);//__________________________________________
			}
		}

		if (this->leverTimer[2].timeElapsed() >= 1)
		{
			this->moveLever2 = false;
		}
		/*
		if (this->leverTimer[2].timeElapsed() >= 3)
		{
			this->moveLeverAgain2 = true;
		}
		if (this->leverTimer[2].timeElapsed() < 4)
		{
			if (this->moveLeverAgain2 == true)
			{
				this->leverGrip[2]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(-90) * dt, 0, 0, 10);
			}
		}
		*/
		//Back Lever 3
		this->leverGrip[3]->collidesWithPlayer();
		if (this->leverGrip[3]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true) //press E
			{
				this->leverTimer[3].restart();
				this->moveLever3 = true;

				this->bellsShallRase = true;
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
		if (this->bellsShallRase == true)
		{
			RaiseBellTimer.restart();
			bellsShallRase = false;
			isMoving = true;//--------------------------------------------------------------
			moveBellsUp();
		}
		if (this->leverTimer[3].timeElapsed() < 1)
		{
			if (this->moveLever3 == true)
			{
				this->leverGrip[3]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);//__________________________________________
			}
		}
		if (this->leverTimer[3].timeElapsed() >= 1)
		{
			this->moveLever3 = false;
		}
		/*
		if (this->leverTimer[3].timeElapsed() >= 3)
		{
			this->moveLeverAgain3 = true;
		}
		if (this->leverTimer[3].timeElapsed() < 4)
		{
			if (this->moveLeverAgain3 == true)
			{
				this->leverGrip[3]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(-90) * dt, 0, 0, 10);
			}
		}
		*/
		/*if (moveTimer.isActive())
		{
			if (this->moveTimer.timeElapsed() > 13.f)
			{
				for (int i = 0; i < 4; i++)
				{
					leverGrip[i]->toggleActivateLever();
					isMoving = false;
				}
				moveTimer.stop();
			}
		}*/
	}
}

void TristansRoom::onEntrance()
{
	Room::onEntrance();
	this->m_player->setSpawnPosition(this->getEntrancePosition());
	this->m_player->setRotation({ 0.f, XM_PI, 0.f, 1.f });
}

void TristansRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
}

void TristansRoom::portals()
{
	this->addPortalToRoom(XMVectorSet(0, 0, 0, 1), 9, &m_models->at(9),
		XMVectorSet(0, 48.8f, 113, 1), //Pos
		XMVectorSet(1, 1, 1, 1),    //Scale
		DirectX::XMFLOAT3(3, 20, 5), //BBSize
		0, false);
	this->portal = dynamic_cast<Portal*>(this->m_gameObjects.back());
	portal->setActiveStatus(false);
}

//int TristansRoom::randomNr()
//{
//	int randNr;
//	randNr = (rand() % 3 )+ 1;
//	return randNr;
//}

void TristansRoom::moveBellsUp()
{
	if (this->RaiseBellTimer.timeElapsed() >= 0.f && this->RaiseBellTimer.timeElapsed() <= 1.f)
	{
		if (this->bell1->GetAnimating() == false)
		{
			bell1->raiseBell();
			bell2->raiseBell();
			bell3->raiseBell();
		}
	}
	if (this->RaiseBellTimer.timeElapsed() > 1.f)
	{
		isMoving = false;
	}
}

void TristansRoom::moveBellsDown(float startTime)
{
	if (this->moveTimer.timeElapsed() >= startTime && this->moveTimer.timeElapsed() <= startTime + 1)
	{
		if (this->bell1->GetAnimating() == false)
		{
			bell1->lowerBell();
			bell2->lowerBell();
			bell3->lowerBell();
		}
	}
	if (this->RaiseBellTimer.timeElapsed() > startTime + 1)
	{
		isMoving = false;
	}
}

void TristansRoom::Bellmove1(float startTime)
{
	//Step1
	if (this->moveTimer.timeElapsed() >= startTime && this->moveTimer.timeElapsed() <= startTime + 1.f)
	{
		OutputDebugString(L"MOVE1");
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			OutputDebugString(L"Step 1 START!\n");
			bell1->moveBellForward();
			//bell2->lowerBell();
			bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 1 END!\n");
			this->diamond->setPosition(DirectX::XMVectorSet( 30.f, 0.f, -30.f, 1) + this->m_worldPosition);
		}
	}
	//Step2
	if (this->moveTimer.timeElapsed() >= startTime + 1.f && this->moveTimer.timeElapsed() <= startTime + 2.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellRight();
			bell2->moveBellRight();
			bell3->moveBellLeft2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 2 \n");
		}
	}
	//Step3
	if (this->moveTimer.timeElapsed() >= startTime + 2.f && this->moveTimer.timeElapsed() <= startTime + 3.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			//bell1->moveBellBackward2();
			bell2->moveBellForward2();
			bell3->moveBellBackward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 3 \n");
		}
	}
	//Step4
	if (this->moveTimer.timeElapsed() >= startTime + 3.f && this->moveTimer.timeElapsed() <= startTime + 4.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			//bell1->moveBellBackward2();
			bell2->moveBellLeft();
			bell3->moveBellRight();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 4 \n");
		}
	}
	//Step5
	if (this->moveTimer.timeElapsed() >= startTime + 4.f && this->moveTimer.timeElapsed() <= startTime + 5.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellRight();
			//bell3->moveBellRight();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 5 \n");
		}
	}
	//Step6
	if (this->moveTimer.timeElapsed() >= startTime + 5.f && this->moveTimer.timeElapsed() <= startTime + 6.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			bell2->moveBellBackward();
			bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 6 \n");
		}
	}
	//Step7
	if (this->moveTimer.timeElapsed() >= startTime + 6.f && this->moveTimer.timeElapsed() <= startTime + 7.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			//bell1->moveBellBackward();
			bell2->moveBellLeft2();
			bell3->moveBellRight();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 7 \n");
		}
	}
	//Step8
	if (this->moveTimer.timeElapsed() >= startTime + 7.f && this->moveTimer.timeElapsed() <= startTime + 8.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellForward();
			bell2->moveBellForward();
			bell3->moveBellBackward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 8 \n");
		}
	}
	//Step9
	if (this->moveTimer.timeElapsed() >= startTime + 8.f && this->moveTimer.timeElapsed() <= startTime + 9.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellRight();
			bell3->moveBellLeft2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 9 \n");
		}
	}
	//Step10
	if (this->moveTimer.timeElapsed() >= startTime + 9.f && this->moveTimer.timeElapsed() <= startTime + 10.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			//bell1->moveBellRight2();
			bell2->moveBellBackward2();
			bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 10 \n");
		}
	}
	//Step11
	if (this->moveTimer.timeElapsed() >= startTime + 10.f && this->moveTimer.timeElapsed() <= startTime + 11.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellLeft();
			bell3->moveBellRight2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 11 \n");
		}
	}
	//Step12
	if (this->moveTimer.timeElapsed() >= startTime + 11.f && this->moveTimer.timeElapsed() <= startTime + 12.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			//bell2->moveBellLeft();
			bell3->moveBellBackward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 12 \n");
		}
	}
	if (this->moveTimer.timeElapsed() >= startTime + 12.2f)
	{
		this->diamond->setPosition(DirectX::XMVectorSet(30.f, 50.f, -30.f, 1) + this->m_worldPosition);
	}
	if (moveTimer.isActive())
	{
		if (this->moveTimer.timeElapsed() > 13.2f)
		{
			for (int i = 0; i < 4; i++)
			{
				leverGrip[i]->toggleActivateLever();
				isMoving = false;
			}
			moveTimer.stop();
		}
	}
}

void TristansRoom::Bellmove2(float startTime)
{
	//Step1
	if (this->moveTimer.timeElapsed() >= startTime && this->moveTimer.timeElapsed() <= startTime + 1.f)
	{
		OutputDebugString(L"MOVE2");
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			OutputDebugString(L"Step 1 START!\n");
			bell1->moveBellForward();
			bell2->moveBellForward2();
			//bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 1 END!\n");
			this->diamond->setPosition(DirectX::XMVectorSet(0.f, 0.f, -30.f, 1) + this->m_worldPosition);
		}
	}
	//Step2
	if (this->moveTimer.timeElapsed() >= startTime + 1.f && this->moveTimer.timeElapsed() <= startTime + 2.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellRight();
			//bell2->moveBellRight();
			bell3->moveBellLeft();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 2 \n");
		}
	}
	//Step3
	if (this->moveTimer.timeElapsed() >= startTime + 2.f && this->moveTimer.timeElapsed() <= startTime + 3.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellRight();
			//bell2->moveBellForward2();
			//bell3->moveBellBackward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 3 \n");
		}
	}
	//Step4
	if (this->moveTimer.timeElapsed() >= startTime + 3.f && this->moveTimer.timeElapsed() <= startTime + 4.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			//bell2->moveBellLeft();
			//bell3->moveBellRight();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 4 \n");
		}
	}
	//Step5
	if (this->moveTimer.timeElapsed() >= startTime + 4.f && this->moveTimer.timeElapsed() <= startTime + 5.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			//bell1->moveBellLeft();
			//bell2->moveBellRight();
			bell3->moveBellForward();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 5 \n");
		}
	}
	//Step6
	if (this->moveTimer.timeElapsed() >= startTime + 5.f && this->moveTimer.timeElapsed() <= startTime + 6.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft2();
			bell2->moveBellRight();
			//bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 6 \n");
		}
	}
	//Step7
	if (this->moveTimer.timeElapsed() >= startTime + 6.f && this->moveTimer.timeElapsed() <= startTime + 7.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellForward2();
			bell2->moveBellBackward();
			bell3->moveBellForward();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 7 \n");
		}
	}
	//Step8
	if (this->moveTimer.timeElapsed() >= startTime + 7.f && this->moveTimer.timeElapsed() <= startTime + 8.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellBackward2();
			bell2->moveBellLeft();
			bell3->moveBellRight();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 8 \n");
		}
	}
	//Step9
	if (this->moveTimer.timeElapsed() >= startTime + 8.f && this->moveTimer.timeElapsed() <= startTime + 9.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellLeft();
			bell3->moveBellLeft2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 9 \n");
		}
	}
	//Step10
	if (this->moveTimer.timeElapsed() >= startTime + 9.f && this->moveTimer.timeElapsed() <= startTime + 10.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellForward2();
			bell2->moveBellBackward();
			bell3->moveBellBackward();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 10 \n");
		}
	}
	//Step11
	if (this->moveTimer.timeElapsed() >= startTime + 10.f && this->moveTimer.timeElapsed() <= startTime + 11.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft2();
			bell2->moveBellRight();
			bell3->moveBellRight2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 11 \n");
		}
	}
	//Step12
	if (this->moveTimer.timeElapsed() >= startTime + 11.f && this->moveTimer.timeElapsed() <= startTime + 12.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellBackward2();
			//bell2->moveBellLeft();
			bell3->moveBellBackward();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 12 \n");
			//this->diamond->setPosition(DirectX::XMVectorSet(0.f, 50.f, -30.f, 1.f) + this->m_worldPosition);
		}
	}
	if (this->moveTimer.timeElapsed() >= startTime + 12.2f)
	{
		this->diamond->setPosition(DirectX::XMVectorSet(0.f, 50.f, -30.f, 1.f) + this->m_worldPosition);
	}
	if (moveTimer.isActive())
	{
		if (this->moveTimer.timeElapsed() > 13.2f)
		{
			for (int i = 0; i < 4; i++)
			{
				leverGrip[i]->toggleActivateLever();
				isMoving = false;
			}
			moveTimer.stop();
		}
	}
}

void TristansRoom::Bellmove3(float startTime)
{
	//Step1
	if (this->moveTimer.timeElapsed() >= startTime && this->moveTimer.timeElapsed() <= startTime + 1.f)
	{
		OutputDebugString(L"MOVE3");
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			OutputDebugString(L"Step 1 START!\n");
			bell1->moveBellForward();
			bell2->moveBellForward2();
			//bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 1 END!\n");
			this->diamond->setPosition(DirectX::XMVectorSet(-30.f, 0.f, -30.f, 1) + this->m_worldPosition);
		}
	}
	//Step2
	if (this->moveTimer.timeElapsed() >= startTime + 1.f && this->moveTimer.timeElapsed() <= startTime + 2.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellLeft();
			bell3->moveBellLeft();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 2 \n");
		}
	}
	//Step3
	if (this->moveTimer.timeElapsed() >= startTime + 2.f && this->moveTimer.timeElapsed() <= startTime + 3.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			//bell1->moveBellRight();
			bell2->moveBellBackward2();
			bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L", Step 3 \n");
		}
	}
	//Step4
	if (this->moveTimer.timeElapsed() >= startTime + 3.f && this->moveTimer.timeElapsed() <= startTime + 4.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellRight();
			//bell3->moveBellRight();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 4 \n");
		}
	}
	//Step5
	if (this->moveTimer.timeElapsed() >= startTime + 4.f && this->moveTimer.timeElapsed() <= startTime + 5.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			//bell2->moveBellRight();
			bell3->moveBellRight();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 5 \n");
		}
	}
	//Step6
	if (this->moveTimer.timeElapsed() >= startTime + 5.f && this->moveTimer.timeElapsed() <= startTime + 6.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellBackward();
			bell2->moveBellForward2();
			//bell3->moveBellForward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 6 \n");
		}
	}
	//Step7
	if (this->moveTimer.timeElapsed() >= startTime + 6.f && this->moveTimer.timeElapsed() <= startTime + 7.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellRight2();
			bell2->moveBellLeft();
			bell3->moveBellLeft();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 7 \n");
		}
	}
	//Step8
	if (this->moveTimer.timeElapsed() >= startTime + 7.f && this->moveTimer.timeElapsed() <= startTime + 8.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellForward2();
			bell2->moveBellBackward();
			bell3->moveBellBackward2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 8 \n");
		}
	}
	//Step9
	if (this->moveTimer.timeElapsed() >= startTime + 8.f && this->moveTimer.timeElapsed() <= startTime + 9.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			bell2->moveBellRight();
			//bell3->moveBellLeft2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 9 \n");
		}
	}
	//Step10
	if (this->moveTimer.timeElapsed() >= startTime + 9.f && this->moveTimer.timeElapsed() <= startTime + 10.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			//bell1->moveBellForward2();
			bell2->moveBellRight();
			//bell3->moveBellBackward();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 10 \n");
		}
	}
	//Step11
	if (this->moveTimer.timeElapsed() >= startTime + 10.f && this->moveTimer.timeElapsed() <= startTime + 11.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellLeft();
			//bell2->moveBellRight();
			//bell3->moveBellRight2();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 11 \n");
		}
	}
	//Step12
	if (this->moveTimer.timeElapsed() >= startTime + 11.f && this->moveTimer.timeElapsed() <= startTime + 12.f)
	{
		if (!this->bell1->GetAnimating() && !this->bell2->GetAnimating() && !this->bell3->GetAnimating())
		{
			bell1->moveBellBackward2();
			bell2->moveBellBackward();
			//bell3->moveBellBackward();
			OutputDebugStringA(std::to_string(this->moveTimer.timeElapsed()).c_str());
			OutputDebugString(L"Step 12 \n");
			//this->diamond->setPosition(DirectX::XMVectorSet(-30.f, 50.f, -30.f, 1) + this->m_worldPosition);
		}
	}
	if (this->moveTimer.timeElapsed() >= startTime + 12.2f)
	{
		this->diamond->setPosition(DirectX::XMVectorSet(-30.f, 50.f, -30.f, 1) + this->m_worldPosition);
	}
	if (moveTimer.isActive())
	{
		if (this->moveTimer.timeElapsed() > 13.2f)
		{
			for (int i = 0; i < 4; i++)
			{
				leverGrip[i]->toggleActivateLever();
				isMoving = false;
			}
			moveTimer.stop();
		}
	}
}

//void TristansRoom::rotateLeverDown(int Index)
//{
//	/*for (int i = 0; i < 4; i++)
//	{*/
//		this->leverStartRot = this->leverGrip[Index]->getRotation();
//		XMVECTOR moveRot = XMVectorSet(pMath::convertDegreesToRadians(90), 0, 0, 0);
//		this->leverEndRot = this->leverStartRot + moveRot;
//
//		if (!XMVector3Equal(leverStartRot, leverEndRot))
//		{
//			XMVECTOR rotationDelta = this->leverEndRot - this->leverStartRot;
//			XMVECTOR currentleverRot = this->leverGrip[Index]->getRotation();
//			this->leverGrip[Index]->setRotation(this->leverStartRot + (rotationDelta * leverTimer->timeElapsed()));
//			if (this->leverTimer[Index].timeElapsed() > 1)
//			{
//				this->leverStartRot = this->leverEndRot;
//				this->leverGrip[Index]->setRotation(this->leverEndRot);
//			}
//		}
//	//}
//}

void TristansRoom::onWin()
{
	StatusTextHandler::get().sendText("You guessed right!\n  Portal activated", 5);
	//this->portal->setPosition(XMVectorSet(0, 48.8f, 113, 1));
	this->portal->setActiveStatus(true);
	moveTimer.stop();
	leverGrip[1]->toggleActivateLever();
	leverGrip[2]->toggleActivateLever();
	leverGrip[3]->toggleActivateLever();
}

void TristansRoom::onFail()
{
	//Reset Player
	this->m_player->respawn();
	this->m_player->setPosition(getEntrancePosition());

	for (int i = 0; i < 4; i++)
	{
		leverGrip[i]->toggleActivateLever();
		//this->leverGrip[i]->getMoveCompPtr()->rotation = XMVectorSet(pMath::convertDegreesToRadians(-90), 0, 0, 10);
		this->leverGrip[i]->getMoveCompPtr()->rotation = XMVectorSet(0, 0, 0, 1);
	}
	this->moveLever0 = false;
	this->moveLever1 = false;
	this->moveLever2 = false;
	this->moveLever3 = false;

	this->isAnimationGoing = false;

	randNr = (rand() % 3);

	this->bell1->setPosition(XMVectorSet(0 + BELL_OFFSET, 45, 100, 1) + this->m_worldPosition);
	this->bell2->setPosition(XMVectorSet( 0, 45, 100, 1) + this->m_worldPosition );
	this->bell3->setPosition(XMVectorSet(0 - BELL_OFFSET, 45, 100, 1) + this->m_worldPosition);
	
	this->diamond->setPosition(DirectX::XMVectorSet(0.f, 50.f, -30.f, 1.f) + this->m_worldPosition);
	moveTimer.stop();

	StatusTextHandler::get().sendText("You guessed wrong!\n5 sec added to timer", 5);
	this->m_gameTimerPointer->addTime(5);
}
