#include "pch.h"
#include "Room.h"

Room::Room()
{
	initParent();
}

Room::~Room()
{
	m_player = nullptr;
	m_dContext = nullptr;
	m_device = nullptr;
	m_gameTimerPointer = nullptr;
	m_models = nullptr;
	
	m_wvpCBuffers = nullptr;
	int i = 0;
	for (auto p : m_gameObjects)
	{
		i++;
		if (p != nullptr)
		{
			delete p;
		}
	}
	m_gameObjects.clear();

	for (int i = 0; i < m_rooms.size(); i++) //Not created here. Will release in gamestate
	{
		m_rooms.at(i) = nullptr;
	}
}

void Room::initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* models, std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* cBuffer, Player* player, XMVECTOR position, std::shared_ptr<DirectX::AudioEngine> audioEngine, Timer* gameTimer, GameOptions option)
{
	m_device = device;
	m_dContext = dContext;
	m_player = player;
	m_worldPosition = position;
	m_wvpCBuffers = cBuffer;
	m_models = models;
	audioEngine = audioEngine;
	m_gameTimerPointer = gameTimer;

	resourceHandler = &ResourceHandler::get();
	option = option;
}
void Room::initParent()
{
	m_dContext = nullptr;
	m_device = nullptr;
	m_player = nullptr;
	m_models = nullptr;
	m_wvpCBuffers = nullptr;
	m_entrencePosition = DirectX::XMVectorZero();
	m_worldPosition = DirectX::XMVectorZero();
	m_dirLight.lightColor = { .8f, .8f, .8f, 1.f };
	m_dirLight.lightDirection = {-0.8f, 1.0f, -0.7f, 0.0f };
	m_fogData.cameraPos = XMFLOAT3(0, 0, 0);
	m_fogData.fogEnd = 0;
	m_fogData.fogStart = 0;
	m_fogData.fogColor = { 0.5f, 0.5f, 0.5f };
	m_lightData.lightColor = DirectX::XMFLOAT3(1.f, 1.0f, 1.0f);
	m_lightData.strength = 0.5f;
	m_lightData.nrOfPointLights = 0;
}
void Room::update(float dt, Camera* camera, Room* &activeRoom, bool &activeRoomChanged)
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		Portal* portalPtr = dynamic_cast<Portal*>(m_gameObjects[i]);

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
			m_gameObjects[i]->update(dt);
		}

		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = camera->getViewMatrix() * camera->getProjectionMatrix();
		wvpData.wvp = m_gameObjects[i]->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = m_gameObjects[i]->getWorldMatrix();

		m_wvpCBuffers->at(m_gameObjects.at(i)->getWvpCBufferIndex()).upd(&wvpData);
	}
}

void Room::onEntrance()
{
	m_gameTimerPointer->stop();
	m_player->setSpawnPosition(getEntrancePosition());
}

void Room::addGameObjectToRoom(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration)
{
	m_gameObjects.emplace_back(new GameObject());
	GameObject* gObject = m_gameObjects.back();
	m_wvpCBuffers->emplace_back();
	m_wvpCBuffers->back().init(m_device, m_dContext);
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
		if(m_player != nullptr)
			m_player->addAABB(gObject->getAABBPtr());
	}
	if (boundingBoxSize.x == 0 && boundingBoxSize.y == 0 && boundingBoxSize.z == 0)
	{

	}
	else
		gObject->setBoundingBox(boundingBoxSize);
}
void Room::addPlatformToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox, BoundingOrientedBox* pyramid)
{
	m_wvpCBuffers->emplace_back();
	m_wvpCBuffers->back().init(m_device, m_dContext);
	m_gameObjects.emplace_back(new Platform());
	dynamic_cast<Platform*>(m_gameObjects.back())->init(true, mdlIndex, (int)m_wvpCBuffers->size() - 1, pyramid, mdl);

	XMVECTOR pos = m_worldPosition + position;
	m_gameObjects.back()->setBoundingBox(platformBoundingBox);
	m_gameObjects.back()->setPosition(pos);
	if(m_player != nullptr)
		m_player->addAABB(m_gameObjects.back()->getAABBPtr());
}

void Room::addPortalToRoom(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room, bool oneTimeUse)
{
	m_wvpCBuffers->emplace_back();
	m_wvpCBuffers->back().init(m_device, m_dContext);
	int bufferIndex = (int)m_wvpCBuffers->size() - 1;

	XMVECTOR pos = m_worldPosition + teleportLocation;
	m_gameObjects.emplace_back(new Portal());
	if(room != -1)
		dynamic_cast<Portal*>(m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers->size() - 1, mdl, m_rooms[room]->getEntrancePosition() + XMVectorSet(0.f, m_player->getAABB().Extents.y + 0.1f, 0, 0), m_player, room, oneTimeUse);
	else
		dynamic_cast<Portal*>(m_gameObjects.back())->initialize(mdlIndx, (int)m_wvpCBuffers->size() - 1, mdl, pos, m_player, room, oneTimeUse);

	pos = m_worldPosition + position;

	m_gameObjects.back()->setScale(scale3D);
	m_gameObjects.back()->setPosition(pos);
	m_gameObjects.back()->setBoundingBox(boundingBoxSize);
	//m_player.addAABB(m_gameObjects.back()->getAABBPtr());
}

void Room::addLeverToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB)
{
	XMVECTOR pos = m_worldPosition + position;

	m_wvpCBuffers->emplace_back();
	m_wvpCBuffers->back().init(m_device, m_dContext);
	m_gameObjects.emplace_back(new Lever());
	dynamic_cast<Lever*>(m_gameObjects.back())->init(false, mdlIndex, (int)m_wvpCBuffers->size() - 1, mdl);
	m_gameObjects.back()->setPosition(pos);
	m_gameObjects.back()->setBoundingBox(leverBB);
	m_gameObjects.back()->getMoveCompPtr()->rotation = rotation;
}

void Room::addObjectToRoom(GameObject* object)
{
	m_wvpCBuffers->emplace_back();
	m_wvpCBuffers->back().init(m_device, m_dContext);
	m_gameObjects.emplace_back(object);
	m_gameObjects.back()->setWvpCBufferIndex((int)m_wvpCBuffers->size() - 1);
}

std::vector<GameObject*>* Room::getGameObjectsPtr()
{
	return &m_gameObjects;
}

std::vector<BoundingBox>* Room::getBoundingBoxPtr()
{
	return &m_boundingBoxes;
}

std::vector<BoundingOrientedBox>* Room::getOrientedBoundingBoxPtr()
{
	return &m_orientedBoundingBoxes;
}

std::vector<BoundingBox>* Room::getTriggerBoxes()
{
	return &m_triggerBoundingBoxes;
}

void Room::addBoundingBox(XMVECTOR position, XMFLOAT3 extends)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, m_worldPosition + position);
	m_boundingBoxes.emplace_back(roomPos, extends);	
}

void Room::addOrientedBoundingBox(XMVECTOR position, XMFLOAT3 extends, XMVECTOR rotation)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, m_worldPosition + position);
	XMVECTOR rotationVector = XMQuaternionRotationRollPitchYawFromVector(rotation);
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, rotationVector);
	m_orientedBoundingBoxes.emplace_back(roomPos, extends, rot);

}

void Room::addOrientedBoundingBox(BoundingOrientedBox OBB)
{
	m_orientedBoundingBoxes.emplace_back(OBB);
}

void Room::addTriggerBB(XMVECTOR position, XMFLOAT3 extends)
{
	XMFLOAT3 roomPos;
	XMStoreFloat3(&roomPos, m_worldPosition + position);
	m_triggerBoundingBoxes.emplace_back(roomPos, extends);
}

DirectX::XMVECTOR Room::getEntrancePosition() const
{
	if (m_player == nullptr) return XMVectorZero();
	return (m_worldPosition + m_entrencePosition + XMVectorSet(0.f, m_player->getAABB().Extents.y + 0.1f, 0, 0));
}

void Room::addRooms(std::vector<Room*>* rooms)
{
	for (int i = 0; i < rooms->size(); i++)
	{
		m_rooms.emplace_back(rooms->at(i));
	}
}

void Room::updatePlayerBB()
{
	if (m_player == nullptr) return;
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		if(m_gameObjects.at(i)->collidable())
			m_player->addAABB(m_gameObjects.at(i)->getAABBPtr());
	}
	m_player->addAABBFromVector(&m_boundingBoxes);
	m_player->addOrientedBBFromVector(&m_orientedBoundingBoxes);
}

int Room::createLight(XMFLOAT3 position, float range, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 atteniation)
{
	if(m_lightData.nrOfPointLights <= 5)
	{
		XMFLOAT3 wPos;
		XMStoreFloat3(&wPos, m_worldPosition + XMLoadFloat3(&position));
		PointLight pLight;
		pLight.plPosition = wPos;
		pLight.plRange = range;
		pLight.plAmbient = ambientColor;
		pLight.plDiffuse = diffuseColor;
		pLight.att = atteniation;

		m_lightData.pointLights[m_lightData.nrOfPointLights++] = pLight;
	}	
	else
	{
		assert(false && "Error, adding more lights to room than allowed.");
	}
	return m_lightData.nrOfPointLights - 1;
}

int Room::createLight(PointLight pLight)
{
	XMFLOAT3 wPos;
	XMStoreFloat3(&wPos, m_worldPosition + XMLoadFloat3(&pLight.plPosition));
	pLight.plPosition = wPos;
	if (m_lightData.nrOfPointLights <= 5)
	{
		m_lightData.pointLights[m_lightData.nrOfPointLights++] = pLight;
	}
	else
	{
		assert(false && "Error, adding more lights to room than allowed.");
	}
	return m_lightData.nrOfPointLights - 1;
}

void Room::changeLight(int index, XMFLOAT3 position, float range, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 atteniation)
{
	m_lightData.pointLights[index].plPosition = position;
	m_lightData.pointLights[index].plRange = range;
	m_lightData.pointLights[index].plAmbient = ambientColor;
	m_lightData.pointLights[index].plDiffuse = ambientColor;
	m_lightData.pointLights[index].att = atteniation;
}

PointLight* Room::getLight(int index)
{
	return &m_lightData.pointLights[index];
}

DirectX::XMVECTOR Room::getRelativePosition(DirectX::XMVECTOR pos) const
{
	XMVECTOR temp = m_worldPosition + pos;
	return temp;
}

PS_DIR_BUFFER Room::getDirectionalLight() const
{
	return m_dirLight;
}

PS_FOG_BUFFER Room::getFogData() const
{
	return m_fogData;
}

PS_LIGHT_BUFFER Room::getLightData() const
{
	return m_lightData;
}
