#include "pch.h"
#include "Room.h"

Room::Room()
{
	this->initParent();
}

Room::~Room()
{

}

void Room::initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* models, std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* cBuffer, Player* player, XMVECTOR position, std::shared_ptr<DirectX::AudioEngine> audioEngine, Timer* gameTimer, GameOptions option)
{
	this->m_device = device;
	this->m_dContext = dContext;
	this->m_player = player;
	this->m_worldPosition = position;
	this->m_wvpCBuffers = cBuffer;
	this->m_models = models;
	this->audioEngine = audioEngine;
	this->m_gameTimerPointer = gameTimer;

	this->resourceHandler = &ResourceHandler::get();
	this->option = option;
}
void Room::initParent()
{
	this->m_dContext = nullptr;
	this->m_device = nullptr;
	this->m_player = nullptr;
	this->m_models = nullptr;
	this->m_wvpCBuffers = nullptr;
	this->m_entrencePosition = DirectX::XMVectorZero();
	this->m_worldPosition = DirectX::XMVectorZero();
	this->m_dirLight.lightColor = { .8f, .8f, .8f, 1.f };
	this->m_dirLight.lightDirection = {-0.8f, 1.0f, -0.7f, 0.0f };
	this->m_fogData.cameraPos = XMFLOAT3(0, 0, 0);
	this->m_fogData.fogEnd = 0;
	this->m_fogData.fogStart = 0;
	this->m_fogData.fogColor = { 0.5f, 0.5f, 0.5f };
	this->m_lightData.lightColor = DirectX::XMFLOAT3(1.f, 1.0f, 1.0f);
	this->m_lightData.strength = 0.5f;
	this->m_lightData.nrOfPointLights = 0;
}
void Room::update(float dt, Camera* camera, Room* &activeRoom, bool &activeRoomChanged)
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
		if(this->m_player != nullptr)
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
	if(m_player != nullptr)
		this->m_player->addAABB(this->m_gameObjects.back()->getAABBPtr());
}

void Room::addPortalToRoom(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room, bool oneTimeUse)
{
	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	int bufferIndex = (int)m_wvpCBuffers->size() - 1;

	XMVECTOR pos = m_worldPosition + teleportLocation;
	this->m_gameObjects.emplace_back(new Portal());
	if(room != -1)
		dynamic_cast<Portal*>(this->m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers->size() - 1, mdl, this->m_rooms[room]->getEntrancePosition(), this->m_player, room, oneTimeUse);
	else
		dynamic_cast<Portal*>(this->m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers->size() - 1, mdl, pos, this->m_player, room, oneTimeUse);

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

std::vector<BoundingOrientedBox>* Room::getOrientedBoundingBoxPtr()
{
	return &this->m_orientedBoundingBoxes;
}


std::vector<BoundingBox>* Room::getTriggerBoxes()
{
	return &this->m_triggerBoundingBoxes;
}


void Room::addBoundingBox(XMVECTOR position, XMFLOAT3 extends)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, this->m_worldPosition + position);
	this->m_boundingBoxes.emplace_back(roomPos, extends);	
}

void Room::addOrientedBoundingBox(XMVECTOR position, XMFLOAT3 extends, XMVECTOR rotation)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, this->m_worldPosition + position);
	XMVECTOR rotationVector = XMQuaternionRotationRollPitchYawFromVector(rotation);
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, rotationVector);
	this->m_orientedBoundingBoxes.emplace_back(roomPos, extends, rot);

}

void Room::addTriggerBB(XMVECTOR position, XMFLOAT3 extends)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, this->m_worldPosition + position);
	this->m_triggerBoundingBoxes.emplace_back(roomPos, extends);
}

DirectX::XMVECTOR Room::getEntrancePosition()
{
	if (m_player == nullptr) return XMVectorZero();
	return (this->m_worldPosition + this->m_entrencePosition + XMVectorSet(0.f, m_player->getAABB().Extents.y + 0.1f, 0, 0));
}

void Room::addRooms(std::vector<Room*>* rooms)
{
	for (int i = 0; i < rooms->size(); i++)
	{
		this->m_rooms.emplace_back(rooms->at(i));
	}
}

void Room::updatePlayerBB()
{
	if (m_player == nullptr) return;
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		if(this->m_gameObjects.at(i)->collidable())
			this->m_player->addAABB(this->m_gameObjects.at(i)->getAABBPtr());
	}
	this->m_player->addAABBFromVector(&m_boundingBoxes);
	this->m_player->addOrientedBBFromVector(&m_orientedBoundingBoxes);
}

int Room::createLight(XMFLOAT3 position, float range, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 atteniation)
{
	if(m_lightData.nrOfPointLights <= 5)
	{
		XMFLOAT3 wPos;
		XMStoreFloat3(&wPos, this->m_worldPosition + XMLoadFloat3(&position));
		PointLight pLight;
		pLight.plPosition = wPos;
		pLight.plRange = range;
		pLight.plAmbient = ambientColor;
		pLight.plDiffuse = diffuseColor;
		pLight.att = atteniation;

		this->m_lightData.pointLights[this->m_lightData.nrOfPointLights++] = pLight;
	}	
	else
	{
		assert(false && "Error, adding more lights to room than allowed.");
	}
	return this->m_lightData.nrOfPointLights - 1;
}

int Room::createLight(PointLight pLight)
{
	XMFLOAT3 wPos;
	XMStoreFloat3(&wPos, this->m_worldPosition + XMLoadFloat3(&pLight.plPosition));
	pLight.plPosition = wPos;
	if (m_lightData.nrOfPointLights <= 5)
	{
		this->m_lightData.pointLights[this->m_lightData.nrOfPointLights++] = pLight;
	}
	else
	{
		assert(false && "Error, adding more lights to room than allowed.");
	}
	return this->m_lightData.nrOfPointLights - 1;
}

void Room::changeLight(int index, XMFLOAT3 position, float range, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 atteniation)
{
	this->m_lightData.pointLights[index].plPosition = position;
	this->m_lightData.pointLights[index].plRange = range;
	this->m_lightData.pointLights[index].plAmbient = ambientColor;
	this->m_lightData.pointLights[index].plDiffuse = ambientColor;
	this->m_lightData.pointLights[index].att = atteniation;
}

PointLight* Room::getLight(int index)
{
	return &m_lightData.pointLights[index];
}

DirectX::XMVECTOR Room::getRelativePosition(DirectX::XMVECTOR pos)
{
	XMVECTOR temp = this->m_worldPosition + pos;
	return temp;
}

PS_DIR_BUFFER Room::getDirectionalLight()
{
	return this->m_dirLight;
}

PS_FOG_BUFFER Room::getFogData()
{
	return this->m_fogData;
}

PS_LIGHT_BUFFER Room::getLightData()
{
	return this->m_lightData;
}
