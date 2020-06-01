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
#include"finalRoom.h"
#include"PyramidRoom.h"
#include"KevinsRoom.h"
#include"EdvinsRoom.h"
#include "TristansRoom.h"
#include "FindGemsRoom.h"
#include"iGameState.h"


using namespace std;

class GameState : public iGameState
{
private:
	//Edvin stuff for map
	float playerHeight;
	float pyramidHeight;
	float playerPosY;
	float markerTop;
	float markerBottom;
	float difference;
	float markerPosY;

	Player m_player;
	Timer m_gameTime;
	DirectX::BoundingOrientedBox m_pyramidOBB;
	Room* m_activeRoom;
	int nrOfGameObjects;
  
	std::vector<Room*> m_rooms;

	std::vector<DirectX::BoundingBox*> platformBB;
	std::vector<GameObject*>* m_chainGObjects;
	

	//UI Stuff
	ID3D11ShaderResourceView* m_crossHairSRV;
	DirectX::XMFLOAT2 m_crosshairPosition;

	ID3D11ShaderResourceView* m_map;
	DirectX::XMFLOAT2 m_mapPosition;

	ID3D11ShaderResourceView* m_mapPlayer;
	DirectX::XMFLOAT2 m_mapPlayerPosition;

	std::string m_timerString;
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
	std::vector<BoundingOrientedBox>* getActiveRoomOrientedBoundingBoxPtr();
	std::vector<BoundingBox>* getActiveRoomTriggerBox();
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr();
	constantBufferData* getConstantBufferData();
	PS_DIR_BUFFER getActiveRoomDirectionalLight();
	PS_FOG_BUFFER getActiveRoomFogData();
	PS_LIGHT_BUFFER getActiveRoomLightData();
	void addGameObjectToWorld(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration);
	void addPlatformToWorld(int mdlIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl, DirectX::XMVECTOR position, DirectX::XMFLOAT3 platformBoundingBox);
	void addLeverToWorld(int mdlIndex, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMFLOAT3 leverBB);
	void addPortalToWorld(XMVECTOR teleportLocation, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize, int room);
	void looseALife(bool looseLife);
	std::wstring getRoomUITexturePath();

	// Initialization
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine);
	// Update
	void update(float dt);
	states handleInput(Keyboard* keyboard, Mouse* mouePointer, float dt);
	//void updateCustomViewLayerVariables(ViewLayer* viewLayer);
	XMFLOAT3 getCameraPos();

	void roomChangeInit();
	Timer* getGameTimerPtr();
	void afterChange();
	void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr);
};