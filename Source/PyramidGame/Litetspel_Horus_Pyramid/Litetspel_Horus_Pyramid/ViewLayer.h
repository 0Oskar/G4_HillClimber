#pragma once

#include "GameObject.h"
#include "Model.h"
#include "Timer.h"
#include "Platform.h"
#include "Shaders.h"
#include "ShadowMapInstance.h"
#include "StatusTextHandler.h"
#include "iGameState.h"
#include "Transition.h"

class ViewLayer
{
private:
	struct RenderTexture
	{
		ID3D11Texture2D* rtt;
		ID3D11RenderTargetView* rtv;
		ID3D11ShaderResourceView* srv;
		ID3D11UnorderedAccessView* uav;
		DXGI_FORMAT format;

		RenderTexture()
		{
			rtt = nullptr;
			rtv = nullptr;
			srv = nullptr;
			uav = nullptr;
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		~RenderTexture()
		{
			if (rtt)
				rtt->Release();
			if (rtv)
				rtv->Release();
			if (srv)
				srv->Release();
			if (uav)
				uav->Release();
		}
	};

	// Window
	HWND m_window;
	GameOptions* m_options;

	// Device
	Microsoft::WRL::ComPtr< ID3D11Device > m_device;
	Microsoft::WRL::ComPtr< ID3D11DeviceContext > m_deviceContext;
	Microsoft::WRL::ComPtr< ID3DUserDefinedAnnotation > m_annotation;

	// Render Target
	Microsoft::WRL::ComPtr< IDXGISwapChain > m_swapChain;
	Microsoft::WRL::ComPtr< ID3D11RenderTargetView > m_outputRTV;
	float m_clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	float m_clearColorBlack[4] = { 0.f, 0.f, 0.f, 1.f };
	float m_clearColorWhite[4] = { 1.f, 1.f, 1.f, 1.f };

	enum GBufferType { DIFFUSE_GB, NORMAL_SHADOWMASK_GB, SPECULAR_SHININESS_GB, AMBIENT_GLOBALAMBIENTCONTR_GB, DEPTH_GB, GB_NUM };
	
	RenderTexture m_gBuffer[GBufferType::GB_NUM];

	// Depth Buffer
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > m_depthStencilView;
	Microsoft::WRL::ComPtr< ID3D11Texture2D > m_depthStencilBuffer;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;

	D3D11_VIEWPORT m_viewport;

	// NUll Views
	ID3D11ShaderResourceView* m_nullSRV = nullptr;
	ID3D11ShaderResourceView* m_nullSRVs[GB_NUM] = { m_nullSRV, m_nullSRV, m_nullSRV, m_nullSRV, m_nullSRV };
	ID3D11RenderTargetView* m_nullRTV = nullptr;
	ID3D11RenderTargetView* m_nullRTVs[GB_NUM] = { m_nullRTV, m_nullRTV, m_nullRTV, m_nullRTV, m_nullRTV };

	// SamplerState
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	// Shaders
	Shaders m_GBufferShaders;
	Shaders m_lightingShaders;
	Shaders m_forwardLightingShaders;
	Shaders m_skyShaders;

	// Constant Buffer
	Microsoft::WRL::ComPtr< ID3D11Buffer > m_constantBuffer;

	// Texture Handler
	ResourceHandler* resourceHandler;

	// Shadow Mapping
	ShadowMapInstance m_shadowInstance;
	
	// SpriteBatch
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_spriteRasterizerState;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont16;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont32;

	// Primitive Batch
	std::unique_ptr< DirectX::CommonStates > m_states;
	std::unique_ptr< DirectX::BasicEffect > m_effect;
	std::unique_ptr< DirectX::PrimitiveBatch<DirectX::VertexPositionColor> > m_batch;
	Microsoft::WRL::ComPtr< ID3D11InputLayout > m_batchInputLayout;
	
	// Objects from state
	std::vector<GameObject*>* m_gameObjectsFromState;
	std::vector<GameObject*>* m_gameObjectsFromActiveRoom;
	std::vector<BoundingBox>* m_boundingBoxesFromActiveRoom;
	std::vector<BoundingBox>* m_triggerBoxes;
	std::vector<BoundingOrientedBox>* m_orientedBoundingBoxesFromActiveRoom;
	std::unordered_map<std::string, Model>* m_modelsFromState;
	DirectX::BoundingOrientedBox m_pyramidOBB;

	ConstBuffer<PS_PER_FRAME_BUFFER> m_perFrameBuffer;
	PS_PER_FRAME_BUFFER* m_constantBufferDataFromStatePtr;

	std::vector< ConstBuffer<VS_WVPW_CONSTANT_BUFFER> >* m_wvpCBufferFromState;
	ConstBuffer<VS_VP_MATRICES_CBUFFER> m_inverseMatricesBuffer;
	ConstBuffer<DirectX::XMMATRIX> m_skyboxCameraBuffer;
	ConstBuffer<DIR_LIGHT_DATA> m_skyboxSunLightBuffer;
	
	DirectX::XMMATRIX* m_viewMatrix;
	DirectX::XMMATRIX* m_projectionMatrix;

	// Sky
	Model m_skyCube;

	// Transition
	Transition* m_transition;

	// FPS Counter
	Timer m_timer;
	std::string m_fpsString;
	int m_fps;

	// Status Text
	StatusTextHandler* m_statusTextHandler;

	// Debug Drawing
	bool m_drawPhysicsPrimitives = false;
	bool m_drawLightsDebug = false;
	bool m_drawShadowmapDebug = false;
	bool m_drawGBufferDebug = false;

	// Initialization Functions
	void initDeviceAndSwapChain();
	void initRenderTarget(RenderTexture& rtv, UINT width, UINT height, DXGI_FORMAT format, UINT mipLevels = 1);
	void initViewPort();
	void initDepthStencilBuffer();
	void initSamplerState();
	void initShaders();
	void initConstantBuffer();
	void initSky();

public:
	ViewLayer();
	~ViewLayer();

	// Getters
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getContextDevice();

	// Setters
	void setViewMatrix(DirectX::XMMATRIX* ViewMatrix);
	void setProjectionMatrix(DirectX::XMMATRIX* newProjectionMatrix);

	// Setters for State Pointers
	void setgameObjectsFromState(std::vector<GameObject*>* gameObjectsFromState);
	void setgameObjectsFromActiveRoom(std::vector<GameObject*>* gameObjectsFromState);
	void setBoundingBoxesFromActiveRoom(std::vector<BoundingBox>* bbFromRoom);
	void setOrientedBoundingBoxesFromActiveRoom(std::vector<BoundingOrientedBox>* bbFromRoom);
	void setTriggerBoxFromActiveRoom(std::vector<BoundingBox>* bbFromRoom);
	void setModelsFromState(std::unordered_map<std::string, Model>* models);
	void setPerFrameDataFromState(PS_PER_FRAME_BUFFER* cbDataFromState);
	void setWvpCBufferFromState(std::vector< ConstBuffer<VS_WVPW_CONSTANT_BUFFER> >* models);

	// Initialization
	void initialize(HWND window, GameOptions* options);

	// Reload
	void reloadShaders();

	// Update
	void update(float dt, XMFLOAT3 cameraPos);

	// Render
	void render(iGameState* gameState);

	// Debug
	bool getDrawPhysicsPrimitives() const;
	bool getDrawLightsDebug() const;
	bool getDrawGBufferDebug() const;

	void setDrawPhysicsPrimitives(bool enabled);
	void setDrawLightsDebug(bool enabled);
	void setDrawGBufferDebug(bool enabled);
};