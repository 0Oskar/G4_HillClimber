#pragma once
#include "Player.h"
#include "Camera.h"
#include "Model.h"
#include "Platform.h"
#include "CheckpointHandler.h"
#include "ViewLayer.h"
#include "Lever.h"
#include "Portal.h"
#include "TemplateRoom.h"
#include "finalRoom.h"
#include "PyramidRoom.h"
#include "KevinsRoom.h"
#include "EdvinsRoom.h"
#include "TristansRoom.h"
#include "FindGemsRoom.h"
#include "iGameState.h"


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
	bool m_gameOver;
	

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
	std::vector<BoundingBox>* getActiveRoomBoundingBoxesPtr();
	std::vector<BoundingOrientedBox>* getActiveRoomOrientedBoundingBoxesPtr();
	std::vector<BoundingBox>* getActiveRoomTriggerBox();
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr();
	constantBufferData* getConstantBufferData();
	PS_DIR_BUFFER getActiveRoomDirectionalLight();
	PS_FOG_BUFFER getActiveRoomFogData();
	PS_LIGHT_BUFFER getActiveRoomLightData();
	void addGameObjectToWorld(const bool dynamic, const bool colide, const float weight, const int mdlIndx, Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMVECTOR scale3D, const DirectX::XMFLOAT3 boundingBoxSize, const DirectX::XMFLOAT3 acceleration, const DirectX::XMFLOAT3 deceleration);
	void addPlatformToWorld(const int mdlIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMFLOAT3 platformBoundingBox);
	void addLeverToWorld(const int mdlIndex, Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMVECTOR rotation, const DirectX::XMFLOAT3 leverBB);
	void addPortalToWorld(const XMVECTOR teleportLocation, const int mdlIndx, Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMVECTOR scale3D, const DirectX::XMFLOAT3 boundingBoxSize, const int room);
	void looseALife(const bool looseLife);
	std::wstring getRoomUITexturePath();

	// Initialization
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, const GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine);
	// Update
	void update(const float dt);
	states handleInput(Keyboard* keyboard, Mouse* mouePointer, const float dt);
	void highScoreCheck();
	//void updateCustomViewLayerVariables(ViewLayer* viewLayer);
	XMFLOAT3 getCameraPos() const;

	void roomChangeInit();
	Timer* getGameTimerPtr();
	void afterChange();
	void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr);
	void onPop();
};