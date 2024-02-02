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
#include "ViewHelper.h"
#include "SSAOInstance.h"

enum class ViewDebugCommands
{
	NONE_VDC,
	TOGGLE_DRAW_PHYSICS_PRIMITVES_VDC,
	TOGGLE_DRAW_MODEL_BB_PRIMITVES_VDC,
	TOGGLE_FRUSTOM_CULLING_VDC,
	TOGGLE_DRAW_LIGHTS_DEBUG_VDC,
	TOGGLE_SSAO_VDC,
	TOGGLE_SHADOWMAP_VDC,
	TOGGLE_SHADOWMAP_DEBUG_VDC,
	TOGGLE_GBUFFER_DEBUG_VDC,
	TOGGLE_DRAW_CALL_STATS_VDC
};

class ViewLayer
{
private:
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
	float m_clearColorNormal[4] = { 1.f, 0.5f, 0.5f, 1.f };

	enum GBufferType { DIFFUSE_GB, NORMAL_GB, SPECULAR_SHININESS_GB, AMBIENT_GLOBALAMBIENTCONTR_GB, SHADOW_GB, AMBIENT_OCCLUSION_GB, DEPTH_GB, GB_NUM };
	RenderTexture m_gBuffer[GBufferType::GB_NUM];

	// Depth Buffer
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > m_depthStencilView;
	Microsoft::WRL::ComPtr< ID3D11Texture2D > m_depthStencilBuffer;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;

	D3D11_VIEWPORT m_viewport;

	// NUll Views
	ID3D11ShaderResourceView* m_nullSRV = nullptr;
	ID3D11ShaderResourceView* m_nullSRVs[GB_NUM] = { m_nullSRV, m_nullSRV, m_nullSRV, m_nullSRV, m_nullSRV, m_nullSRV, m_nullSRV };
	ID3D11RenderTargetView* m_nullRTV = nullptr;
	ID3D11RenderTargetView* m_nullRTVs[GB_NUM] = { m_nullRTV, m_nullRTV, m_nullRTV, m_nullRTV, m_nullRTV, m_nullRTV, m_nullRTV };
	ID3D11UnorderedAccessView* m_nullUAV = nullptr;
	ID3D11UnorderedAccessView* m_nullUAVs[GB_NUM] = { m_nullUAV, m_nullUAV, m_nullUAV, m_nullUAV, m_nullUAV, m_nullUAV, m_nullUAV };

	// SamplerState
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	// Shaders
	Shaders m_GBufferShaders;
	Shaders m_lightingShaders;
	Shaders m_forwardLightingShaders;
	Shaders m_skyShaders;
	Shaders m_translucentShadowBlurShader;
	Shaders m_volumetricLightShader;
	Shaders m_blurShader;

	// Texture Handler
	ResourceHandler* resourceHandler;

	// Shadow Mapping
	ShadowMapInstance m_shadowInstance;
	ShadowMapInstance m_translucentShadowInstance;

	RenderTexture m_translucentShadowsBlurPingPongRT;
	RenderTexture m_translucentColorBlurPingPongRT;

	RenderTexture m_translucentShadowsBluredRT;
	RenderTexture m_translucentColorBluredRT;

	CS_BLUR_CBUFFER m_translucentShadowblurCData;
	
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

	// Constant Buffers
	ConstBuffer<SHADOW_PER_FRAME_BUFFER> m_shadowPerFrameBuffer;
	ConstBuffer<PS_PER_FRAME_BUFFER> m_perFrameBuffer;
	PS_PER_FRAME_BUFFER* m_constantBufferDataFromStatePtr;

	std::vector< ConstBuffer<VS_WVPW_CONSTANT_BUFFER> >* m_wvpCBufferFromState;
	ConstBuffer<VP_MATRICES_CBUFFER> m_inverseMatricesBuffer;
	ConstBuffer<DirectX::XMMATRIX> m_skyboxCameraBuffer;
	ConstBuffer<DIR_LIGHT_DATA> m_skyboxSunLightBuffer;
	ConstBuffer<VOLUMETRIC_LIGHT_DATA> m_volumetricLightBuffer;
	ConstBuffer<CAMERA_BUFFER> m_cameraBuffer;
	ConstBuffer<CASCADING_LIGHT_CAMERA_BUFFER> m_lightCameraBuffer;
	
	Camera* m_cameraPtr;

	// SSAO
	SSAOInstance m_SSAOInstance;

	// Blur
	Shaders m_edgePreservingBlurCS;
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > m_blurPingPongSRV;
	Microsoft::WRL::ComPtr< ID3D11UnorderedAccessView > m_blurPingPongUAV;
	CS_BLUR_CBUFFER m_blurCData;
	float m_ssaoBlurSigma = 5.f;
	ConstBuffer< CS_BLUR_CBUFFER > m_blurCBuffer;

	// Volumetric Lighting
	RenderTexture m_volumetricLightRT;
	RenderTexture m_volumetricLightBlurPingPongRT;
	RenderTexture m_volumetricLightBluredRT;

	CS_BLUR_CBUFFER m_volumetricLightblurCData;

	// Sky
	Model m_skyCube;

	// Transition
	Transition* m_transition;

	// FPS Counter
	Timer m_timer;
	std::string m_fpsString;
	int m_fps;

	// Draw Call Counter
	uint32_t m_shadowmapDrawCallCount;
	uint32_t m_drawCallCount;
	uint32_t m_postProcessDrawCallCount;

	// Status Text
	StatusTextHandler* m_statusTextHandler;

	// Initialization Functions
	void initDeviceAndSwapChain();
	void initRenderTarget(RenderTexture& rtv, UINT bindFlags, std::string name, UINT width, UINT height, DXGI_FORMAT format, UINT mipLevels = 1);
	void initViewPort();
	void initDepthStencilBuffer();
	void initSamplerState();
	void initShaders();
	void initConstantBuffer();
	void initSky();

	void initSSAOBlurPass();
	void blurSSAOPass();

	void initBlurTranslucentShadowsPass(uint32_t translucentTextureSize);
	void blurTranslucentShadowsPass();

	void initVolumetricLightPass();
	void volumetricLightPass();
	void blurVolumetricLightPass();

public:
	ViewLayer();
	~ViewLayer();

	// Getters
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getContextDevice();

	// Setters
	void setCamera(Camera* camera);
	int* usingShadowmapDebug();

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
	void update(float dt);

	// Render
	void render(iGameState* gameState);

	// Debug Drawing
	bool m_drawPhysicsPrimitives = false;
	bool m_drawModelBoxPrimitives = false;
	bool m_drawLightsDebug = false;
	bool m_drawShadowmapDebug = false;
	bool m_drawGBufferDebug = false;
	bool m_drawDrawCallStatsDebug = true;

	// Feature Toggles
	bool m_ssaoToggle = true;
	bool m_shadowMappingToggle = true;
	bool m_volumetricLightToggle = true;
	bool m_frustumCullingToggle = false;
	bool m_ignorePositionShadowToggle = false;
	float m_ignorePositionShadowTimer = 0;
	const float IGNORE_POSITION_SHADOW_TIME = 0.3f;
};