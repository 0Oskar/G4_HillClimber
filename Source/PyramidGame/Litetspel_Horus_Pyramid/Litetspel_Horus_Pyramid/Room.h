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
	std::vector<ConstBuffer<VS_WVPW_CONSTANT_BUFFER>>* m_wvpCBuffers;
	std::unordered_map<std::string, Model>* m_models;
	std::vector<DirectX::BoundingBox> m_boundingBoxes;
	std::vector<DirectX::BoundingOrientedBox> m_orientedBoundingBoxes;
	std::vector<DirectX::BoundingBox> m_triggerBoundingBoxes;
	std::vector<Room*> m_rooms;
	PS_PER_FRAME_BUFFER m_perFrameData;
	ResourceHandler* resourceHandler;
	Player* m_player;
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;
	Timer* m_gameTimerPointer;
	DirectX::XMVECTOR m_entrencePosition;
	DirectX::XMVECTOR m_worldPosition;
	GameOptions option;

	virtual void createBoundingBoxes() {};
	virtual void createSceneObjects() {};
	virtual void onCompleted() {};
	void addBoundingBox(XMVECTOR position, XMFLOAT3 extends);
	void addOrientedBoundingBox(XMVECTOR position, XMFLOAT3 extends, XMVECTOR rotation);
	void addOrientedBoundingBox(BoundingOrientedBox OBB);
	void addTriggerBB(XMVECTOR position, XMFLOAT3 extends);
public:
	Room();
	~Room();
	virtual void initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::unordered_map<std::string, Model>* models, std::vector<ConstBuffer<VS_WVPW_CONSTANT_BUFFER>>* cBuffer, Player* player, XMVECTOR position, std::shared_ptr<DirectX::AudioEngine> audioEngine, Timer* gameTimer, GameOptions option);
	void initParent();
	virtual void update(const float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged) = 0;
	virtual void init() {};
	virtual void portals() {};
	virtual void onEntrance();
	virtual void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr) { };


	void addGameObjectToRoom(const bool dynamic, const bool colide, const float weight, Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMVECTOR scale3D, const DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), const DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), const DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1));
	void addPlatformToRoom(Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox, BoundingOrientedBox* pyramid = nullptr);
	void addPortalToRoom(const XMVECTOR teleportLocation, Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMVECTOR scale3D, const DirectX::XMFLOAT3 boundingBoxSize, const int room = -1, const bool oneTimeUse = true);
	void addLeverToRoom(Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMVECTOR rotation, const DirectX::XMFLOAT3 leverBB);
	void addObjectToRoom(GameObject* object);
	void addRooms(std::vector<Room*>* rooms);
	void updatePlayerBB();
	
	int createLight(const XMFLOAT3 position, const float range, const XMFLOAT3 diffuseColor);
	int createLight(const PointLight pLight);
	void changeLight(const int index, const XMFLOAT3 position, const float range, const XMFLOAT3 diffuseColor);
	void changeLight(const int index, PointLight light);
	PointLight* getLight(const int index);
	std::vector<GameObject*>* getGameObjectsPtr();
	std::vector<BoundingBox>* getBoundingBoxPtr();
	std::vector<BoundingOrientedBox>* getOrientedBoundingBoxPtr();
	std::vector<BoundingBox>* getTriggerBoxes();
	DirectX::XMVECTOR getEntrancePosition() const;
	DirectX::XMVECTOR getRelativePosition(const DirectX::XMVECTOR pos) const;
	PS_PER_FRAME_BUFFER getPerFrameData();
	
	bool m_completed;
};