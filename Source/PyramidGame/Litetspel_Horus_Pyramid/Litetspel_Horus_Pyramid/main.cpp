#ifndef UNICODE
#define UNICODE
#endif

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

// Window
HWND window;
int windowWidth = 1280;
int windowHeight = 720;
HRESULT hr;

// DirectX
Microsoft::WRL::ComPtr< ID3D11Device > _device;
Microsoft::WRL::ComPtr< ID3D11DeviceContext > _deviceContext;

// Render Target
Microsoft::WRL::ComPtr < IDXGISwapChain > _swapChain;
Microsoft::WRL::ComPtr < ID3D11RenderTargetView > _outputRTV;
float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };

// Depth Buffer
Microsoft::WRL::ComPtr < ID3D11DepthStencilView > _depthStencilView;
Microsoft::WRL::ComPtr < ID3D11Texture2D > _depthStencilBuffer;
Microsoft::WRL::ComPtr < ID3D11DepthStencilState > _depthStencilState;

// Shaders
Microsoft::WRL::ComPtr < ID3D11VertexShader > _vertexShader;
Microsoft::WRL::ComPtr < ID3D11PixelShader > _pixelShader;
Microsoft::WRL::ComPtr < ID3D11InputLayout >_vertexLayout;

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
Microsoft::WRL::ComPtr < ID3D11Buffer > _vertexBuffer;
UINT _stride;
UINT _size;

// Window Functions
void createWin32Window(HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// DirectX Functions
void initialize();
void initDeviceAndSwapChain();
void initViewPort();
void initDepthStencilBuffer();
void initVertexBuffer();
void initShaders();
void render();

// Main
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	// Window Setup
	const wchar_t WINDOWTILE[] = L"Litetspel Horus Pyramid";
	createWin32Window(hInstance, WINDOWTILE, window);

	// Initialize Engine
	initialize();

	// Show Window
	ShowWindow(window, nShowCmd);
	OutputDebugStringA("Window Created!\n");

	MSG msg = { };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // Message loop
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // Render/Logic Loop
		{
			render();
		}
	}

	return 0;
}

void createWin32Window(HINSTANCE hInstance, const wchar_t* windowTitle, HWND& _d3d11Window)
{
	const wchar_t CLASS_NAME[] = L"Litetspel_Horus_Pyramid";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.
	_d3d11Window = CreateWindowEx(
		0,                          // Optional window styles.
		CLASS_NAME,                 // Window class
		windowTitle,				// Window text
		WS_OVERLAPPEDWINDOW,        // Window style
		CW_USEDEFAULT,				// Position, X
		CW_USEDEFAULT,				// Position, Y
		windowWidth,				// Width
		windowHeight,				// Height
		NULL,						// Parent window    
		NULL,						// Menu
		hInstance,					// Instance handle
		NULL						// Additional application data
		);
	assert(_d3d11Window);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void initialize()
{
	initDeviceAndSwapChain();
	initViewPort();
	initDepthStencilBuffer();
	initVertexBuffer();
	initShaders();
}

void initDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Create Device and Swap Chain
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	D3D_FEATURE_LEVEL feature_level[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&_swapChain,
		&_device,
		feature_level,
		&_deviceContext
		);
	assert(SUCCEEDED(hr));

	// Back Buffer / Render Targets
	ID3D11Texture2D* backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	assert(SUCCEEDED(hr));

	hr = _device->CreateRenderTargetView(backBuffer, NULL, &_outputRTV);
	assert(SUCCEEDED(hr));

	backBuffer->Release();
}

void initViewPort()
{
	RECT winRect;
	GetClientRect(window, &winRect);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = (FLOAT)winRect.left;
	viewport.TopLeftY = (FLOAT)winRect.top;
	viewport.Width = (FLOAT)windowWidth;
	viewport.Height = (FLOAT)windowHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	_deviceContext->RSSetViewports(1, &viewport);
}

void initDepthStencilBuffer()
{
	// Create Depth Stencil Buffer Texture
	D3D11_TEXTURE2D_DESC dsBufferDesc;
	dsBufferDesc.Width = windowWidth;
	dsBufferDesc.Height = windowHeight;
	dsBufferDesc.MipLevels = 1;
	dsBufferDesc.ArraySize = 1;
	dsBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	dsBufferDesc.SampleDesc.Count = 1;
	dsBufferDesc.SampleDesc.Quality = 0;
	dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	dsBufferDesc.CPUAccessFlags = 0;
	dsBufferDesc.MiscFlags = 0;

	HRESULT hr = _device->CreateTexture2D(&dsBufferDesc, NULL, &_depthStencilBuffer);
	assert(SUCCEEDED(hr));

	// Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	dsViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Flags = 0;
	dsViewDesc.Texture2D.MipSlice = 0;

	hr = _device->CreateDepthStencilView(_depthStencilBuffer.Get(), &dsViewDesc, &_depthStencilView);
	assert(SUCCEEDED(hr));

	// Create Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = _device->CreateDepthStencilState(&dsDesc, &_depthStencilState);
	assert(SUCCEEDED(hr));
}

void initVertexBuffer()
{
	_size = 3;
	_stride = UINT(sizeof(Vertex));

	Vertex vertices[]
	{
		Vertex(
			-0.5f, -0.5f, 0.f,
			1.f, 0.f, 0.f
		),
		Vertex(
			0.f, 0.5f, 0.f,
			0.f, 1.f, 0.f
		),
		Vertex(
			0.5f, -0.5f, 0.f,
			0.f, 0.f, 1.f
		)
	};

	// Buffer Description
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.ByteWidth = _stride * _size;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");
}

void initShaders()
{
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
	#endif

	// Create Vertex Shader
	
	ID3DBlob* vsBlob = nullptr;
	hr = D3DCompileFromFile(
		L"Shader Files\\VertexShader.hlsl",	// filename
		nullptr,							// optional macros
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// optional include files
		"main",								// entry point
		"vs_5_0",							// shader model (target)
		flags,								// shader compile options (DEBUGGING)
		0,									// IGNORE...DEPRECATED.
		&vsBlob,							// double pointer to ID3DBlob		
		&errorBlob							// pointer for Error Blob messages.
	);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (vsBlob)
			vsBlob->Release();
		assert(false);
	}

	hr = _device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&_vertexShader
	);
	assert(SUCCEEDED(hr) && "Error, failed to create vertex shader!");

	// Vertex Layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	hr = _device->CreateInputLayout(
		layoutDesc,
		2,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&_vertexLayout
	);
	assert(SUCCEEDED(hr) && "Error, failed to create vertex layout!");
	vsBlob->Release();
	
	// Create Pixel Shader
	ID3DBlob* psBlob = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	hr = D3DCompileFromFile(
		L"Shader Files\\PixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		flags,
		0,
		&psBlob,
		&errorBlob
	);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (psBlob)
			psBlob->Release();
		assert(false);
	}

	hr = _device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		&_pixelShader
	);
	assert(SUCCEEDED(hr) && "Error, failed to create pixel shader!");
	psBlob->Release();
}

void render()
{
	// Clear Frame
	_deviceContext->ClearRenderTargetView(_outputRTV.Get(), clearColor);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set Render Target
	_deviceContext->OMSetRenderTargets(1, _outputRTV.GetAddressOf(), _depthStencilView.Get());

	// Set Vertex Buffers
	UINT vertexOffset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &_stride, &vertexOffset);
	
	// Set Shaders
	_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->IASetInputLayout(_vertexLayout.Get());

	_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	// Draw
	_deviceContext->Draw(_size, 0);

	// Swap Frames
	_swapChain->Present(0, 0);
}