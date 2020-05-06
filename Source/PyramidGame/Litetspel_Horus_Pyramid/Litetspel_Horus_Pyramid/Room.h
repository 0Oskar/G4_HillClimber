#pragma once
#include"pch.h"
#include"GameObject.h"
#include"Player.h"
#include"Platform.h"
#include"Model.h"
#include"Camera.h"
class Room
{
protected:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

	std::vector<GameObject*> m_gameObjects;
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* m_wvpCBuffers;
	std::vector<Model>* m_models;
	std::vector<DirectX::BoundingBox> m_boundingBoxes;
	
	Player* m_player;
	bool m_active;
	DirectX::XMVECTOR m_entrencePosition;
	DirectX::XMVECTOR m_worldPosition;

	virtual void createBoundingBoxes() {}
	virtual void createSceneObjects() {}
	virtual void onCompleted() {};
	void addBoundingBox(XMVECTOR position, XMFLOAT3 extends);
public:
	Room();
	~Room();
	virtual void initialize(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Model>* models, std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>*, Player* player, XMVECTOR position);
	virtual void update(float dt, Camera* camera);
	virtual void init() {};
	virtual void onEntrance() {}
	void setActive(bool activityStatus);
	void addGameObjectToRoom(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration);
	void addPlatformToRoom(int mdlIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox);
	std::vector<GameObject*>* getGameObjectsPtr();
	std::vector<BoundingBox>* getBoundingBoxPtr();

	DirectX::XMVECTOR getEntrancePosition();
};