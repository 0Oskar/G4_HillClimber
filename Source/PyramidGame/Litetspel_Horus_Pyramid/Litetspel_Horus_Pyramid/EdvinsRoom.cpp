#include "pch.h"
#include "EdvinsRoom.h"

//TODO:
//Oriented bounding box
//Particalas

void EdvinsRoom::createBoundingBoxes()
{
	this->addBoundingBox({ 0, 6, 0, 1 }, DirectX::XMFLOAT3(1, 12, 20)); //Back Wall
	this->addBoundingBox({ -70, 6, 0, 1 }, DirectX::XMFLOAT3(1, 12, 6)); //Behind portal
	this->addBoundingBox({ -40, 6, -20, 1 }, DirectX::XMFLOAT3(30, 12, 1)); //Right Wall
	this->addBoundingBox({ -40, 6, 20, 1 }, DirectX::XMFLOAT3(30, 12, 1)); //Left Wall
	this->addBoundingBox({ -45, 0, 0, 1 }, DirectX::XMFLOAT3(1.5, 2, 1.5)); //Pedistal
}

void EdvinsRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);


	// PuzzleRoom 
	pos = DirectX::XMVectorSet(0, 3, 0, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 8, &m_models->at(8), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	
	// Pedistal
	scale = DirectX::XMVectorSet(2, 2, 2, 1);
	pos = DirectX::XMVectorSet(-45, -1.3, 0, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 17, &m_models->at(17), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));

	// Cover
	scale = DirectX::XMVectorSet(1, 1, 1, 1);
	this->addGameObjectToRoom(true, false, 1, 18, &m_models->at(18), this->coverPos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->cover = this->m_gameObjects.back();

	// Buttons:

	// Button 1 
	pos = DirectX::XMVectorSet(-10, this->buttonsPosY, 10, 10); //world pos
	this->addLeverToRoom(9, &m_models->at(9), pos, rot, XMFLOAT3(5, 10, 2));
	this->m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_0.png"); //Set texture
	this->buttons.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back())); //Add to buttons array
	this->buttons.at(0)->setPlayerBoundingBox(this->m_player->getAABBPtr()); //Create BB

	// Button 2
	pos = DirectX::XMVectorSet(-10, this->buttonsPosY, 5, 1); //world pos
	this->addLeverToRoom(9, &m_models->at(9), pos, rot, XMFLOAT3(5, 10, 2));
	this->m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_1.png");
	this->buttons.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	this->buttons.at(1)->setPlayerBoundingBox(this->m_player->getAABBPtr());

	// Button 3
	pos = DirectX::XMVectorSet(-10, this->buttonsPosY, 0, 1); //world pos
	this->addLeverToRoom(9, &m_models->at(9), pos, rot, XMFLOAT3(5, 10, 2));
	this->m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_2.png");
	this->buttons.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	this->buttons.at(2)->setPlayerBoundingBox(this->m_player->getAABBPtr());

	// Button 4
	pos = DirectX::XMVectorSet(-10, this->buttonsPosY, -5, 1); //world pos
	this->addLeverToRoom(9, &m_models->at(9), pos, rot, XMFLOAT3(5, 10, 2));
	this->m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_3.png");
	this->buttons.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	this->buttons.at(3)->setPlayerBoundingBox(this->m_player->getAABBPtr());

	// Button 5
	pos = DirectX::XMVectorSet(-10, this->buttonsPosY, -10, 1); //world pos
	this->addLeverToRoom(9, &m_models->at(9), pos, rot, XMFLOAT3(5, 10, 2));
	this->m_gameObjects.back()->setTexturePath(L"Textures/Hyroglajf_4.png");
	this->buttons.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));
	this->buttons.at(4)->setPlayerBoundingBox(this->m_player->getAABBPtr());

	// Bricks:

	// Brick 1
	pos = DirectX::XMVectorSet(-5, 10, 15, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 11, &m_models->at(11), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->bricks.emplace_back(this->m_gameObjects.back());

	// Brick 2
	pos = DirectX::XMVectorSet(-5, 10, 8, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 12, &m_models->at(12), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->bricks.emplace_back(this->m_gameObjects.back());

	// Brick 3
	pos = DirectX::XMVectorSet(-5, 10, 1, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 13, &m_models->at(13), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->bricks.emplace_back(this->m_gameObjects.back());

	// Brick 4
	pos = DirectX::XMVectorSet(-5, 10, -6, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 14, &m_models->at(14), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->bricks.emplace_back(this->m_gameObjects.back());

	// Brick 5
	pos = DirectX::XMVectorSet(-5, 10, -13, 1); //world pos
	this->addGameObjectToRoom(true, false, 1, 15, &m_models->at(15), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->bricks.emplace_back(this->m_gameObjects.back());

	// Leaver Base
	pos = DirectX::XMVectorSet(-45, 2, 0, 1); //world pos
	rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	this->addLeverToRoom(16, &m_models->at(16), pos, rot, DirectX::XMFLOAT3(5, 10, 5));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	lever = dynamic_cast<Lever*>(m_gameObjects.back());

	// Leaver Handle
	pos = DirectX::XMVectorSet(-45, 2, 0, 1); //world pos
	rot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	this->addGameObjectToRoom(true, false, 1, 20, &m_models->at(20), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(2.f, 2.f, 2.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	this->leverHandle = this->m_gameObjects.back();
}

void EdvinsRoom::onWin()
{
	OutputDebugStringA("\nYou won!\n");
	//Activate portal
	this->roomPortal->setPosition(XMVectorSet(-70, -5, -100, 1));

	//Activate particalas

}

void EdvinsRoom::onFail()
{
	OutputDebugStringA("\nYou lost!\n");
	//Reset Pos
	this->m_player->getMoveCompPtr()->position = { -58, 5, -100, 1 };

	//Reset lever
	this->canPullLever = true;
	this->leverHandle->getMoveCompPtr()->rotation = XMVectorSet(0.f, pMath::convertDegreesToRadians(90), 0, 1);
	
	//Reset Buttons
	for (int i = 0; i < 5; i++)
	{
		buttons[i]->getMoveCompPtr()->position = buttons[i]->getMoveCompPtr()->position + DirectX::XMVectorSet(0, -10, 0, 10);
	}
	this->buttonsPosY = -6;

	//Add Time
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
	this->m_entrencePosition = { -60, 5, 0, 1 };
}

EdvinsRoom::~EdvinsRoom()
{
	Room::~Room();
}

void EdvinsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);

	// -------- Lever Stuff -------- //

	this->lever->collidesWithPlayer();
	if (this->lever->getCanUseLever() == true) //Is inside lever BB
	{
		if (this->m_player->getinUse() == true && this->tempLever == false && this->canPullLever == true) //And press E on the BB
		{
			for (int i = 0; i < 5; i++)
			{
				//Reset player guess array
				this->guessOrder[i] = -1;

				//GenerateArr
				int number = rand() % 5;
				this->hyroglajfArr[i] = number;

				//Set texture
				std::wstring path = L"Textures/Hyroglajf_" + std::to_wstring(number) + L".png";
				this->bricks[i]->setTexturePath(path);

			}

			this->guessPos = 0;
			this->tempLever = true;
			this->canMoveCover = true;
			this->timer.restart();
			this->leverTimer.restart();
			this->canPullLever = false;
			this->moveLever = true;
		}
	}

	if (this->leverTimer.timeElapsed() < 1)
	{
		if (this->moveLever == true)
		{
			this->leverHandle->getMoveCompPtr()->rotation += DirectX::XMVectorSet(0, 0, pMath::convertDegreesToRadians(90) * dt, 10);
		}
	}

	if (this->leverTimer.timeElapsed() >= 1 && this->canMoveCover == true)
	{
		this->moveCoverUp = true; //Call moveCover
		this->canMoveCover = false;
		this->moveLever = false;
	}
	
	// -------- Cover Stuff -------- //

	if (this->moveCoverUp == true) //Move cover up
	{
		if (XMVectorGetX(this->coverPos) >= -11) //Move X
		{
			this->cover->getMoveCompPtr()->position = this->cover->getMoveCompPtr()->position + DirectX::XMVectorSet(-5 * dt, 0, 0, 10);
			this->cover->getMoveCompPtr()->position = this->cover->getMoveCompPtr()->position + DirectX::XMVectorSet(0, 8 * dt, 0, 10);
			this->coverPos += XMVectorSetX(this->coverPos, -5 * dt);
		}

		else
		{
			this->coverTimer.restart();
			this->moveCoverDown = true;
			this->moveCoverUp = false;
		}
	}

	if (this->coverTimer.timeElapsed() >= 5 && this->moveCoverDown == true) //Move cover down
	{
		if (XMVectorGetX(this->coverPos) <= -6) //Move X
		{
			this->cover->getMoveCompPtr()->position = this->cover->getMoveCompPtr()->position + DirectX::XMVectorSet(5 * dt, 0, 0, 10);
			this->cover->getMoveCompPtr()->position = this->cover->getMoveCompPtr()->position + DirectX::XMVectorSet(0, -8 * dt, 0, 10);
			this->coverPos += XMVectorSetX(this->coverPos, 5 * dt);
		}
		else
		{
			this->moveButtons = true;
			this->moveCoverDown = false;
		}
	}

	// -------- Button Stuff -------- //

	if (this->moveButtons == true)
	{
		for (int i = 0; i < 5; i++) //Move all buttons
		{
			if (this->buttonsPosY <= 45) //Move while below 5 units in Y (45 = nrOfButtons * desiredMoveYAmount = 5 * 9)
			{
				buttons[i]->getMoveCompPtr()->position = buttons[i]->getMoveCompPtr()->position + DirectX::XMVectorSet(0, 30 * dt, 0, 10); //Move one frame
				this->buttonsPosY += 30 * dt;
			}

			else //Stop moving
			{
				this->moveButtons = false;
			}

		}
	}

	if (timer.timeElapsed() >= 1)
	{
		this->tempLever = false;
		this->spinButtonIndex = -1;
	}

	if (guessPos <= 4)
	{
		this->buttons[0]->collidesWithPlayer();			//Press button 0 ----------------------------------
		if (this->buttons[0]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true && this->tempLever == false)
			{
				this->guessOrder[guessPos] = 0;
				this->guessPos++;
				this->tempLever = true;
				timer.restart();
				this->spinButtonIndex = 0;
			}
		}

		this->buttons[1]->collidesWithPlayer();			//Press button 1 ----------------------------------
		if (this->buttons[1]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true && this->tempLever == false)
			{
				this->guessOrder[guessPos] = 1;
				this->guessPos++;
				this->tempLever = true;
				timer.restart();
				this->spinButtonIndex = 1;
			}
		}

		this->buttons[2]->collidesWithPlayer();			//Press button 2 ----------------------------------
		if (this->buttons[2]->getCanUseLever() == true && this->tempLever == false)
		{
			if (this->m_player->getinUse() == true)
			{
				this->guessOrder[guessPos] = 2;
				this->guessPos++;
				this->tempLever = true;
				timer.restart();
				this->spinButtonIndex = 2;
			}
		}

		this->buttons[3]->collidesWithPlayer();			//Press button 3 ----------------------------------
		if (this->buttons[3]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true && this->tempLever == false)
			{
				this->guessOrder[guessPos] = 3;
				this->guessPos++;
				this->tempLever = true;
				timer.restart();
				this->spinButtonIndex = 3;
			}
		}

		this->buttons[4]->collidesWithPlayer();			//Press button 4 ----------------------------------
		if (this->buttons[4]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true && this->tempLever == false)
			{
				this->guessOrder[guessPos] = 4;
				this->guessPos++;
				this->tempLever = true;
				timer.restart();
				this->spinButtonIndex = 4;
			}
		}
	}

	if (this->spinButtonIndex != -1)
	{
		this->buttons[this->spinButtonIndex]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(0, pMath::convertDegreesToRadians(180) * dt, 0, 10);
	}
	
	// -------- Finish Stuff -------- //

	if (this->guessPos == 5)
	{
		if (CorrectOrder(guessOrder, hyroglajfArr))
		{
			onWin();
		}
		else
		{
			onFail();
		}

		
		this->guessPos = 0;
		for (int i = 0; i < 5; i++)
		{
			//Reset player guess array
			this->guessOrder[i] = -1;
		}
	}
}

void EdvinsRoom::onEntrance()
{
	
}

void EdvinsRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
	this->lever->setPlayerBoundingBox(this->m_player->getAABBPtr());
	
	srand(time(0));
	this->guessPos = 0;
	for (int i = 0; i < 5; i++)
	{
		//this->buttons.at(i)->setPlayerBoundingBox(this->m_player->getAABBPtr());
		//Reset player guess array
		this->guessOrder[i] = -1;
	}
}

void EdvinsRoom::portals()
{
	this->addPortalToRoom(XMVectorSet(0, 0, 0, 1), 9, &m_models->at(9),
		XMVectorSet(-100, -5, 0, 1), //Pos
		XMVectorSet(1, 1, 1, 1),    //Scale
		DirectX::XMFLOAT3(3, 20, 5), //BBSize
		0, false);
	this->roomPortal = dynamic_cast<Portal*>(m_gameObjects.back());
}
