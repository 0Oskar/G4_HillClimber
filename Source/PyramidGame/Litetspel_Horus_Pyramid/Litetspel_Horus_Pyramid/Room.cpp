#include "pch.h"
#include "Room.h"

Room::Room()
{
	this->m_active = false;
	this->m_dContext = nullptr;
	this->m_device = nullptr;
	this->m_player = nullptr;
	this->m_models = nullptr;
	this->m_wvpCBuffers = nullptr;
	this->m_gameTimerPointer = nullptr;
	this->m_entrencePosition = DirectX::XMVectorZero();
	this->m_worldPosition = DirectX::XMVectorZero();
}

Room::~Room()
{

}

void Room::initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* models, std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* cBuffer, Player* player, XMVECTOR position, std::shared_ptr<DirectX::AudioEngine> audioEngine, Timer* gameTimer)
{
	this->m_device = device;
	this->m_dContext = dContext;
	this->m_player = player;
	this->m_worldPosition = position;
	this->m_wvpCBuffers = cBuffer;
	this->m_models = models;
	this->audioEngine = audioEngine;
	this->m_gameTimerPointer = gameTimer;
}
void Room::initParent()
{
	this->m_active = false;
	this->m_dContext = nullptr;
	this->m_device = nullptr;
	this->m_player = nullptr;
	this->m_models = nullptr;
	this->m_wvpCBuffers = nullptr;
	this->m_entrencePosition = DirectX::XMVectorZero();
	this->m_worldPosition = DirectX::XMVectorZero();
}
void Room::update(float dt, Camera* camera, Room* &activeRoom, bool &activeRoomChanged)
{
	if (!this->m_active)
	{
		for (int i = 0; i < this->m_gameObjects.size(); i++)
		{
			Portal* portalPtr = dynamic_cast<Portal*>(this->m_gameObjects[i]);

			if (portalPtr != nullptr)
			{
				portalPtr->update();
				if (portalPtr->shouldChangeActiveRoom())
				{
					activeRoom = m_rooms.at(portalPtr->getRoomID());
					portalPtr->resetActiveRoomVariable();
					activeRoomChanged = true;
					activeRoom->onEntrance();
				}
			}
			else
			{
				this->m_gameObjects[i]->update(dt);
			}

			VS_CONSTANT_BUFFER wvpData;
			DirectX::XMMATRIX viewPMtrx = camera->getViewMatrix() * camera->getProjectionMatrix();
			wvpData.wvp = this->m_gameObjects[i]->getWorldMatrix() * viewPMtrx;
			wvpData.worldMatrix = this->m_gameObjects[i]->getWorldMatrix();
			
			this->m_wvpCBuffers->at(this->m_gameObjects.at(i)->getWvpCBufferIndex()).upd(&wvpData);

		}
	}
}
void Room::setActive(bool activityStatus)
{
	this->m_active = activityStatus;
}
void Room::addGameObjectToRoom(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration)
{
	this->m_gameObjects.emplace_back(new GameObject());
	GameObject* gObject = this->m_gameObjects.back();
	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	int bufferIndex = (int)m_wvpCBuffers->size() - 1;
	if (dynamic)
	{
		gObject->initializeDynamic(colide, false, mdlIndx, bufferIndex, weight, acceleration, deceleration, mdl);
	}
	else
	{
		gObject->initializeStatic(colide, mdlIndx, bufferIndex, mdl);
	}

	gObject->setScale(scale3D);
	gObject->setPosition(m_worldPosition + position);

	if (colide)
	{
		this->m_player->addAABB(gObject->getAABBPtr());
	}
	if (boundingBoxSize.x == 0 && boundingBoxSize.y == 0 && boundingBoxSize.z == 0)
	{

	}
	else
		gObject->setBoundingBox(boundingBoxSize);
}
void Room::addPlatformToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox, BoundingOrientedBox* pyramid)
{
	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(new Platform());
	dynamic_cast<Platform*>(this->m_gameObjects.back())->init(true, mdlIndex, (int)m_wvpCBuffers->size() - 1, pyramid, mdl);

	XMVECTOR pos = m_worldPosition + position;
	this->m_gameObjects.back()->setBoundingBox(platformBoundingBox);
	this->m_gameObjects.back()->setPosition(pos);
	this->m_player->addAABB(this->m_gameObjects.back()->getAABBPtr());
}

void Room::addPortalToRoom(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room)
{
	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	int bufferIndex = (int)m_wvpCBuffers->size() - 1;

	XMVECTOR pos = m_worldPosition + teleportLocation;
	this->m_gameObjects.emplace_back(new Portal());
	if(room != -1)
		dynamic_cast<Portal*>(this->m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers->size() - 1, mdl, this->m_rooms[room]->getEntrancePosition(), this->m_player, room);
	else
		dynamic_cast<Portal*>(this->m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers->size() - 1, mdl, pos, this->m_player, room);

	pos = m_worldPosition + position;

	this->m_gameObjects.back()->setScale(scale3D);
	this->m_gameObjects.back()->setPosition(pos);
	this->m_gameObjects.back()->setBoundingBox(boundingBoxSize);
	//this->m_player.addAABB(this->m_gameObjects.back()->getAABBPtr());
}

void Room::addLeverToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB)
{
	XMVECTOR pos = m_worldPosition + position;

	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(new Lever());
	dynamic_cast<Lever*>(this->m_gameObjects.back())->init(false, mdlIndex, (int)m_wvpCBuffers->size() - 1, mdl);
	this->m_gameObjects.back()->setPosition(pos);
	this->m_gameObjects.back()->setBoundingBox(leverBB);
	this->m_gameObjects.back()->getMoveCompPtr()->rotation = rotation;
}

void Room::addObjectToRoom(GameObject* object)
{
	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(object);
	this->m_gameObjects.back()->setWvpCBufferIndex((int)m_wvpCBuffers->size() - 1);
}

std::vector<GameObject*>* Room::getGameObjectsPtr()
{
	return &this->m_gameObjects;
}

std::vector<BoundingBox>* Room::getBoundingBoxPtr()
{
	return &this->m_boundingBoxes;
}

void Room::addBoundingBox(XMVECTOR position, XMFLOAT3 extends)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, this->m_worldPosition + position);
	this->m_boundingBoxes.emplace_back(roomPos, extends);	
}

void Room::addTriggerBB(XMVECTOR position, XMFLOAT3 extends)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, this->m_worldPosition + position);
	this->m_triggerBoundingBoxes.emplace_back(roomPos, extends);
}

DirectX::XMVECTOR Room::getEntrancePosition()
{
	return (this->m_worldPosition + this->m_entrencePosition + XMVectorSet(0.f, m_player->getAABB().Extents.y + 0.1f, 0, 0));
}

void Room::addRooms(std::vector<Room*>* rooms)
{
	for (int i = 0; i < rooms->size(); i++)
	{
		this->m_rooms.emplace_back(rooms->at(i));
	}
}

DirectX::XMVECTOR Room::getRelativePosition(DirectX::XMVECTOR pos)
{
	XMVECTOR temp = this->m_worldPosition + pos;
	return temp;
}