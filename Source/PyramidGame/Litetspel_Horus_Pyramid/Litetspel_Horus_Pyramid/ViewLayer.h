#pragma once

#include "GameObject.h"
#include "Model.h"
#include "Timer.h"
#include "Platform.h"
#include "Shaders.h"
#include "ShadowMapInstance.h"
#include "StatusTextHandler.h"
#include "iGameState.h"
class ViewLayer
{
private:
	// Window
	HWND m_window;
	GameOptions* m_options;

	// Device
	Microsoft::WRL::ComPtr< ID3D11Device > m_device;
	Microsoft::WRL::ComPtr< ID3D11DeviceContext > m_deviceContext;

	// Render Target
	Microsoft::WRL::ComPtr< IDXGISwapChain > m_swapChain;
	Microsoft::WRL::ComPtr< ID3D11RenderTargetView > m_outputRTV;
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };

	// Depth Buffer
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > m_depthStencilView;
	Microsoft::WRL::ComPtr< ID3D11Texture2D > m_depthStencilBuffer;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;
	
	// SamplerState
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	// Shaders
	Shaders m_shaders;

	// Shadow Mapping
	static const int SHADOW_MAP_SIZE = 2048;
	ShadowMapInstance m_shadowInstance;

	// Constant Buffer
	Microsoft::WRL::ComPtr< ID3D11Buffer > m_constantBuffer;

	// Texture Handler
	ResourceHandler* resourceHandler;

	
	// SpriteBatch
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_spriteRasterizerState;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont16;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont32;
	// Gem UI 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI0_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI1_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI2_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI3_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI4_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gemUI5_SRV;
	DirectX::XMFLOAT2 m_gemUI_Position;
	std::wstring m_currentRoomUIPath;

	// Primitive Batch
	std::unique_ptr< DirectX::CommonStates > m_states;
	std::unique_ptr< DirectX::BasicEffect > m_effect;
	std::unique_ptr< DirectX::PrimitiveBatch<DirectX::VertexPositionColor> > m_batch;
	Microsoft::WRL::ComPtr< ID3D11InputLayout > m_batchInputLayout;
	bool m_drawPrimitives;

	// Objects from state
	std::vector<GameObject*>* m_gameObjectsFromState;
	std::vector<GameObject*>* m_gameObjectsFromActiveRoom;
	std::vector<BoundingBox>* m_boundingBoxesFromActiveRoom;
	std::vector<BoundingBox>* m_triggerBoxes;
	std::vector<BoundingOrientedBox>* m_orientedBoundingBoxesFromActiveRoom;
	std::vector<Model>* m_modelsFromState;
	std::vector< ConstBuffer<VS_CONSTANT_BUFFER> >* m_wvpCBufferFromState;
	DirectX::BoundingOrientedBox m_pyramidOBB;
	constantBufferData* m_constantBufferDataFromStatePtr;

	ConstBuffer<PS_LIGHT_BUFFER> m_lightBuffer;
	ConstBuffer<PS_FOG_BUFFER> m_fogBuffer;
	ConstBuffer<PS_DIR_BUFFER> m_dirLightBuffer;
	PS_DIR_BUFFER m_dirLight;

	DirectX::XMMATRIX* m_viewMatrix;
	DirectX::XMMATRIX* m_projectionMatrix;

	// Sprite and Font Rendering
	


	// FPS Counter
	Timer m_timer;
	std::string m_fpsString;
	int m_fps;

	// Status Text
	StatusTextHandler* m_statusTextHandler;

	// Initialization Functions
	void initDeviceAndSwapChain();
	void initViewPort();
	void initDepthStencilBuffer();
	void initSamplerState();
	void initShaders();
	void initConstantBuffer();

public:
	ViewLayer();
	~ViewLayer();

	// Getters
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getContextDevice();

	// Setters
	void setViewMatrix(DirectX::XMMATRIX* ViewMatrix);
	void setProjectionMatrix(DirectX::XMMATRIX* newProjectionMatrix);
	void setRoomUITexturePath(std::wstring texturePath);

	// Setters for State Pointers
	void setgameObjectsFromState(std::vector<GameObject*>* gameObjectsFromState);
	void setgameObjectsFromActiveRoom(std::vector<GameObject*>* gameObjectsFromState);
	void setBoundingBoxesFromActiveRoom(std::vector<BoundingBox>* bbFromRoom);
	void setOrientedBoundingBoxesFromActiveRoom(std::vector<BoundingOrientedBox>* bbFromRoom);
	void setTriggerBoxFromActiveRoom(std::vector<BoundingBox>* bbFromRoom);
	void setModelsFromState(std::vector<Model>* models);
	void setDirLightFromActiveRoom(PS_DIR_BUFFER dirLight);
	void setFogDataFromActiveRoom(PS_FOG_BUFFER fogData);
	void setConstantBuffersFromGameState(constantBufferData* cbDataFromState);
	void setLightDataFromActiveRoom(PS_LIGHT_BUFFER lightData);
	void setWvpCBufferFromState(std::vector< ConstBuffer<VS_CONSTANT_BUFFER> >* models);
	// Initialization
	void initialize(HWND window, GameOptions* options);

	// Update
	void update(float dt, XMFLOAT3 cameraPos);

	// Render
	void render(iGameState* gameState);
	void toggleDrawPrimitives(bool toggle);
};