#include "pch.h"
#include "EdvinsRoom.h"

//TODO:
//Oriented bounding box
//Particalas

void EdvinsRoom::createBoundingBoxes()
{
	addBoundingBox({ -35, -1, 0, 1 }, DirectX::XMFLOAT3(35, 1, 30)); //Floor

	addBoundingBox({ 0, 6, 0, 1 }, DirectX::XMFLOAT3(1, 12, 19)); //Back Wall
	addBoundingBox({ -69, 6, 0, 1 }, DirectX::XMFLOAT3(1, 12, 5)); //Portal Wall

	// (Left)
	addBoundingBox({ -66, 6, 6, 1 }, DirectX::XMFLOAT3(2, 12, 1));
	addBoundingBox({ -67, 6,1 }, DirectX::XMFLOAT3(1, 12, 2));
	addBoundingBox({ -65, 6, 12, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -63, 6, 14, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -61, 6, 16, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -59, 6, 19, 1 }, DirectX::XMFLOAT3(1, 12, 2));
	addBoundingBox({ -56, 6, 22, 1 }, DirectX::XMFLOAT3(3, 12, 1));
	addBoundingBox({ -46, 6, 24, 1 }, DirectX::XMFLOAT3(7, 12, 1));
	addBoundingBox({ -35, 6, 26, 1 }, DirectX::XMFLOAT3(4, 12, 1));
	addBoundingBox({ -26, 6, 28, 1 }, DirectX::XMFLOAT3(5, 12, 1));
	addBoundingBox({ -18, 6, 30, 1 }, DirectX::XMFLOAT3(3, 12, 1));
	addBoundingBox({ -12, 6, 28, 1 }, DirectX::XMFLOAT3(3, 12, 1));
	addBoundingBox({ -7, 6, 26, 1 }, DirectX::XMFLOAT3(2, 12, 1));
	addBoundingBox({ -4, 6, 24, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -2, 6, 21, 1 }, DirectX::XMFLOAT3(1, 12, 2));

	// (Right)
	addBoundingBox({ -66, 6, -6, 1 }, DirectX::XMFLOAT3(2, 12, 1));
	addBoundingBox({ -67, 6, -9, 1 }, DirectX::XMFLOAT3(1, 12, 2));
	addBoundingBox({ -65, 6, -12, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -63, 6, -14, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -61, 6, -16, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -59, 6, -19, 1 }, DirectX::XMFLOAT3(1, 12, 2));
	addBoundingBox({ -56, 6, -22, 1 }, DirectX::XMFLOAT3(3, 12, 1));
	addBoundingBox({ -46, 6, -24, 1 }, DirectX::XMFLOAT3(7, 12, 1));
	addBoundingBox({ -35, 6, -26, 1 }, DirectX::XMFLOAT3(4, 12, 1));
	addBoundingBox({ -26, 6, -28, 1 }, DirectX::XMFLOAT3(5, 12, 1));
	addBoundingBox({ -18, 6, -30, 1 }, DirectX::XMFLOAT3(3, 12, 1));
	addBoundingBox({ -12, 6, -28, 1 }, DirectX::XMFLOAT3(3, 12, 1));
	addBoundingBox({ -7, 6, -26, 1 }, DirectX::XMFLOAT3(2, 12, 1));
	addBoundingBox({ -4, 6, -24, 1 }, DirectX::XMFLOAT3(1, 12, 1));
	addBoundingBox({ -2, 6, -21, 1 }, DirectX::XMFLOAT3(1, 12, 2));


	addBoundingBox({ -45, 0, 0, 1 }, DirectX::XMFLOAT3(1.5, 2, 1.5)); //Pedistal
}

void EdvinsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// PuzzleRoom 
	pos = DirectX::XMVectorSet(0, 3, 0, 1); //world pos
	addGameObjectToRoom(true, false, 1, &m_models->at("EdvinPuzzleRoom.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	
	// Pedistal
	scale = DirectX::XMVectorSet(2, 2, 2, 1);
	pos = DirectX::XMVectorSet(-45.f, -1.3f, 0.f, 1.f); //world pos
	addGameObjectToRoom(true, false, 1, &m_models->at("Pedistal.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Cover
	scale = DirectX::XMVectorSet(1, 1, 1, 1);
	addGameObjectToRoom(true, false, 1, &m_models->at("Cover.bff"), coverPos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	cover = m_gameObjects.back();

	// Buttons:

	// Button 1 
	pos = DirectX::XMVectorSet(-10, buttonsPosY, 10, 10); //world pos
	addLeverToRoom(&m_models->at("Button.bff"), pos, rot, XMFLOAT3(5, 10, 2));
	m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_0.png"); //Set texture
	buttons.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back())); //Add to buttons array
	buttons.at(0)->setPlayerBoundingBox(m_player->getAABBPtr()); //Create BB

	// Button 2
	pos = DirectX::XMVectorSet(-10, buttonsPosY, 5, 1); //world pos
	addLeverToRoom(&m_models->at("Button.bff"), pos, rot, XMFLOAT3(5, 10, 2));
	m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_1.png");
	buttons.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	buttons.at(1)->setPlayerBoundingBox(m_player->getAABBPtr());

	// Button 3
	pos = DirectX::XMVectorSet(-10, buttonsPosY, 0, 1); //world pos
	addLeverToRoom(&m_models->at("Button.bff"), pos, rot, XMFLOAT3(5, 10, 2));
	m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_2.png");
	buttons.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	buttons.at(2)->setPlayerBoundingBox(m_player->getAABBPtr());

	// Button 4
	pos = DirectX::XMVectorSet(-10, buttonsPosY, -5, 1); //world pos
	addLeverToRoom(&m_models->at("Button.bff"), pos, rot, XMFLOAT3(5, 10, 2));
	m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_3.png");
	buttons.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	buttons.at(3)->setPlayerBoundingBox(m_player->getAABBPtr());

	// Button 5
	pos = DirectX::XMVectorSet(-10, buttonsPosY, -10, 1); //world pos
	addLeverToRoom(&m_models->at("Button.bff"), pos, rot, XMFLOAT3(5, 10, 2));
	m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_4.png");
	buttons.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	buttons.at(4)->setPlayerBoundingBox(m_player->getAABBPtr());

	// Bricks:

	// Brick 1
	pos = DirectX::XMVectorSet(-6, 14, 15, 1); //world pos
	addGameObjectToRoom(true, false, 1, &m_models->at("Brick_1.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	m_bricks.emplace_back(m_gameObjects.back());

	// Brick 2
	pos = DirectX::XMVectorSet(-6, 14, 8, 1); //world pos
	addGameObjectToRoom(true, false, 1, &m_models->at("Brick_2.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	m_bricks.emplace_back(m_gameObjects.back());

	// Brick 3
	pos = DirectX::XMVectorSet(-6, 14, 1, 1); //world pos
	addGameObjectToRoom(true, false, 1, &m_models->at("Brick_3.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	m_bricks.emplace_back(m_gameObjects.back());

	// Brick 4
	pos = DirectX::XMVectorSet(-6, 14, -6, 1); //world pos
	addGameObjectToRoom(true, false, 1, &m_models->at("Brick_4.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	m_bricks.emplace_back(m_gameObjects.back());

	// Brick 5
	pos = DirectX::XMVectorSet(-6, 14, -13, 1); //world pos
	addGameObjectToRoom(true, false, 1, &m_models->at("Brick_5.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	m_bricks.emplace_back(m_gameObjects.back());

	// Leaver Base
	pos = DirectX::XMVectorSet(-45, 2, 0, 1); //world pos
	rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	addLeverToRoom(&m_models->at("LeverBase.bff"), pos, rot, DirectX::XMFLOAT3(5, 10, 5));
	m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	lever = dynamic_cast<Lever*>(m_gameObjects.back());

	// Leaver Handle
	pos = DirectX::XMVectorSet(-45, 2, 0, 1); //world pos
	rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	addGameObjectToRoom(true, false, 1, &m_models->at("LeverHandle.bff"), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	leverHandle = m_gameObjects.back();
}

void EdvinsRoom::onWin()
{
	guessPos = 6;
	//Activate portal
	StatusTextHandler::get().sendText("You guessed the correct order!\n           Portal activated", 5);
	roomPortal->setPosition(XMVectorSet(-270, 0, 200, 1));

	//Activate Portal Lights
	getLight(1)->range = 4.f;
	getLight(2)->range = 4.f;
	
	//Activate particalas

}

void EdvinsRoom::onFail()
{
	//Reset Pos
	m_player->respawn();

	//Reset lever
	canPullLever = true;
	leverHandle->getMoveCompPtr()->rotation = XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	
	//Reset Buttons
	guessPos = 0;
	buttonsPosY = -6;
	for (int i = 0; i < 5; i++)
	{
		buttons[i]->getMoveCompPtr()->position = buttons[i]->getMoveCompPtr()->position + DirectX::XMVectorSet(0.f, -9.15f, 0.f, 10.f);
		//Reset player guess array
		guessOrder[i] = -1;
	}

	//Add Time
	StatusTextHandler::get().sendText("You guessed the wrong order!\n         5 sec added to timer", 5);
	m_gameTimerPointer->addTime(5);
}

bool EdvinsRoom::CorrectOrder(int arr1[], int arr2[])
{
	for (int i = 0; i < 5; i++)
	{
		if (arr1[i] != arr2[i])
		{
			return false;
		}
	}

	return true;
}


//Public:

EdvinsRoom::EdvinsRoom()
{
	Room::initParent();
	m_entrencePosition = { -60, -3, -1, 1 };

	//Green light from portal
	PointLight light;
	light.position = { -269.f, 3.f, 200.f };
	light.diffuse = { 0.f, 0.5f, 0.f };
	light.range = 10.f;

	int lightID = createLight(light); //0

	// Right:
	PointLight rightLight;
	rightLight.position = { -264.f, 4.f, 205.f };
	rightLight.diffuse = { 1.f, 0.f, 0.f };
	rightLight.range = 0.f;

	lightID = createLight(rightLight); //1

	//Left:
	PointLight leftLight;
	leftLight.position = { -264.f, 4.f, 195.f };
	leftLight.diffuse = { 1.f, 0.f, 0.f };
	leftLight.range = 0.f;

	lightID = createLight(leftLight); //2
}

EdvinsRoom::~EdvinsRoom()
{
	Room::~Room();
}

void EdvinsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	if (!m_player->getIsSpawning())
	{
		Room::update(dt, camera, activeRoom, activeRoomChanged);

		// -------- Lever Stuff -------- //

		lever->collidesWithPlayer();
		if (lever->getCanUseLever() == true && canPullLever == true) //Is inside lever BB
		{
			StatusTextHandler::get().sendText("Press E to interact", 0.01);
			if (m_player->getinUse() == true && tempLever == false) //And press E on the BB
			{
				for (int i = 0; i < 5; i++)
				{
					//Reset player guess array
					guessOrder[i] = -1;

					//GenerateArr
					int number = rand() % 5;
					hyroglajfArr[i] = number;

					//Set texture
					std::wstring path = L"Textures/Hyroglajf_" + std::to_wstring(number) + L".png";
					m_bricks[i]->setTexturePath(path);

				}

				guessPos = 0;
				tempLever = true;
				canMoveCover = true;
				timer.restart();
				leverTimer.restart();
				canPullLever = false;
				moveLever = true;
			}
		}

		if (leverTimer.timeElapsed() < 1)
		{
			if (moveLever == true)
			{
				leverHandle->getMoveCompPtr()->rotation += DirectX::XMVectorSet(0, 0, pMath::convertDegreesToRadians(90) * dt, 10);
			}
		}

		if (leverTimer.timeElapsed() >= 1 && canMoveCover == true)
		{
			moveCoverUp = true; //Call moveCover
			canMoveCover = false;
			moveLever = false;
		}

		// -------- Cover Stuff -------- //

		if (moveCoverUp == true) //Move cover up
		{
			if (XMVectorGetX(coverPos) >= -11) //Move X
			{
				cover->getMoveCompPtr()->position = cover->getMoveCompPtr()->position + DirectX::XMVectorSet(-5 * dt, 0, 0, 10);
				cover->getMoveCompPtr()->position = cover->getMoveCompPtr()->position + DirectX::XMVectorSet(0, 8 * dt, 0, 10);
				coverPos += XMVectorSetX(coverPos, -5 * dt);
			}

			else
			{
				coverTimer.restart();
				moveCoverDown = true;
				moveCoverUp = false;
			}
		}

		if (coverTimer.timeElapsed() >= 5 && moveCoverDown == true) //Move cover down
		{
			if (XMVectorGetX(coverPos) <= -7) //Move X
			{
				cover->getMoveCompPtr()->position = cover->getMoveCompPtr()->position + DirectX::XMVectorSet(5 * dt, 0, 0, 10);
				cover->getMoveCompPtr()->position = cover->getMoveCompPtr()->position + DirectX::XMVectorSet(0, -8 * dt, 0, 10);
				coverPos += XMVectorSetX(coverPos, 5 * dt);
			}
			else
			{
				moveButtons = true;
				moveCoverDown = false;
			}
		}

		// -------- Button Stuff -------- //

		if (moveButtons == true)
		{
			for (int i = 0; i < 5; i++) //Move all buttons
			{
				if (buttonsPosY <= 40) //Move while below 5 units in Y (45 = nrOfButtons * desiredMoveYAmount = 5 * 9)
				{
					buttons[i]->getMoveCompPtr()->position = buttons[i]->getMoveCompPtr()->position + DirectX::XMVectorSet(0, 30 * dt, 0, 10); //Move one frame
					buttonsPosY += 30 * dt;
				}

				else //Stop moving
				{
					moveButtons = false;
				}

			}
		}

		if (timer.timeElapsed() >= 1)
		{
			tempLever = false;
			spinButtonIndex = -1;
		}

		if (guessPos <= 4)
		{
			buttons[0]->collidesWithPlayer();			//Press button 0 ----------------------------------
			if (buttons[0]->getCanUseLever() == true)
			{
				if (m_player->getinUse() == true && tempLever == false)
				{
					guessOrder[guessPos] = 0;
					guessPos++;
					tempLever = true;
					timer.restart();
					spinButtonIndex = 0;
				}
			}

			buttons[1]->collidesWithPlayer();			//Press button 1 ----------------------------------
			if (buttons[1]->getCanUseLever() == true)
			{
				if (m_player->getinUse() == true && tempLever == false)
				{
					guessOrder[guessPos] = 1;
					guessPos++;
					tempLever = true;
					timer.restart();
					spinButtonIndex = 1;
				}
			}

			buttons[2]->collidesWithPlayer();			//Press button 2 ----------------------------------
			if (buttons[2]->getCanUseLever() == true && tempLever == false)
			{
				if (m_player->getinUse() == true)
				{
					guessOrder[guessPos] = 2;
					guessPos++;
					tempLever = true;
					timer.restart();
					spinButtonIndex = 2;
				}
			}

			buttons[3]->collidesWithPlayer();			//Press button 3 ----------------------------------
			if (buttons[3]->getCanUseLever() == true)
			{
				if (m_player->getinUse() == true && tempLever == false)
				{
					guessOrder[guessPos] = 3;
					guessPos++;
					tempLever = true;
					timer.restart();
					spinButtonIndex = 3;
				}
			}

			buttons[4]->collidesWithPlayer();			//Press button 4 ----------------------------------
			if (buttons[4]->getCanUseLever() == true)
			{
				if (m_player->getinUse() == true && tempLever == false)
				{
					guessOrder[guessPos] = 4;
					guessPos++;
					tempLever = true;
					timer.restart();
					spinButtonIndex = 4;
				}
			}
		}

		if (spinButtonIndex != -1)
		{
			buttons[spinButtonIndex]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(0, pMath::convertDegreesToRadians(180) * dt, 0, 10);
		}

		// -------- Finish Stuff -------- //

		if (guessPos == 5)
		{
			if (CorrectOrder(guessOrder, hyroglajfArr))
			{
				onWin();
			}
			else
			{
				onFail();
			}
		}
	}
}

void EdvinsRoom::onEntrance()
{
	Room::onEntrance();
	m_player->setSpawnPosition(getEntrancePosition());
}

void EdvinsRoom::init()
{
	createSceneObjects();
	createBoundingBoxes();
	lever->setPlayerBoundingBox(m_player->getAABBPtr());
	
	srand((unsigned int)time(0));
	guessPos = 0;
	for (int i = 0; i < 5; i++)
	{
		//buttons.at(i)->setPlayerBoundingBox(m_player->getAABBPtr());
		//Reset player guess array
		guessOrder[i] = -1;
	}
}

void EdvinsRoom::portals()
{
	addPortalToRoom(XMVectorSet(0, 0, 0, 1), &m_models->at("Button.bff"),
		XMVectorSet(-100, -5, 0, 1), //Pos
		XMVectorSet(1, 1, 1, 1),    //Scale
		DirectX::XMFLOAT3(3, 20, 5), //BBSize
		0, false);
	roomPortal = dynamic_cast<Portal*>(m_gameObjects.back());
}
