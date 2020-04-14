#pragma once

#include "VertexBuffer.h"
#include "Camera.h"
#include "Player.h"
#include "ConstantBuffer.h"

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

	// Shaders
	Microsoft::WRL::ComPtr< ID3D11VertexShader > m_vertexShader;
	Microsoft::WRL::ComPtr< ID3D11PixelShader > m_pixelShader;
	Microsoft::WRL::ComPtr< ID3D11InputLayout > m_vertexLayout;

	// Constant Buffer
	Microsoft::WRL::ComPtr< ID3D11Buffer > m_constantBuffer;

	ConstBuffer<VS_CONSTANT_BUFFER> m_triangleCBuffer;

	// Vertex Buffer
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
		Vertex(float x, float y, float z, float r, float g, float b)
		{
			position = DirectX::XMFLOAT3(x, y, z);
			color = DirectX::XMFLOAT3(r, g, b);
		}
	};
	VertexBuffer<Vertex> m_vertexBuffer;

	// Player
	Player m_player;

	// Camera
	Camera m_camera;

	// Initialization Functions
	void initDeviceAndSwapChain();
	void initViewPort();
	void initDepthStencilBuffer();
	void initVertexBuffer();
	void initShaders();
	void initConstantBuffer();

public:
	ViewLayer();
	~ViewLayer();

	// Initialization
	void initialize(HWND window, GameOptions* options);

	// Render
	void render();
};