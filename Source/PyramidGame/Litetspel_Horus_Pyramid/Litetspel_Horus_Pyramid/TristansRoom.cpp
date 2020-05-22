#include "pch.h"
#include "TristansRoom.h"

void TristansRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	this->addBoundingBox({ 70, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Left Wall
	this->addBoundingBox({ -70, 15, 0 }, DirectX::XMFLOAT3(1, 75, 150)); //Right Wall
	this->addBoundingBox({ 0, 4.5f, 110 }, DirectX::XMFLOAT3(100, 75, 1)); //Back Wall
	this->addBoundingBox({ 0, 4.5f, -100 }, DirectX::XMFLOAT3(100, 75, 1)); //Front Wall

	this->addBoundingBox({ 0, 45, 90 }, DirectX::XMFLOAT3(100, 1, 30)); //Floor 1
	this->addBoundingBox({ 0, 45, -52 }, DirectX::XMFLOAT3(100, 1, 50)); //Floor 2
	this->addBoundingBox({ 0, 45, 0 }, DirectX::XMFLOAT3(50, 1, 100)); //Floor TEMP

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

void TristansRoom::createSceneObjects()
{
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// PuzzleRoom
	pos = DirectX::XMVectorSet(0, 45, 110, 1); // World pos
	this->addGameObjectToRoom(true, false, 1, 21, &m_models->at(21), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));

	//Bells
	//pos = DirectX::XMVectorSet(0, 4.5f + 45, 0, 1); // World pos
	pos = DirectX::XMVectorSet(0, 45, 100, 1); // World pos
	this->addGameObjectToRoom(true, false, 1, 27, &m_models->at(27), pos, scale, XMFLOAT3(1, 1, 1), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f));

	//Front Lever Grip
	pos = DirectX::XMVectorSet(0, 48.8f, 89.15f, 1); // World pos
	this->addLeverToRoom( 28, &m_models->at(28), pos,XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[0]->setPlayerBoundingBox(this->m_player->getAABBPtr());

	//Back Lever Grip 1
	pos = DirectX::XMVectorSet(15, 48.8f, 73.25f, 1); // World pos
	this->addLeverToRoom(28, &m_models->at(28), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[1]->setPlayerBoundingBox(this->m_player->getAABBPtr());

	//Back Lever Grip 2
	pos = DirectX::XMVectorSet(0, 48.8f, 73.25f, 1); // World pos
	this->addLeverToRoom(28, &m_models->at(28), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[2]->setPlayerBoundingBox(this->m_player->getAABBPtr());

	//Back Lever Grip 3
	pos = DirectX::XMVectorSet(-15, 48.8f, 73.25f, 1); // World pos
	this->addLeverToRoom(28, &m_models->at(28), pos, XMVectorZero(), XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = XMVectorZero();
	this->leverGrip.emplace_back(dynamic_cast<Lever*>(m_gameObjects.back()));
	this->leverGrip[3]->setPlayerBoundingBox(this->m_player->getAABBPtr());
}

void TristansRoom::onCompleted()
{

}

TristansRoom::TristansRoom()
{
	Room::initParent();
	this->m_entrencePosition = { 0, 42 , 100, 1 };
}

TristansRoom::~TristansRoom()
{
	Room::~Room();
}

void TristansRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);
	//Spikes
	for (int i = 0; i < SpikesBB.size(); i++)
	{
		if (SpikesBB[i].Intersects(this->m_player->getAABB()))
		{
			this->m_player->getMoveCompPtr()->position = this->getRelativePosition(DirectX::XMVectorSet(0, 55, 95, 1));
		}
	}
	//Front Lever
	this->leverGrip[0]->collidesWithPlayer();
	if (this->leverGrip[0]->getCanUseLever() == true)
	{
		if (this->m_player->getinUse() == true && this->tempLever0 == false && this->canPullLever0 == true)
		{
			this->tempLever0 = true;
			this->leverTimer[0].restart();
			this->canPullLever0 = false;
			this->moveLever0 = true;
		}
	}
	if (this->leverTimer[0].timeElapsed() < 1)
	{
		if (this->moveLever0 == true)
		{
			this->leverGrip[0]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0 , 10);
		}
	}
	if (this->leverTimer[0].timeElapsed() >= 1)
	{
		this->moveLever0 = false;
	}
	//Back Lever 1
	this->leverGrip[1]->collidesWithPlayer();
	if (this->leverGrip[1]->getCanUseLever() == true)
	{
		if (this->m_player->getinUse() == true && this->tempLever1 == false && this->canPullLever1 == true)
		{
			this->tempLever1 = true;
			this->leverTimer[1].restart();
			this->canPullLever1 = false;
			this->moveLever1 = true;
		}
	}
	if (this->leverTimer[1].timeElapsed() < 1)
	{
		if (this->moveLever1 == true)
		{
			this->leverGrip[1]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
		}
	}
	if (this->leverTimer[1].timeElapsed() >= 1)
	{
		this->moveLever1 = false;
	}
	//Back Lever 2
	this->leverGrip[2]->collidesWithPlayer();
	if (this->leverGrip[2]->getCanUseLever() == true)
	{
		if (this->m_player->getinUse() == true && this->tempLever2 == false && this->canPullLever2 == true)
		{
			this->tempLever2 = true;
			this->leverTimer[2].restart();
			this->canPullLever2 = false;
			this->moveLever2 = true;
		}
	}
	if (this->leverTimer[2].timeElapsed() < 1)
	{
		if (this->moveLever2 == true)
		{
			this->leverGrip[2]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
		}
	}
	if (this->leverTimer[2].timeElapsed() >= 1)
	{
		this->moveLever2 = false;
	}
	//Back Lever 3
	this->leverGrip[3]->collidesWithPlayer();
	if (this->leverGrip[3]->getCanUseLever() == true)
	{
		if (this->m_player->getinUse() == true && this->tempLever3 == false && this->canPullLever3 == true)
		{
			this->tempLever3 = true;
			this->leverTimer[3].restart();
			this->canPullLever3 = false;
			this->moveLever3 = true;
		}
	}
	if (this->leverTimer[3].timeElapsed() < 1)
	{
		if (this->moveLever3 == true)
		{
			this->leverGrip[3]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
		}
	}
	if (this->leverTimer[3].timeElapsed() >= 1)
	{
		this->moveLever3 = false;
	}

	//levers
	/*for (int i = 0; i < 3; i++)
	{
		this->leverGrip[i]->collidesWithPlayer();
		if (this->leverGrip[i]->getCanUseLever() == true)
		{
			if (this->m_player->getinUse() == true && this->tempLever == false && this->canPullLever == true)
			{
				this->tempLever = true;
				this->leverTimer[i].restart();
				this->canPullLever = false;
				this->moveLever = true;
			}
		}
		if (this->leverTimer[i].timeElapsed() < 1)
		{
			if (this->moveLever == true)
			{
				this->leverGrip[i]->getMoveCompPtr()->rotation += DirectX::XMVectorSet(pMath::convertDegreesToRadians(90) * dt, 0, 0, 10);
			}
		}
		if (this->leverTimer[i].timeElapsed() >= 1)
		{
			this->moveLever = false;
		}
	}*/
}

void TristansRoom::onEntrance()
{

}

void TristansRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
}

void TristansRoom::portals()
{

}
