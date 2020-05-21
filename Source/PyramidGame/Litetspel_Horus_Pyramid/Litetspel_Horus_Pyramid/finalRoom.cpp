#include "pch.h"
#include "finalRoom.h"

finalRoom::finalRoom()
{
	Room::initParent();
}
finalRoom::~finalRoom()
{
	Room::~Room();
}

void finalRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);


	////Making the axe swing + Orientedhitboxes


	timesHitString = std::to_string(timesHit);


	if (doOnceAtStart != 1)
	{
		if (rotationSwap < 130.0f)
		{
			rotationSwap += dt * swingingSpeed;
			
			swingingAxes[0]->getMoveCompPtr()->rotation -= XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);

			swingingAxes[2]->getMoveCompPtr()->rotation -= XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);
			
			if (rotationSwap > 30.0f)
			{
				swingingAxes[1]->getMoveCompPtr()->rotation += XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);

			}
			
		}
		if (rotationSwap >= 130.0f)
		{
			doOnceAtStart = 1;
		}
	}
	
	if (doOnceAtStart == 1 && flipDirection == false)
	{
		rotationSwap -= dt * swingingSpeed;
		
		swingingAxes[0]->getMoveCompPtr()->rotation += XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);
		swingingAxes[2]->getMoveCompPtr()->rotation += XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);
		swingingAxes[1]->getMoveCompPtr()->rotation -= XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);

		if (rotationSwap <= 30.0f)
		{
			flipDirection = true;
		}
	}
	else if (doOnceAtStart == 1 && flipDirection == true)
	{
		rotationSwap += dt * swingingSpeed;
		
		swingingAxes[0]->getMoveCompPtr()->rotation -= XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);
		swingingAxes[2]->getMoveCompPtr()->rotation -= XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);
		swingingAxes[1]->getMoveCompPtr()->rotation += XMVectorSet(0.0f, 0.0f, XMConvertToRadians(dt * swingingSpeed), 0.0f);
		

		if (rotationSwap >= 130.0f)
		{
			flipDirection = false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (axeBB[i].Intersects(this->m_player->getAABB()))
		{
			timesHit++;
			respawn = true;
			OutputDebugStringA(timesHitString.c_str());	
			OutputDebugStringA("\n");
		}
	}

	for (int i = 0; i < 3; i++)
	{
		aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[i]->getMoveCompPtr()->rotation + XMVectorSet(0.0f, 0.0f, XMConvertToRadians(77.0f), 0.0f));

		XMStoreFloat4(&rot, aRotation);

		this->axeBB[i].Orientation = rot;
		m_orientedBoundingBoxes[i].Orientation = rot;
	}
	

}

void finalRoom::init()
{
	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
}

void finalRoom::portals()
{
	//Add portals here
}

void finalRoom::onEntrance()
{
	//What should happen when player enter room
}
void finalRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	//Add all boundingboxes that are not part of a gameobject(Walls and ground for rooms etc)
	this->addBoundingBox({ -10.f, -1.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(40.f, 1.f, 120.f));
	this->addBoundingBox({ 18.f, 5.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 20.f, 50.f));
	this->addBoundingBox({ -43.f, 5.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 20.f, 50.f));
	this->addBoundingBox({ -43.f, 5.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 20.f, 50.f));
	this->addBoundingBox({ -13.f, 5.0f, -61.0f + 140.f, 1 }, DirectX::XMFLOAT3(50.f, 20.f, 2.f));

}
void finalRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Add all gameobjects that the room should have.
	//this->addPlatformToRoom(3, &m_models->at(3), DirectX::XMVectorSet(0, 5, 1, 1.f), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	//Demonstrate addOjvectToRoom function
	this->m_objectTest = new Lever(); //Create any child of gameobject here. In this case a lever
	this->m_objectTest->init(false, 7, -1, &m_models->at(7)); //Needs to be initialized. 
	this->addObjectToRoom(this->m_objectTest); //Function add pointer to m_gameobjects and assigns it a wvpbuffer.
	this->m_objectTest->setPosition({ 0, 0, 0 }); //We can now set positions etc

	vec = DirectX::XMVectorSet(-0.f, -4.3f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 27, &m_models->at(27), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ 0.0f, XMConvertToRadians(180), 0.0f, 0.f });


	//Swinging axes + boundingBoxes for them
	vec = DirectX::XMVectorSet(-13.4f, 13.5f, -20.1f, 1.f);
	this->addGameObjectToRoom(true, false, 2, 28, &m_models->at(28), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 2.f, 2.f));
	this->swingingAxes.emplace_back(this->m_gameObjects.back());

	vec = DirectX::XMVectorSet(-13.4f, 13.5f, -34.65f, 1.f);
	this->addGameObjectToRoom(true, false, 2, 28, &m_models->at(28), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 2.f, 2.f));
	this->swingingAxes.emplace_back(this->m_gameObjects.back());
	this->swingingAxes[1]->getMoveCompPtr()->rotation = (XMVectorSet(0.0f, 0.0f, XMConvertToRadians(230), 0.0f));
	

	vec = DirectX::XMVectorSet(-13.4f, 13.5f, -47.f, 1.f);
	this->addGameObjectToRoom(true, false, 2, 28, &m_models->at(28), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 2.f, 2.f));
	this->swingingAxes.emplace_back(this->m_gameObjects.back());

	XMStoreFloat3(&axePos, swingingAxes[0]->getMoveCompPtr()->position);
	

	aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[0]->getMoveCompPtr()->rotation);
	rot;

	XMStoreFloat4(&rot, aRotation);

	this->axeBB.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));
	this->m_orientedBoundingBoxes.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));


	XMStoreFloat3(&axePos, swingingAxes[1]->getMoveCompPtr()->position);


	aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[1]->getMoveCompPtr()->rotation);
	rot;

	XMStoreFloat4(&rot, aRotation);

	this->axeBB.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));
	this->m_orientedBoundingBoxes.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));

	XMStoreFloat3(&axePos, swingingAxes[2]->getMoveCompPtr()->position);

	aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[2]->getMoveCompPtr()->rotation);
	rot;

	XMStoreFloat4(&rot, aRotation);
	this->axeBB.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));
	this->m_orientedBoundingBoxes.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));


}
void finalRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
}