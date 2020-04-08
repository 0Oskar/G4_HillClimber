#pragma once

// Standard
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <assert.h>
#include <memory>
#include <wrl/client.h>

// DirectX 11
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// DirectX 11 Linking
#pragma comment (lib, "user32")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "gdi32")
#pragma comment (lib, "d3dcompiler.lib")

class ViewLayer
{
private:
	// Window
	HWND m_window;
	int m_wWidth;
	int m_wHeight;

	// Device
	Microsoft::WRL::ComPtr< ID3D11Device > m_device;
	Microsoft::WRL::ComPtr< ID3D11DeviceContext > m_deviceContext;

	// Render Target
	Microsoft::WRL::ComPtr < IDXGISwapChain > m_swapChain;
	Microsoft::WRL::ComPtr < ID3D11RenderTargetView > m_outputRTV;
	float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };

	// Depth Buffer
	Microsoft::WRL::ComPtr < ID3D11DepthStencilView > m_depthStencilView;
	Microsoft::WRL::ComPtr < ID3D11Texture2D > m_depthStencilBuffer;
	Microsoft::WRL::ComPtr < ID3D11DepthStencilState > m_depthStencilState;

	// Shaders
	Microsoft::WRL::ComPtr < ID3D11VertexShader > m_vertexShader;
	Microsoft::WRL::ComPtr < ID3D11PixelShader > m_pixelShader;
	Microsoft::WRL::ComPtr < ID3D11InputLayout >m_vertexLayout;

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
	Microsoft::WRL::ComPtr < ID3D11Buffer > m_vertexBuffer;
	UINT m_stride;
	UINT m_size;

	// Initialization Functions
	void initDeviceAndSwapChain();
	void initViewPort();
	void initDepthStencilBuffer();
	void initVertexBuffer();
	void initShaders();

public:
	ViewLayer(int width, int height);
	~ViewLayer();

	// Getters
	UINT getWindowWidth() const;
	UINT getWindowHeight() const;

	// Initialization
	void initialize(HWND window);

	// Render
	void render();

};