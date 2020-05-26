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
	std::vector<DirectX::BoundingOrientedBox> m_orientedBoundingBoxes;
	std::vector<DirectX::BoundingBox> m_triggerBoundingBoxes;
	std::vector<Room*> m_rooms;
	PS_DIR_BUFFER m_dirLight;
	PS_FOG_BUFFER m_fogData;
	PS_LIGHT_BUFFER m_lightData;
	
	Player* m_player;
	std::shared_ptr<DirectX::AudioEngine> audioEngine;
	Timer* m_gameTimerPointer;
	DirectX::XMVECTOR m_entrencePosition;
	DirectX::XMVECTOR m_worldPosition;

	virtual void createBoundingBoxes() {};
	virtual void createSceneObjects() {};
	virtual void onCompleted() {};
	void addBoundingBox(XMVECTOR position, XMFLOAT3 extends);
	void addOrientedBoundingBox(XMVECTOR position, XMFLOAT3 extends, XMVECTOR rotation);
	void addTriggerBB(XMVECTOR position, XMFLOAT3 extends);
public:
	Room();
	~Room();
	virtual void initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* models, std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* cBuffer, Player* player, XMVECTOR position, std::shared_ptr<DirectX::AudioEngine> audioEngine, Timer* gameTimer);
	void initParent();
	virtual void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged) = 0;
	virtual void init() {};
	virtual void portals() {};
	virtual void onEntrance() {};

	void addGameObjectToRoom(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1));
	void addPlatformToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox, BoundingOrientedBox* pyramid = nullptr);
	void addPortalToRoom(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room = -1, bool oneTimeUse = true);
	void addLeverToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB);
	void addObjectToRoom(GameObject* object);
	void addRooms(std::vector<Room*>* rooms);
	void updatePlayerBB();
	
	int createLight(XMFLOAT3 position, float range, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 atteniation = { 1, 0, 0 });
	int createLight(PointLight pLight);
	void changeLight(int index, XMFLOAT3 position, float range, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 atteniation = { 1, 0, 0 });
	PointLight* getLight(int index);
	std::vector<GameObject*>* getGameObjectsPtr();
	std::vector<BoundingBox>* getBoundingBoxPtr();
	std::vector<BoundingOrientedBox>* getOrientedBoundingBoxPtr();
	std::vector<BoundingBox>* getTriggerBoxes();
	DirectX::XMVECTOR getEntrancePosition();
	DirectX::XMVECTOR getRelativePosition(DirectX::XMVECTOR pos);
	PS_DIR_BUFFER getDirectionalLight();
	PS_FOG_BUFFER getFogData();
	PS_LIGHT_BUFFER getLightData();


	bool m_completed;

};