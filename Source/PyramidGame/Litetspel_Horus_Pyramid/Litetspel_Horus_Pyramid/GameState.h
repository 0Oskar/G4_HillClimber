#pragma once

#include "Player.h"
#include "Camera.h"
#include "Model.h"
#include "Platform.h"
#include "Portal.h"
#include "TemplateRoom.h"
#include "CheckpointHandler.h"

class GameState
{
private:
	Player m_player;
	Camera m_camera;
	
	std::vector<Model> m_models;
	std::vector<GameObject*> m_gameObjects;
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>> m_wvpCBuffers;
	DirectX::BoundingOrientedBox m_pyramidOBB;
	std::vector<GameObject*>* m_chainGObjects;
	std::vector<Room*> m_rooms;
	Room* m_activeRoom;
	CheckpointHandler m_checkpointHandler;
  
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

public:
	GameState();
	~GameState();

	// Getters
	DirectX::XMMATRIX* getViewMatrix() const;
	DirectX::XMMATRIX* getProjectionMatrix() const;
	std::vector<Model>* getModelsPtr();
	std::vector<GameObject*>* getGameObjectsPtr();
	std::vector<GameObject*>* getActiveRoomGameObjectsPtr();
	std::vector<BoundingBox>* getActiveRoomBoundingBoxsPtr();
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr();
	void addGameObjectToWorld(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration);
	void addPlatformToWorld(int mdlIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox);
	void addPortalToWorld(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize);

	// Initialization
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine);

	// Update
	void update(Keyboard* keyboard, MouseEvent mouseEvent, Mouse* mousePointer, float dt);

	bool m_activeRoomChanged;
};