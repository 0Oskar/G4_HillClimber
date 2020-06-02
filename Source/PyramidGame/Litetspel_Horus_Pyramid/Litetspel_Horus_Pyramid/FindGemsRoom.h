#pragma once
#include"Room.h"

// Viktor's Room

class FindGemsRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	// Lever
	std::vector<DirectX::BoundingBox*> m_LeverBB;
	std::vector<Lever*> m_Lever;

	// Gate
	GameObject* gate;
	BoundingBox* m_GateBB;
	bool m_MoveGateUp;
	DirectX::XMVECTOR gatePos = DirectX::XMVectorSet(0, 5, 34, 1);

	// Gems
	std::vector<Lever*> m_Gems;
	unsigned int m_SpawnIndex[8] = { 0,1,2,3,4,5,6,7 };
	//void showInventoryGem(int gemIndex);
	bool m_holdingGem;
	int m_gemInPlace;
	std::vector<XMVECTOR> m_PreFixedSpawnpoints;
	
	// Gem slots
	std::vector<Lever*> m_gemSlots;
	bool m_GameSlotFilled_0;
	bool m_GameSlotFilled_1;
	bool m_GameSlotFilled_2;
	bool m_GameSlotFilled_3;
	bool m_GameSlotFilled_4;

	// Gem UI 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI0_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI1_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI2_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI3_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI4_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI5_SRV;
	DirectX::XMFLOAT2 m_gemUI_Position;
	std::wstring m_currentRoomUIPath;

	// Portal
	BoundingBox* m_PortalBB;
	Portal* m_portal;
	
	// Light
	//PointLight m_PortalLight;

public:
	FindGemsRoom();
	~FindGemsRoom();

	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	std::wstring getRoomUITexturePath();
	void init();
	void portals();
	void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr);
};