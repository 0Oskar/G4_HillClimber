#pragma once

#include "GameObject.h"
#include "Model.h"
#include "Timer.h"
#include "Platform.h"
#include "Shaders.h"

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
	float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };

	// Depth Buffer
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > m_depthStencilView;
	Microsoft::WRL::ComPtr< ID3D11Texture2D > m_depthStencilBuffer;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;
	
	// SamplerState
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	// Shaders
	Shaders m_shaders;

	// Constant Buffer
	Microsoft::WRL::ComPtr< ID3D11Buffer > m_constantBuffer;

	// Texture Handler
	ResourceHandler* resourceHandler;

	// Crosshair SpriteBatch
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	ID3D11ShaderResourceView* m_crossHairSRV;
	DirectX::XMFLOAT2 m_crosshairPosition;

	// Primitive Batch
	std::unique_ptr< DirectX::CommonStates > m_states;
	std::unique_ptr< DirectX::BasicEffect > m_effect;
	std::unique_ptr< DirectX::PrimitiveBatch<DirectX::VertexPositionColor> > m_batch;
	Microsoft::WRL::ComPtr< ID3D11InputLayout > m_batchInputLayout;
	bool m_drawPrimitives;

	// Objects from state
	std::vector<GameObject*>* m_gameObjectsFromState;
	std::vector<Model>* m_modelsFromState;
	std::vector< ConstBuffer<VS_CONSTANT_BUFFER> >* m_wvpCBufferFromState;
	DirectX::BoundingOrientedBox m_pyramidOBB;

	ConstBuffer<PS_LIGHT_BUFFER> m_lightBuffer;
	ConstBuffer<PS_DIR_BUFFER> m_dirLightBuffer;

	DirectX::XMMATRIX* m_viewMatrix;
	DirectX::XMMATRIX* m_projectionMatrix;

	// FPS Counter
	Timer m_timer;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::string m_fpsString;
	int m_fps;

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
	void setViewMatrix(DirectX::XMMATRIX* newViewMatrix);
	void setProjectionMatrix(DirectX::XMMATRIX* newProjectionMatrix);

	// Setters for State Pointers
	void setgameObjectsFromState(std::vector<GameObject*>* gameObjectsFromState);
	void setModelsFromState(std::vector<Model>* models);
	void setWvpCBufferFromState(std::vector< ConstBuffer<VS_CONSTANT_BUFFER> >* models);

	// Initialization
	void initialize(HWND window, GameOptions* options);

	// Update
	void update(float dt);

	// Render
	void render();
	void toggleDrawPrimitives(bool toggle);
};