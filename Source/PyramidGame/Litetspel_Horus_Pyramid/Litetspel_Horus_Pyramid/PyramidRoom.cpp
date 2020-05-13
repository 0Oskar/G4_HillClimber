#include "pch.h"
#include"PyramidRoom.h"

PyramidRoom::PyramidRoom()
{
	Room::initParent();
	this->completedRooms = 0;
}
PyramidRoom::~PyramidRoom()
{
	Room::~Room();
}

void PyramidRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
	Room::update(dt, camera, activeRoom, activeRoomChanged);

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

}

void PyramidRoom::init(DirectX::BoundingOrientedBox* pyramidBB)
{
	this->m_pyramidOOB = *pyramidBB; //DirectX::BoundingOrientedBox(*pyramidBB);
	this->createSceneObjects();
	this->createBoundingBoxes();
	this->m_player->addAABBFromVector(&m_boundingBoxes);
	this->m_player->addOrientedBBFromVector(&m_orientedBoundingBoxes);

	this->m_entrencePosition = { -25, 0, -20 };
}

void PyramidRoom::portals()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	vec = DirectX::XMVectorSet(0.f, 25.f, 45.f, 1.f);
	XMVECTOR vecScale = DirectX::XMVectorSet(1.f, 1.f, -1.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 10, &m_models->at(10), vec, NormalScale, DirectX::XMFLOAT3(3.f, 8.f, 0.6f), 2);

	vec = DirectX::XMVectorSet(10.f, 85.f, 119.f, 1.f);
	this->addPortalToRoom(XMVectorSet(0.f, 0.f, 0.f, 1.f), 10, &m_models->at(10), vec, NormalScale, DirectX::XMFLOAT3(3.f, 8.f, 0.6f), 2);
}

std::vector<DirectX::BoundingBox*> PyramidRoom::getBBForHook()
{
	return this->platformBB;
}

void PyramidRoom::onEntrance()
{
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
				platformPtr->setPosition(platformPtr->getPosition() + platformPush);
			}
		}
	}
}

void PyramidRoom::createBoundingBoxes()
{
	this->addOrientedBoundingBox({ -10, 5, -10 }, { 10, 5, 2 }, { 0, 1, 0, 0 });

}
void PyramidRoom::createSceneObjects()
{
	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//Ground
	this->addGameObjectToRoom(false, true, 0, 0, &m_models->at(0), vec, NormalScale, DirectX::XMFLOAT3(1000.f, 10.f, 1000.f));

	//Pyramid
	vec = DirectX::XMVectorSet(0.f, 0.f, 100.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 1, &m_models->at(1), vec, NormalScale);

	//platform 1
	vec = DirectX::XMVectorSet(-20.f, 20.f, 36.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 2
	vec = DirectX::XMVectorSet(-10.f, 20.f, 36.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 3
	vec = DirectX::XMVectorSet(10.f, 30.f, 48.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 4
	vec = DirectX::XMVectorSet(-40.f, 40.f, 60.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 5
	vec = DirectX::XMVectorSet(30.f, 50.f, 72.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 6
	vec = DirectX::XMVectorSet(-10.f, 60.f, 84.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 7
	vec = DirectX::XMVectorSet(20.f, 70.f, 96.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 8
	vec = DirectX::XMVectorSet(0.f, 80.f, 108.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 9
	vec = DirectX::XMVectorSet(-10.f, 90.f, 120.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 10
	vec = DirectX::XMVectorSet(10.f, 100.f, 132.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 11
	vec = DirectX::XMVectorSet(-10.f, 110.f, 144.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	// platform 12 final
	vec = DirectX::XMVectorSet(0.f, 120.f, 170.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(13.f, 0.5f, 13.f));
	vec = DirectX::XMVectorSet(5.f, 1.f, 5.f, 1.f);
	this->m_gameObjects.back()->setScale(vec);

	//2 Platform gameobjects 
	vec = DirectX::XMVectorSet(10.f, 10, 0, 1.f);
	this->addGameObjectToRoom(true, true, 2, 3, &m_models->at(3), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));
	vec = DirectX::XMVectorSet(10.f, 15, 0, 1.f);
	this->addGameObjectToRoom(true, true, 2, 3, &m_models->at(3), vec, DirectX::XMVectorSet(1, 1, 1, 1), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f));

	//Platform
	vec = DirectX::XMVectorSet(10.f, 25, 0, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	//Platform
	vec = DirectX::XMVectorSet(0.f, 10.f, 24.f, 1.f);
	this->addPlatformToRoom(3, &m_models->at(3), vec, DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &this->m_pyramidOOB);

	this->addPlatformToRoom(3, &m_models->at(3), DirectX::XMVectorSet(0, 5, 1, 1.f), DirectX::XMFLOAT3(2.5f, 0.5f, 2.5f), &m_pyramidOOB);

	// Checkpoints
	vec = DirectX::XMVectorSet(0.f, 25.f, 42.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_gameObjects.back()->setRotation({ 0.f, 1.57f, 0.f });
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	vec = DirectX::XMVectorSet(10.f, 85.f, 114.f, 1.f);
	this->addGameObjectToRoom(false, true, 0, 19, &m_models->at(19), vec, { 0.7f, 0.7f, 0.7f }, DirectX::XMFLOAT3(7.f, 1.f, 5.f));
	this->m_gameObjects.back()->setRotation({ 0.f, 1.57f, 0.f });
	this->m_checkpointHandler.addCheckpointGameObject((int)this->m_gameObjects.size() - 1, this->m_gameObjects.back()->getPosition());
	platformBB.emplace_back(this->m_gameObjects.back()->getAABBPtr());

	//Initialize audio component for platforms and add theire boundingboxes to playerBoundingBoxes
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		Platform* castToPlatform = dynamic_cast<Platform*>(this->m_gameObjects.at(i));
		if (castToPlatform != nullptr)
		{
			castToPlatform->setPlayerBoundingBox(this->m_player->getAABBPtr());
			castToPlatform->initAudioComponent(audioEngine, m_player->getMoveCompPtr());
			if (XMVectorGetY(castToPlatform->getPosition()) >= XMVectorGetY(this->m_checkpointHandler.getIndexPosAt(0).second))
				castToPlatform->setPosition(castToPlatform->getPosition() - this->platformPush);
		}
	}
}
void PyramidRoom::onCompleted()
{

}