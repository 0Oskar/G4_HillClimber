#include "pch.h"
#include "finalRoom.h"

finalRoom::finalRoom()
{
	Room::initParent();
	m_entrencePosition = XMVectorSet(-13.f, 0.f, -60.f, 1.0f);
}
finalRoom::~finalRoom()
{
	Room::~Room();
}

void finalRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	if (!m_player->getIsSpawning())
	{
		Room::update(dt, camera, activeRoom, activeRoomChanged);

		////Making the axe swing + Orientedhitboxes

		timesHitString = std::to_string(timesHit);

		//Jewel object activation
		lever->collidesWithPlayer();
		if (lever->getCanUseLever() == true)
		{
			if (m_player->getinUse() == true)
			{
				wonGame = true;
				OutputDebugStringA("WON");
				OutputDebugStringA("\n");
			}
		}


		//Do once to correct the positioning of the axes due to them being rotated differently at export

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
		//Axe collisions
		for (int i = 0; i < 3; i++)
		{
			if (axeBB[i].Intersects(m_player->getAABB()))
			{
				timesHit++;
				m_player->respawn();
				//OutputDebugStringA(timesHitString.c_str());
				//OutputDebugStringA("\n");
			}
		}

		//Make sure bounding boxes rotate with the axes
		for (int i = 0; i < 3; i++)
		{
			aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[i]->getMoveCompPtr()->rotation + XMVectorSet(0.0f, 0.0f, XMConvertToRadians(77.0f), 0.0f));

			XMStoreFloat4(&rot, aRotation);

			axeBB[i].Orientation = rot;
			m_orientedBoundingBoxes[i].Orientation = rot;
		}
	}
}

void finalRoom::init()
{
	createSceneObjects();
	createBoundingBoxes();
}

void finalRoom::portals()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Add portals here
	vec = DirectX::XMVectorSet(-13.7f, 5.f, -73.f, 1.f);
	XMVECTOR vecScale = DirectX::XMVectorSet(1.3f, 1.3f, -1.3f, 1.f);
	addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), &m_models->at("PortalGate.bff"), vec, NormalScale, DirectX::XMFLOAT3(3.f, 8.f, 0.6f), 0, false);
	m_gameObjects.back()->setRotation(XMVectorSet(0.0f, XMConvertToRadians(180), 0.0f, 0.0f));
}

void finalRoom::onEntrance()
{
	//What should happen when player enter room
	m_player->setSpawnPosition(getEntrancePosition());
}
void finalRoom::createBoundingBoxes()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	//Add all boundingboxes that are not part of a gameobject(Walls and ground for rooms etc)
	//Floor
	addBoundingBox({ -10.f, -1.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(40.f, 1.f, 120.f));
	addBoundingBox({ 18.f, 5.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 20.f, 50.f));
	addBoundingBox({ -43.f, 5.0f, -104.0f + 140.f, 1 }, DirectX::XMFLOAT3(2.f, 20.f, 50.f));
	addBoundingBox({ -13.f, 5.0f, -59.9f + 140.f, 1 }, DirectX::XMFLOAT3(50.f, 20.f, 2.f));
	addBoundingBox({ -10.f, 26.0f, -135.0f + 140.f, 1 }, DirectX::XMFLOAT3(40.f, 1.f, 85.f));
	addBoundingBox({ -13.f, 5.0f, -213.0f + 140.f, 1 }, DirectX::XMFLOAT3(50.f, 20.f, 2.f));
	addBoundingBox({ 5.f, 5.0f, -150.0f + 140.f, 1 }, DirectX::XMFLOAT3(11.5f, 20.f, 2.f));
	addBoundingBox({ -31.6f, 5.0f, -150.0f + 140.f, 1 }, DirectX::XMFLOAT3(11.5f, 20.f, 2.f));
	addBoundingBox({ -6.6f, 2.f, -53.f, 1.f }, DirectX::XMFLOAT3(1.5f, 5.f, 46.f));
	addBoundingBox({ -21.4f, 2.f, -53.f, 1.f }, DirectX::XMFLOAT3(1.5f, 5.f, 46.f));

	//Pillars boundingBoxes
	addBoundingBox({ -32.5f, 20.f, 61.f, 1.f }, DirectX::XMFLOAT3(2.f, 20.f, 2.f));
	addBoundingBox({ -32.5f, 20.f, 7.1f, 1.f }, DirectX::XMFLOAT3(2.f, 20.f, 2.f));
	addBoundingBox({ 7.2f, 20.f, 7.1f, 1.f }, DirectX::XMFLOAT3(2.f, 20.f, 2.f));
	addBoundingBox({ 7.2f, 20.f, 61.f, 1.f }, DirectX::XMFLOAT3(2.f, 20.f, 2.f));

	//Objects in room boundingBoxes
	addBoundingBox({ 10.4f, 4.f, 34.6f, 1.f }, DirectX::XMFLOAT3(4.f, 3.f, 3.3f));
	addBoundingBox({ -11.4f, 2.f, 75.2f, 1.f }, DirectX::XMFLOAT3(3.5f, 2.f, 2.f));
	addBoundingBox({ -12.7f, 2.f, 33.3f, 1.f }, DirectX::XMFLOAT3(2.7f, 2.f, 7.f));
	addBoundingBox({ -21.f, 2.f, -4.f, 1.f }, DirectX::XMFLOAT3(1.5f, 3.f, 1.5f));
	addBoundingBox({ -6.6f, 2.f, -4.f, 1.f }, DirectX::XMFLOAT3(1.5f, 3.f, 1.5f));
}
void finalRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);


	//Add all gameobjects that the room should have.
	//addPlatformToRoom(3, &m_models->at(3), DirectX::XMVectorSet(0, 5, 1, 1.f), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	//Demonstrate addOjvectToRoom function
	m_objectTest = new Lever(); //Create any child of gameobject here. In this case a lever
	m_objectTest->init(false, -1, &m_models->at("PortalGate.bff")); //Needs to be initialized. 
	addObjectToRoom(m_objectTest); //Function add pointer to m_gameobjects and assigns it a wvpbuffer.
	m_objectTest->setPosition({ 0, 0, 0 }); //We can now set positions etc

	vec = DirectX::XMVectorSet(-0.f, -4.3f, 0.f, 1.f);
	addGameObjectToRoom(false, false, 2, &m_models->at("endRoom.obj"), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	m_gameObjects.back()->setRotation({ 0.0f, XMConvertToRadians(180), 0.0f, 0.f });

	//Platform hookable hawk
	vec = DirectX::XMVectorSet(-11.3f, 14.2f, -64.9f + 140.f, 1.f);
	addPlatformToRoom(&m_models->at("platform.obj"), vec, DirectX::XMFLOAT3(4.f, 0.5f, 2.f));
	m_gameObjects.back()->setScale({ 0.0f,0.0f,0.0f, 0.0f });

	//Swinging axes + boundingBoxes for them
	vec = DirectX::XMVectorSet(-13.4f, 13.5f, -20.1f, 1.f);
	addGameObjectToRoom(true, false, 2, &m_models->at("swingingAxe.obj"), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 2.f, 2.f));
	swingingAxes.emplace_back(m_gameObjects.back());

	vec = DirectX::XMVectorSet(-13.4f, 13.5f, -34.65f, 1.f);
	addGameObjectToRoom(true, false, 2, &m_models->at("swingingAxe.obj"), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 2.f, 2.f));
	swingingAxes.emplace_back(m_gameObjects.back());
	swingingAxes[1]->getMoveCompPtr()->rotation = (XMVectorSet(0.0f, 0.0f, XMConvertToRadians(230), 0.0f));
	

	vec = DirectX::XMVectorSet(-13.4f, 13.5f, -47.f, 1.f);
	addGameObjectToRoom(true, false, 2, &m_models->at("swingingAxe.obj"), vec, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1), DirectX::XMFLOAT3(2.f, 2.f, 2.f));
	swingingAxes.emplace_back(m_gameObjects.back());

	XMStoreFloat3(&axePos, swingingAxes[0]->getMoveCompPtr()->position);
	

	aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[0]->getMoveCompPtr()->rotation);
	rot;

	XMStoreFloat4(&rot, aRotation);

	axeBB.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));
	m_orientedBoundingBoxes.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));


	XMStoreFloat3(&axePos, swingingAxes[1]->getMoveCompPtr()->position);


	aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[1]->getMoveCompPtr()->rotation);
	rot;

	XMStoreFloat4(&rot, aRotation);

	axeBB.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));
	m_orientedBoundingBoxes.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));

	XMStoreFloat3(&axePos, swingingAxes[2]->getMoveCompPtr()->position);

	aRotation = XMQuaternionRotationRollPitchYawFromVector(swingingAxes[2]->getMoveCompPtr()->rotation);
	rot;

	XMStoreFloat4(&rot, aRotation);
	axeBB.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));
	m_orientedBoundingBoxes.emplace_back(BoundingOrientedBox(axePos, XMFLOAT3(2.8f, 30.f, 0.8f), rot));

	//Lever
	vec = DirectX::XMVectorSet(-12.7f, 2.f, 73.f, 1.f);
	rotation = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(-90), pMath::convertDegreesToRadians(-270), 1.f);
	addLeverToRoom(&m_models->at("Lever.obj"), vec, rotation, DirectX::XMFLOAT3(4.f, 3.f, 4.f));

	lever = dynamic_cast<Lever*>(m_gameObjects.back());

	lever->setScale(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	lever->setPlayerBoundingBox(m_player->getAABBPtr());

	//Player spawn rotation save
	spawnRotation = m_player->getMoveCompPtr()->rotation;

}
void finalRoom::onCompleted()
{
	//What should happen when the room is completed(Ex: Play sound, move gameobject etc)
}