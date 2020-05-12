#pragma once
#include"pch.h"
#include"GameObject.h"
#include"Player.h"
#include"Platform.h"
#include"Model.h"
#include"Camera.h"
#include"Portal.h"
#include"Lever.h"

class Room
{
protected:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

	std::vector<GameObject*> m_gameObjects;
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* m_wvpCBuffers;
	std::vector<Model>* m_models;
	std::vector<DirectX::BoundingBox> m_boundingBoxes;
	std::vector<DirectX::BoundingBox> m_triggerBoundingBoxes;
	std::vector<Room*> m_rooms;
	
	Player* m_player;
	std::shared_ptr<DirectX::AudioEngine> audioEngine;
	bool m_active;
	DirectX::XMVECTOR m_entrencePosition;
	DirectX::XMVECTOR m_worldPosition;

	virtual void createBoundingBoxes() {};
	virtual void createSceneObjects() {};
	virtual void onCompleted() {};
	void addBoundingBox(XMVECTOR position, XMFLOAT3 extends);
	void addTriggerBB(XMVECTOR position, XMFLOAT3 extends);
public:
	Room();
	~Room();
	virtual void initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* models, std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>*, Player* player, XMVECTOR position, std::shared_ptr<DirectX::AudioEngine> audioEngine);
	void initParent();
	virtual void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged) = 0;
	virtual void init() {};
	virtual void portals() {};
	virtual void onEntrance() {};

	void setActive(bool activityStatus);
	void addGameObjectToRoom(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1));
	void addPlatformToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox, BoundingOrientedBox* pyramid = nullptr);
	void addPortalToRoom(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room = -1);
	void addLeverToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB);
	void addObjectToRoom(GameObject* object);
	void addRooms(std::vector<Room*>* rooms);

	std::vector<GameObject*>* getGameObjectsPtr();
	std::vector<BoundingBox>* getBoundingBoxPtr();
	DirectX::XMVECTOR getEntrancePosition();
	DirectX::XMVECTOR getRelativePosition(DirectX::XMVECTOR pos);

	bool m_completed;

};