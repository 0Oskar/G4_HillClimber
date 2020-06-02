#include "pch.h"
#include"PyramidRoom.h"

PyramidRoom::PyramidRoom()
{
	Room::initParent();
	this->completedRooms = 0;
	this->m_fogData.fogEnd = 600.0f;
	this->m_fogData.fogStart = 300.0f;
	this->m_fogData.fogColor = { 0.5, 0.5f, 0.5f };
	//this->m_fogData.fogColor = { 0.79f, 0.67f, 0.42f };

}
PyramidRoom::~PyramidRoom()
{
	Room::~Room();
}

void PyramidRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);
	if (this->m_player == nullptr) return;
	// Checkpoints
	for (size_t i = 0; i < this->m_checkpointHandler.size(); i++)
	{
		std::pair<int, XMVECTOR> checkpoint = this->m_checkpointHandler.getIndexPosAt((int)i);
		BoundingBox checkpointAABB = this->m_gameObjects[checkpoint.first]->getAABB();
		checkpointAABB.Center.y = checkpointAABB.Center.y += 1;
		if (checkpointAABB.Intersects(this->m_player->getAABB()))
		{
			if (XMVectorGetY(checkpoint.second) > XMVectorGetY(this->m_checkpointHandler.getCurrentpos()))
			{
				this->m_checkpointHandler.setCurrent(checkpoint.first, checkpoint.second);
				this->m_player->setSpawnPosition(checkpoint.second + XMVectorSet(0.f, checkpointAABB.Extents.y + 5, 0.f, 0.f));
			}
		}
	}

	if (assignOnce == false)
	{
		rotating[0] = circleAroundObject(rotateObjects[0], XMVectorSet(palmTreePos[0].m128_f32[0], palmTreePos[0].m128_f32[1] + 10, palmTreePos[0].m128_f32[2], 0.0f), 10);
		rotating[1] = circleAroundObject(rotateObjects[1], XMVectorSet(palmTreePos[1].m128_f32[0], palmTreePos[1].m128_f32[1] + 10, palmTreePos[1].m128_f32[2], 0.0f), 10);
		rotating[2] = circleAroundObject(rotateObjects[2], XMVectorSet(palmTreePos[2].m128_f32[0], palmTreePos[2].m128_f32[1] + 10, palmTreePos[2].m128_f32[2], 0.0f), 10);
		rotating[3] = circleAroundObject(rotateObjects[3], XMVectorSet(palmTreePos[3].m128_f32[0], palmTreePos[3].m128_f32[1] + 10, palmTreePos[3].m128_f32[2], 0.0f), 10);
		rotating[4] = circleAroundObject(rotateObjects[4], XMVectorSet(palmTreePos[4].m128_f32[0], palmTreePos[4].m128_f32[1] + 10, palmTreePos[4].m128_f32[2], 0.0f), 10);
		rotating[5] = circleAroundObject(rotateObjects[5], XMVectorSet(palmTreePos[5].m128_f32[0], palmTreePos[5].m128_f32[1] + 10, palmTreePos[5].m128_f32[2], 0.0f), 10);
		rotating[6] = circleAroundObject(rotateObjects[6], XMVectorSet(3.0f, 600.f, 610.f, 0.0f), 7);

		assignOnce = true;
	}

	rotating[0].rotateAround("Y", dt);
	rotating[1].rotateAround("Y", dt);
	rotating[2].rotateAround("Y", dt);
	rotating[3].rotateAround("Y", dt);
	rotating[4].rotateAround("Y", dt);
	rotating[5].rotateAround("Y", dt);
	rotating[6].rotateAround("Y", dt);
	
	//rotateAroundObjectY(rotateObjects[0], XMVectorSet(palmTreePos[0].m128_f32[0], palmTreePos[0].m128_f32[1] + 10, palmTreePos[0].m128_f32[2], 0.0f), 10, dt);

	//DirectX::XMFLOAT3 hej;
	//XMStoreFloat3(&hej, this->m_player->getPosition());  // FIX
	//float hejY = hej.y;
	//std::string strHej = std::to_string(hejY);
	//strHej += "\n";
	//OutputDebugStringA(strHej.c_str());

	this->clouds->getMoveCompPtr()->rotation += DirectX::XMVectorSet(0, pMath::convertDegreesToRadians(1) * dt, 0, 10);
}

void PyramidRoom::init(DirectX::BoundingOrientedBox* pyramidBB)
{
	srand((unsigned int)time(0));

	this->m_pyramidOOB = *pyramidBB; //DirectX::BoundingOrientedBox(*pyramidBB);


	this->createSceneObjects();
	this->createBoundingBoxes();

	this->m_entrencePosition = XMVectorSet(-25.f, 2.f, -70.f, 1.f);
}

void PyramidRoom::portals()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR vec2 = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	// Portals Level 1
	vec = DirectX::XMVectorSet(-160.3f, 105.f, 124.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 1);

	vec = DirectX::XMVectorSet(208.3f, 105.f, 124.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 1);
	
	// Portals Level 2
	vec = DirectX::XMVectorSet(-220.7f, 190.f, 210.f, 1.f); 
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 2);

	vec = DirectX::XMVectorSet(-30.7f, 190.f, 210.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 2);

	vec = DirectX::XMVectorSet(175.f, 190.f, 210.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 2);

	vec = DirectX::XMVectorSet(360.f, 190.f, 210.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 2);
	
	// Portals Level 3
	vec = DirectX::XMVectorSet(-310.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3);

	vec = DirectX::XMVectorSet(-100.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3); 

	vec = DirectX::XMVectorSet(30.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3); 

	vec = DirectX::XMVectorSet(330.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3);

	// Portals Level 4
	vec = DirectX::XMVectorSet(0.7f, 430.f, 458.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 4);

	// Portals Level 5 finalRoom
	vec = DirectX::XMVectorSet(0.7f, 580.f, 610.f, 1.f); 
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 5);
}

std::vector<DirectX::BoundingBox*> PyramidRoom::getBBForHook()
{
	return this->platformBB;
}

void PyramidRoom::onEntrance()
{
	Room::onEntrance();
	this->m_gameTimerPointer->start();
	// Set respawn with current checkpoint
	std::pair<int, XMVECTOR> checkpoint = this->m_checkpointHandler.getCurrent();
	if (checkpoint.first != -1)
	{
		BoundingBox checkpointAABB = this->m_gameObjects[checkpoint.first]->getAABB();
		this->m_player->setSpawnPosition(checkpoint.second + XMVectorSet(0.f, checkpointAABB.Extents.y + 5, 0.f, 0.f));
	}
	this->m_player->respawn();

	completedRooms++;
	XMVECTOR currentPos = this->m_checkpointHandler.getCurrentpos();
	XMVECTOR nextPos;
	float yValue;
	int checkpointsPerLevel[]
	{
		0,
		2,
		6,
		10,
		11,
		12

	};
	//Push all platforms that are between current and next checpoint out.
	if (this->completedRooms < this->m_checkpointHandler.size() )
		nextPos = this->m_checkpointHandler.getIndexPosAt(checkpointsPerLevel[this->completedRooms]).second;
	else
		nextPos = { 0, 999999, 0 };

	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		Platform* platformPtr = dynamic_cast<Platform*>(this->m_gameObjects.at(i));
		if (platformPtr != nullptr)
		{
			yValue = XMVectorGetY(platformPtr->getPosition());
			if (yValue > XMVectorGetY(currentPos) && yValue < XMVectorGetY(nextPos))
			{
				platformPtr->pushToLocation(platformPtr->getPosition() + platformPush);
			}
		}
	}
}

void PyramidRoom::createBoundingBoxes()
{
	this->addOrientedBoundingBox(this->m_pyramidOOB);

	// Invis walls
	this->addBoundingBox({ -27, 50, -81, 0 }, { 25 , 100, 1 }); // Back
	this->addBoundingBox({ -3, 50, -20, 0 }, { 1 , 100, 60 }); // Right
	this->addBoundingBox({ -51, 50, -20, 0 }, { 1 , 100, 60 }); // Left

	// (Right)
	this->addBoundingBox({ -36, 5, -79, 0 }, { 2 , 10, 2 }); // Pillar 1
	this->addBoundingBox({ -45, 5, -50, 0 }, { 2 , 10, 2 }); // Pillar 2
	this->addBoundingBox({ -48, 5, -20, 0 }, { 2 , 10, 2 }); // Pillar 3

	// (Left)
	this->addBoundingBox({ -7, 1, -77, 0 }, { 7 , 2, 6 }); // Pillar 1
	this->addBoundingBox({ -7, 5, -56, 0 }, { 2 , 10, 2 }); // Pillar 2
	this->addBoundingBox({ -8, 17, -56, 0 }, { 3 , 2, 2.5 }); // Pillar 2 top
	this->addBoundingBox({ -7, 5, -20, 0 }, { 2 , 10, 2 }); // Pillar 3
}

void PyramidRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	addPalmTrees();
	addCactais();
	// Pillars
	vec = DirectX::XMVectorSet(-25, -1.5, -50, 1.f);
	this->addGameObjectToRoom(false, false, 0, 48, &m_models->at(48), vec, NormalScale);

	// Clouds
	vec = DirectX::XMVectorSet(0.f, 300.f, 315.f, 1.f);
	NormalScale = DirectX::XMVectorSet(0.3f, 0.3f, 0.3f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 49, &m_models->at(49), vec, NormalScale);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = DirectX::XMVectorSet(0, rand() % 360, 0, 1);
	this->clouds = this->m_gameObjects.back();

	// Ground
	NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 0, &m_models->at(0), vec, NormalScale, DirectX::XMFLOAT3(1000.f, 10.f, 1000.f));

	// Pyramid
	vec = DirectX::XMVectorSet(0.f, 0.f, 648.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 1, &m_models->at(1), vec, NormalScale);

	//Bird flying try
	vec = DirectX::XMVectorSet(-10.f, 6.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 36, &m_models->at(36), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(-35), 0.0f, 0.0f, 0.f });
	this->m_gameObjects.back()->setScale({ 0.08f,0.08f,0.08f, 0.0f });
	this->rotateObjects.emplace_back(this->m_gameObjects.back());

	//Bird flying try
	vec = DirectX::XMVectorSet(-10.f, 6.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 36, &m_models->at(36), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(-35), 0.0f, 0.0f, 0.f });
	this->m_gameObjects.back()->setScale({ 0.08f,0.08f,0.08f, 0.0f });
	this->rotateObjects.emplace_back(this->m_gameObjects.back());

	//Bird flying try
	vec = DirectX::XMVectorSet(-10.f, 6.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 36, &m_models->at(36), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(-35), 0.0f, 0.0f, 0.f });
	this->m_gameObjects.back()->setScale({ 0.08f,0.08f,0.08f, 0.0f });
	this->rotateObjects.emplace_back(this->m_gameObjects.back());

	//Bird flying try
	vec = DirectX::XMVectorSet(-10.f, 6.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 36, &m_models->at(36), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(-35), 0.0f, 0.0f, 0.f });
	this->m_gameObjects.back()->setScale({ 0.08f,0.08f,0.08f, 0.0f });
	this->rotateObjects.emplace_back(this->m_gameObjects.back());

	//Bird flying try
	vec = DirectX::XMVectorSet(-10.f, 6.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 36, &m_models->at(36), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(-35), 0.0f, 0.0f, 0.f });
	this->m_gameObjects.back()->setScale({ 0.08f,0.08f,0.08f, 0.0f });
	this->rotateObjects.emplace_back(this->m_gameObjects.back());

	//Bird flying try
	vec = DirectX::XMVectorSet(-10.f, 6.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 36, &m_models->at(36), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(-35), 0.0f, 0.0f, 0.f });
	this->m_gameObjects.back()->setScale({ 0.08f,0.08f,0.08f, 0.0f });
	this->rotateObjects.emplace_back(this->m_gameObjects.back());

	//Bird flying try
	vec = DirectX::XMVectorSet(-10.f, 6.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, false, 2, 36, &m_models->at(36), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	this->m_gameObjects.back()->setRotation({ XMConvertToRadians(-35), 0.0f, 0.0f, 0.f });
	this->m_gameObjects.back()->setScale({ 0.08f,0.08f,0.08f, 0.0f });
	this->rotateObjects.emplace_back(this->m_gameObjects.back());



	//MAX DISTANCE I X mellan platforms, 30.f
	//

	//Left Path

	//// platform 1
	vec = DirectX::XMVectorSet(-56.3f, 44.f, 61.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB); //Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-86.3f, 59.f, 77.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-106.3f, 79.f, 97.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-131.3f, 94.f, 112.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-141.3f, 124.f, 143.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-141.3f, 154.f, 174.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-126.3f, 167.f, 187.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//Left path Left path

	//// platform 1
	vec = DirectX::XMVectorSet(-191.3f, 94.f, 113.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB); //Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-208.3f, 114.f, 133.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-230.3f, 129.f, 149.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-255.3f, 151.f, 170.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-285.3f, 151.f, 170.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-255.3f, 176.f, 196.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-205.3f, 210.f, 231.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-185.3f, 225.f, 247.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-160.3f, 235.f, 257.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-135.3f, 257.f, 279.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-120.3f, 282.f, 305.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-98.3f, 300.f, 323.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-68.3f, 300.f, 323.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-48.3f, 315.f, 338.4f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-110.3f, 304.f, 328.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-95.3f, 329.f, 353.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-73.3f, 352.f, 376.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-53.3f, 338.f, 363.5f, 1.f);            
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB); //Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-95.3f, 375.f, 400.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-70.3f, 392.f, 418.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-40.3f, 392.f, 419.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-20.3f, 412.f, 439.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	///////Left left left path///////

	//// platform 1
	vec = DirectX::XMVectorSet(-235.3f, 210.f, 232.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB); //Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-240.3f, 237.f, 259.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB); //Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-260.3f, 257.f, 279.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-290.3f, 257.f, 280.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-290.3f, 272.f, 295.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-290.3f, 294.f, 317.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-270.3f, 314.f, 338.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-240.3f, 314.f, 338.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-240.3f, 334.f, 360.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-220.3f, 356.f, 382.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-200.3f, 373.f, 399.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-170.3f, 373.f, 399.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-140.3f, 373.f, 399.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-120.3f, 393.f, 418.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	////////////////////

	//// platform 1
	vec = DirectX::XMVectorSet(-140.3f, 139.f, 159.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-113.3f, 159.f, 180.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-93.3f, 186.f, 206.4f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Correctedw

	//// platform 1
	vec = DirectX::XMVectorSet(-67.3f, 186.f, 207.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-45.3f, 206.f, 227.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-45.3f, 236.f, 258.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB); //Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-15.3f, 236.f, 258.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(10.3f, 256.f, 278.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(60.3f, 280.f, 303.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(90.3f, 270.f, 293.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(105.3f, 292.f, 315.4f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 312.f, 337.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(155.3f, 312.f, 337.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(172.3f, 330.f, 354.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(187.3f, 345.f, 370.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(207.3f, 365.f, 391.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(237.3f, 365.f, 391.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//Right Path

	//// platform 1
	vec = DirectX::XMVectorSet(-36.3f, 24.f, 41.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-2.3f, 39.f, 55.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(32.3f, 39.f, 55.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(62.3f, 44.f, 61.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(85.3f, 49.f, 66.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(105.3f, 59.f, 76.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(85.3f, 49.f, 66.2f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 64.f, 81.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(138.3f, 77.f, 94.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(158.3f, 87.f,  106.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(170.3f, 100.f, 119.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(240.3f, 115.f, 134.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(262.3f, 127.f, 146.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(242.3f, 147.f, 167.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(222.3f, 167.f, 186.7f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(202.3f, 180.f, 200.4f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(152.3f, 200.f, 221.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(145.3f, 210.f, 231.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 220.f, 241.6f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(100.3f, 220.f, 241.6f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(75.3f, 235.f, 256.6f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(55.3f, 255.f, 277.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(2.3f, 283.f, 306.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-12.3f, 290.f, 313.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-22.3f, 295.f, 318.4f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-2.3f, 315.f, 339.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(15.3f, 315.f, 339.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(29.3f, 323.f, 347.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	////////// Right Path Right Path //////////

	//// platform 1
	vec = DirectX::XMVectorSet(282.3f, 137.f, 156.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(297.3f, 157.f, 177.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(320.3f, 157.f, 177.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(343.3f, 170.f, 190.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(384.3f, 200.f, 221.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(397.3f, 222.f, 244.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(375.3f, 242.f, 264.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(351.3f, 257.f, 279.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(311.3f, 282.f, 306.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(291.3f, 282.f, 305.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(281.3f, 302.f, 326.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(260.3f, 327.f, 352.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(235.3f, 347.f, 372.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(215.3f, 347.f, 372.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(195.3f, 367.f, 392.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(180.3f, 392.f, 417.6f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected
	 
	//// platform 1
	vec = DirectX::XMVectorSet(155.3f, 412.f, 438.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 412.f, 438.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(50.3f, 312.f, 337.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(20.3f, 332.f, 356.2f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(0.3f, 355.f, 380.2f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected
	
	//// platform 1
	vec = DirectX::XMVectorSet(0.3f, 385.f, 410.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(0.3f, 400.f, 426.2f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected


	//// platform 1
	vec = DirectX::XMVectorSet(20.3f, 415.f, 442.5f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(40.3f, 415.f, 442.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(70.3f, 415.f, 442.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(100.3f, 415.f, 442.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(80.3f, 295.f, 319.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(30.3f, 445.f, 473.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(55.3f, 465.f, 493.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(30.3f, 475.f, 503.2f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(5.3f, 485.f, 513.3f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-15.3f, 505.f, 534.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected
	
	//// platform 1
	vec = DirectX::XMVectorSet(-35.3f, 520.f, 551.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-35.3f, 550.f, 581.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected

	//// platform 1
	vec = DirectX::XMVectorSet(-15.3f, 570.f, 600.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);//Corrected


	////////////////////

	// Checkpoints

	//Checkpoints level 1
	vec = DirectX::XMVectorSet(-160.f, 105.f, 118.f, 1.f); 
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(208.f, 105.f, 118.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	//Checkpoints level 2
	vec = DirectX::XMVectorSet(-220.f, 190.f, 203.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(-30.f, 190.f, 203.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(175.f, 190.f, 203.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(360.f, 190.f, 203.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	//Checkpoints level 3
	vec = DirectX::XMVectorSet(-310.f, 265.f, 283.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(-100.f, 265.f, 283.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(30.f, 265.f, 283.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(330.f, 265.f, 283.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());


	//Checkpoints level 4
	vec = DirectX::XMVectorSet(0.7f, 430.f, 454.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	//Checkpoints level 5
	vec = DirectX::XMVectorSet(0.7f, 580.f, 605.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());





	//Initialize audio component for platforms and add theire boundingboxes to playerBoundingBoxes
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_gameObjects.at(i));
		if (castToPlatform != nullptr && this->m_player != nullptr)
		{
			castToPlatform->setPlayerBoundingBox(this->m_player->getAABBPtr());
			castToPlatform->initAudioComponent(audioEngine, m_player->getMoveCompPtr());
			if (XMVectorGetY(castToPlatform->getPosition()) >= XMVectorGetY(this->m_checkpointHandler.getIndexPosAt(0).second))
				castToPlatform->pushToLocation(castToPlatform->getPosition() - this->platformPush);
		}
	}
}

void PyramidRoom::onCompleted()
{

}

void PyramidRoom::addPalmTrees()
{
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR scale;
	
	//Infront section
	for (int i = 0; i < 10; i++)
	{
		int posXNumber = rand() % 800;
		int posZNumber = rand() % 400;
		int Rotnumber = rand() % 360;
		int scaleNumber = rand() % 2 + 2;
		
		pos = DirectX::XMVectorSet(posXNumber - 400, 0, posZNumber - 400 - 100, 1);

		palmTreePos.emplace_back(pos);
		rot = DirectX::XMVectorSet(0, Rotnumber, 0, 1);
		scale = DirectX::XMVectorSet(scaleNumber * 0.01, scaleNumber * 0.01, scaleNumber * 0.01, 1);
		this->addGameObjectToRoom(false, false, 0, 35, &m_models->at(35), pos, scale);
		this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	}

	//Right section
	for (int i = 0; i < 10; i++)
	{
		int posXNumber = rand() % 300;
		int posZNumber = rand() % 1200;
		int Rotnumber = rand() % 360;
		int scaleNumber = rand() % 2 + 2;
		
		pos = DirectX::XMVectorSet(posXNumber + 300 + 100, 0, posZNumber - 300, 1);
		rot = DirectX::XMVectorSet(0, Rotnumber, 0, 1);
		scale = DirectX::XMVectorSet(scaleNumber * 0.01, scaleNumber * 0.01, scaleNumber * 0.01, 1);
		this->addGameObjectToRoom(false, false, 0, 35, &m_models->at(35), pos, scale);
		this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	}

	//Left section
	for (int i = 0; i < 10; i++)
	{
		int posXNumber = rand() % 300;
		int posZNumber = rand() % 1200;
		int Rotnumber = rand() % 360;
		int scaleNumber = rand() % 2 + 2;
		
		pos = DirectX::XMVectorSet((posXNumber * -1) - 400, 0, posZNumber - 300, 1);
		rot = DirectX::XMVectorSet(0, Rotnumber, 0, 1);
		scale = DirectX::XMVectorSet(scaleNumber * 0.01, scaleNumber * 0.01, scaleNumber * 0.01, 1);
		this->addGameObjectToRoom(false, false, 0, 35, &m_models->at(35), pos, scale);
		this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	}

	// Small Pyramid
	pos = DirectX::XMVectorSet(450, 0, -650, 1);
	rot = DirectX::XMVectorSet(0, 45, 0, 1);
	scale = DirectX::XMVectorSet(0.10, 0.10, 0.10, 1);
	this->addGameObjectToRoom(false, true, 0, 1, &m_models->at(1), pos, scale);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;

	// Small Pyramid
	pos = DirectX::XMVectorSet(500, 0, -500, 1);
	rot = DirectX::XMVectorSet(0, 90, 0, 1);
	scale = DirectX::XMVectorSet(0.12, 0.12, 0.12, 1);
	this->addGameObjectToRoom(false, true, 0, 1, &m_models->at(1), pos, scale);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
}

void PyramidRoom::addCactais()
{
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR scale;

	//Infront section
	for (int i = 0; i < 4; i++)
	{
		int posXNumber = rand() % 800;
		int posZNumber = rand() % 400;
		int Rotnumber = rand() % 360;
		float scaleNumber = rand() % 2;

		pos = DirectX::XMVectorSet(posXNumber - 400, 0, posZNumber - 400 - 100, 1);
		rot = DirectX::XMVectorSet(0, Rotnumber, 0, 1);
		scale = DirectX::XMVectorSet(scaleNumber * 0.5, scaleNumber * 0.5, scaleNumber * 0.5, 1);
		this->addGameObjectToRoom(false, false, 0, 47, &m_models->at(47), pos, scale);
		this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	}

	//Right section
	for (int i = 0; i < 4; i++)
	{
		int posXNumber = rand() % 300;
		int posZNumber = rand() % 1200;
		int Rotnumber = rand() % 360;
		int scaleNumber = rand() % 2 + 2;

		pos = DirectX::XMVectorSet(posXNumber + 300 + 100, 0, posZNumber - 300, 1);
		rot = DirectX::XMVectorSet(0, Rotnumber, 0, 1);
		scale = DirectX::XMVectorSet(scaleNumber * 0.5, scaleNumber * 0.5, scaleNumber * 0.5, 1);
		this->addGameObjectToRoom(false, false, 0, 46, &m_models->at(46), pos, scale);
		this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	}

	//Left section
	for (int i = 0; i < 4; i++)
	{
		int posXNumber = rand() % 300;
		int posZNumber = rand() % 1200;
		int Rotnumber = rand() % 360;
		int scaleNumber = rand() % 2 + 2;

		pos = DirectX::XMVectorSet((posXNumber * -1) - 400, 0, posZNumber - 300, 1);
		rot = DirectX::XMVectorSet(0, Rotnumber, 0, 1);
		scale = DirectX::XMVectorSet(scaleNumber * 0.5, scaleNumber * 0.5, scaleNumber * 0.5, 1);
		this->addGameObjectToRoom(false, false, 0, 46, &m_models->at(46), pos, scale);
		this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
	}
}

