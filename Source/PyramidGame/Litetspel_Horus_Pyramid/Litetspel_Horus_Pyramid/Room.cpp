#include "pch.h"
#include"Room.h"

using namespace DirectX;
Room::Room()
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

Room::~Room()
{

}

void Room::initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* models, std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* cBuffer, Player* player, XMVECTOR position)
{
	this->m_device = device;
	this->m_dContext = dContext;
	this->m_player = player;
	this->m_worldPosition = position;
	this->m_wvpCBuffers = cBuffer;
	this->m_models = models;
}
void Room::update(float dt, Camera* camera)
{
	if (!this->m_active)
	{
		for (int i = 0; i < this->m_gameObjects.size(); i++)
		{
			this->m_gameObjects.at(i)->update(dt);

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
void Room::addPlatformToRoom(int mdlIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox)
{
	this->m_wvpCBuffers->emplace_back();
	this->m_wvpCBuffers->back().init(m_device, m_dContext);
	this->m_gameObjects.emplace_back(new Platform());
	dynamic_cast<Platform*>(this->m_gameObjects.back())->init(true, mdlIndex, (int)m_wvpCBuffers->size() - 1, pyramid, mdl);

	XMVECTOR pos = m_worldPosition + position;
	this->m_gameObjects.back()->setPosition(pos);
	this->m_gameObjects.back()->setBoundingBox(platformBoundingBox);
	this->m_player->addAABB(this->m_gameObjects.back()->getAABBPtr());
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
	this->m_boundingBoxes.emplace_back(DirectX::BoundingBox(roomPos, extends));
	this->m_player->addAABB(&this->m_boundingBoxes.back());
}

DirectX::XMVECTOR Room::getEntrancePosition()
{
	return this->m_entrencePosition;
}