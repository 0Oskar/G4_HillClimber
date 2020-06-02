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

	PointLight pLight;
	pLight.plPosition = { 0, 10, 0 };
	pLight.plDiffuse = { 0.3, 0.3, 0.3, 1 };
	pLight.plAmbient = { 0.0, 0.0, 0.0, 1 };
	pLight.plRange = 50;
	pLight.att = { 1, 0, 0 };

	int lightID = this->createLight(pLight); //Create light passing pointlight struct, index return position in array;
	this->getLight(lightID)->plRange = 50; //Using lightID to getLight pointer, change range to 10;
	this->createLight({ -10, 10, -10 }, 50, { 0.0f, 0.0f, 0.1f, 1.f }, { 0.5f, 0.f, 0.f, 1.f }); //Create light passing parameters
	this->m_lightData.lightColor = { 1, 1, 1, };
	this->m_lightData.strength = 0.2f;
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

	this->m_entrencePosition = { -25, 0, -20 };
}

void PyramidRoom::portals()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR vec2 = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Portals Level 1
	vec = DirectX::XMVectorSet(-160.3f, 105.f, 124.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 2);

	vec = DirectX::XMVectorSet(208.3f, 105.f, 124.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 2);
	
	//Portals Level 2
	vec = DirectX::XMVectorSet(-220.7f, 190.f, 210.f, 1.f); 
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3);

	vec = DirectX::XMVectorSet(-30.7f, 190.f, 210.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3);

	vec = DirectX::XMVectorSet(175.f, 190.f, 210.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3);

	vec = DirectX::XMVectorSet(360.f, 190.f, 210.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 3);
	
	//Portals Level 3

	vec = DirectX::XMVectorSet(-310.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 4);

	vec = DirectX::XMVectorSet(-100.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 4); 

	vec = DirectX::XMVectorSet(30.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 4); 

	vec = DirectX::XMVectorSet(330.f, 265.f, 289.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 4);

	//Portals Level 4
	vec = DirectX::XMVectorSet(0.7f, 430.f, 458.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 22, &m_models->at(22), vec, NormalScale, DirectX::XMFLOAT3(8.f, 3.f, 0.6f), 2);

	//Portals Level 5 finalRoom
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
	this->m_player->respawn();
	completedRooms++;
	XMVECTOR currentPos = this->m_checkpointHandler.getCurrentpos();
	XMVECTOR nextPos;
	float yValue;
	//Push all platforms that are between current and next checpoint out.
	if (this->completedRooms < this->m_checkpointHandler.size() )
		nextPos = this->m_checkpointHandler.getIndexPosAt(completedRooms).second;
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
	//this->addOrientedBoundingBox({ -10, 5, -10 }, { 10, 5, 2 }, { 0, 1, 0, 0 });
}

void PyramidRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	addPalmTrees();

	// Clouds
	vec = DirectX::XMVectorSet(0.f, 300.f, 315.f, 1.f);
	NormalScale = DirectX::XMVectorSet(0.3f, 0.3f, 0.3f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 46, &m_models->at(46), vec, NormalScale);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = DirectX::XMVectorSet(0, rand() % 360, 0, 1);
	this->clouds = this->m_gameObjects.back();

	// Ground
	NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 0, &m_models->at(0), vec, NormalScale, DirectX::XMFLOAT3(1000.f, 10.f, 1000.f));

	// Pyramid
	vec = DirectX::XMVectorSet(0.f, 0.f, 648.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 1, &m_models->at(1), vec, NormalScale);


	//MAX DISTANCE I X mellan platforms, 30.f
	//

	//Left Path

	//// platform 1
	vec = DirectX::XMVectorSet(-56.3f, 44.f, 61.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-86.3f, 59.f, 76.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-106.3f, 79.f, 96.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-131.3f, 94.f, 111.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-141.3f, 124.f, 141.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-141.3f, 154.f, 171.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-126.3f, 167.f, 184.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//Left path Left path

	//// platform 1
	vec = DirectX::XMVectorSet(-191.3f, 94.f, 112.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-191.3f, 94.f, 112.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-208.3f, 114.f, 132.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-230.3f, 129.f, 147.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-255.3f, 151.f, 168.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-285.3f, 151.f, 168.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-255.3f, 176.f, 194.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	////// platform 1
	//vec = DirectX::XMVectorSet(-225.3f, 190.f, 208.f, 1.f);
	//this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-205.3f, 210.f, 228.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-185.3f, 225.f, 243.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-160.3f, 235.f, 253.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-135.3f, 257.f, 275.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-120.3f, 282.f, 300.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-98.3f, 300.f, 318.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-68.3f, 300.f, 318.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-48.3f, 315.f, 333.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-140.3f, 304.f, 322.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-110.3f, 304.f, 322.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-95.3f, 329.f, 347.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-73.3f, 352.f, 370.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-53.3f, 338.f, 356.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-95.3f, 375.f, 393.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-70.3f, 392.f, 417.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-40.3f, 392.f, 417.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-20.3f, 412.f, 437.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	///////Left left left path///////

	//// platform 1
	vec = DirectX::XMVectorSet(-235.3f, 210.f, 228.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-240.3f, 237.f, 255.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-260.3f, 257.f, 275.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-290.3f, 257.f, 275.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-290.3f, 272.f, 290.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-290.3f, 294.f, 312.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-270.3f, 314.f, 334.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-240.3f, 314.f, 334.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-240.3f, 334.f, 354.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-220.3f, 356.f, 376.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-200.3f, 373.f, 393.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-170.3f, 373.f, 393.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-140.3f, 373.f, 393.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-120.3f, 393.f, 413.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	////////////////////

	//// platform 1
	vec = DirectX::XMVectorSet(-140.3f, 139.f, 156.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-113.3f, 159.f, 177.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-93.3f, 186.f, 204.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-67.3f, 186.f, 204.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-45.3f, 206.f, 224.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-45.3f, 236.f, 254.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-15.3f, 236.f, 254.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(10.3f, 256.f, 274.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(60.3f, 280.f, 298.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(90.3f, 270.f, 288.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(105.3f, 292.f, 310.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 312.f, 330.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(155.3f, 312.f, 330.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(172.3f, 330.f, 348.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(187.3f, 345.f, 363.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(207.3f, 365.f, 383.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(237.3f, 365.f, 383.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//Right Path

	//// platform 1
	vec = DirectX::XMVectorSet(-36.3f, 24.f, 41.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-2.3f, 39.f, 56.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(32.3f, 39.f, 56.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(62.3f, 44.f, 61.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(85.3f, 49.f, 66.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(105.3f, 59.f, 76.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(85.3f, 49.f, 67.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 64.f, 82.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(138.3f, 77.f, 95.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(158.3f, 87.f,  105.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(170.3f, 100.f, 118.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(240.3f, 115.f, 132.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(262.3f, 127.f, 144.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(242.3f, 147.f, 164.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(222.3f, 167.f, 184.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(202.3f, 180.f, 200.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(152.3f, 200.f, 218.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(145.3f, 210.f, 228.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 220.f, 238.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(100.3f, 220.f, 238.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(75.3f, 235.f, 254.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(55.3f, 255.f, 274.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(2.3f, 283.f, 304.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-12.3f, 290.f, 312.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-22.3f, 295.f, 317.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-2.3f, 315.f, 337.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(15.3f, 315.f, 337.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(29.3f, 323.f, 345.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(49.3f, 333.f, 365.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(64.3f, 333.f, 365.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(79.3f, 355.f, 387.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(60.3f, 377.f, 409.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(47.3f, 395.f, 427.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	////////// Right Path Right Path //////////

	//// platform 1
	vec = DirectX::XMVectorSet(282.3f, 137.f, 155.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(297.3f, 157.f, 175.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(320.3f, 157.f, 175.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(344.3f, 170.f, 188.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(384.3f, 200.f, 218.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(400.3f, 222.f, 240.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(376.3f, 242.f, 260.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(351.3f, 257.f, 275.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(311.3f, 282.f, 301.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(291.3f, 282.f, 301.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(281.3f, 302.f, 322.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(260.3f, 327.f, 348.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(235.3f, 347.f, 368.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(215.3f, 347.f, 368.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(195.3f, 367.f, 388.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(180.3f, 392.f, 412.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(155.3f, 412.f, 432.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(125.3f, 412.f, 433.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-120.3f, 282.f, 301.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(50.3f, 312.f, 333.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(20.3f, 332.f, 353.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(0.3f, 355.f, 376.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);
	
	//// platform 1
	vec = DirectX::XMVectorSet(0.3f, 385.f, 406.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(20.3f, 415.f, 436.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(40.3f, 415.f, 436.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(70.3f, 415.f, 437.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(100.3f, 415.f, 437.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(80.3f, 295.f, 316.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(30.3f, 445.f, 471.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(55.3f, 465.f, 491.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(30.3f, 475.f, 498.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(5.3f, 485.f, 508.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-15.3f, 505.f, 528.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-35.3f, 520.f, 543.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-35.3f, 550.f, 573.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//// platform 1
	vec = DirectX::XMVectorSet(-15.3f, 570.f, 593.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);


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
			//if (XMVectorGetY(castToPlatform->getPosition()) >= XMVectorGetY(this->m_checkpointHandler.getIndexPosAt(0).second))
				//castToPlatform->pushToLocation(castToPlatform->getPosition() - this->platformPush);
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
	pos = DirectX::XMVectorSet(0, 0, -500, 1);
	rot = DirectX::XMVectorSet(0, 45, 0, 1);
	scale = DirectX::XMVectorSet(0.10, 0.10, 0.10, 1);
	this->addGameObjectToRoom(false, true, 0, 1, &m_models->at(1), pos, scale);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rot;
}

