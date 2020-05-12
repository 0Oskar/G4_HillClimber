#pragma once
#include "Player.h"
#include "Camera.h"
#include "Model.h"
#include "Platform.h"
#include "CheckpointHandler.h"
#include "ViewLayer.h"
#include "Lever.h"
#include "Portal.h"
#include"TemplateRoom.h"
#include"PyramidRoom.h"
#include"KevinsRoom.h"
#include"EdvinsRoom.h"

using namespace std;

class GameState
{
private:
	Player m_player;
	Camera m_camera;
	DirectX::BoundingOrientedBox m_pyramidOBB;
	
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;
	Room* m_activeRoom;
  
	std::vector<Model> m_models;
	std::vector<GameObject*> m_gameObjects;
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>> m_wvpCBuffers;
	std::vector<Room*> m_rooms;

	CheckpointHandler m_checkpointHandler;
	std::vector<DirectX::BoundingBox*> platformBB;
	std::vector<GameObject*>* m_chainGObjects;
	
	void loadModels();
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
	void addLeverToWorld(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB);
	void addPortalToWorld(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room);
	void looseALife(bool looseLife);
	// Initialization
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine);
	// Update
	void update(Keyboard* keyboard, MouseEvent mouseEvent, Mouse* mousePointer, float dt);

	bool m_activeRoomChanged;
	void roomChangeInit();
};