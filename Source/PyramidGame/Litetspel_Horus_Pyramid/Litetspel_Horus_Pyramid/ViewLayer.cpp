#include "pch.h"
#include "ViewLayer.h"

void ViewLayer::initDeviceAndSwapChain()
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
	swapChainDesc.OutputWindow = this->m_window;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.Width = this->m_wWidth;
	swapChainDesc.BufferDesc.Height = this->m_wHeight;
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
		&this->m_swapChain,
		&this->m_device,
		feature_level,
		&this->m_deviceContext
		);
	assert(SUCCEEDED(hr) && "Error, failed to create device and swapchain!");

	// Back Buffer Texture
	ID3D11Texture2D* backBuffer = nullptr;
	hr = this->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	assert(SUCCEEDED(hr) && "Error, failed to set backbuffer!");

	// Render Targets
	hr = this->m_device->CreateRenderTargetView(backBuffer, NULL, &this->m_outputRTV);
	assert(SUCCEEDED(hr) && "Error, failed to create ouput render target view!");

	backBuffer->Release();
}

void ViewLayer::initViewPort()
{
	RECT winRect;
	GetClientRect(this->m_window, &winRect);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = (FLOAT)winRect.left;
	viewport.TopLeftY = (FLOAT)winRect.top;
	viewport.Width = (FLOAT)this->m_wWidth;
	viewport.Height = (FLOAT)this->m_wHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	this->m_deviceContext->RSSetViewports(1, &viewport);
}

void ViewLayer::initDepthStencilBuffer()
{
	// Create Depth Stencil Buffer Texture
	D3D11_TEXTURE2D_DESC dsBufferDesc;
	dsBufferDesc.Width = this->m_wWidth;
	dsBufferDesc.Height = this->m_wHeight;
	dsBufferDesc.MipLevels = 1;
	dsBufferDesc.ArraySize = 1;
	dsBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	dsBufferDesc.SampleDesc.Count = 1;
	dsBufferDesc.SampleDesc.Quality = 0;
	dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	dsBufferDesc.CPUAccessFlags = 0;
	dsBufferDesc.MiscFlags = 0;

	HRESULT hr = this->m_device->CreateTexture2D(&dsBufferDesc, NULL, &this->m_depthStencilBuffer);
	assert(SUCCEEDED(hr) && "Error, failed to create depth buffer texture!");

	// Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	dsViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Flags = 0;
	dsViewDesc.Texture2D.MipSlice = 0;

	hr = this->m_device->CreateDepthStencilView(this->m_depthStencilBuffer.Get(), &dsViewDesc, &this->m_depthStencilView);
	assert(SUCCEEDED(hr) && "Error, failed to create depth stencil view!");

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

	hr = this->m_device->CreateDepthStencilState(&dsDesc, &this->m_depthStencilState);
	assert(SUCCEEDED(hr) && "Error, failed to create depth stencil state!");
}

void ViewLayer::initVertexBuffer()
{
	this->m_size = 3;
	this->m_stride = UINT(sizeof(Vertex));

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

	vertexBufferDesc.ByteWidth = this->m_stride * this->m_size;
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

	HRESULT hr = this->m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_vertexBuffer);
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");
}

void ViewLayer::initShaders()
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

	hr = this->m_device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&this->m_vertexShader
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

	hr = this->m_device->CreateInputLayout(
		layoutDesc,
		2,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&this->m_vertexLayout
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

	hr = this->m_device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		&this->m_pixelShader
		);
	assert(SUCCEEDED(hr) && "Error, failed to create pixel shader!");
	psBlob->Release();
}

ViewLayer::ViewLayer(int height, int width)
{
	this->m_wWidth = height;
	this->m_wHeight = width;

	this->m_stride = 0;
	this->m_size = 0;
}

ViewLayer::~ViewLayer() {}

UINT ViewLayer::getWindowWidth() const
{
	return this->m_wWidth;
}

UINT ViewLayer::getWindowHeight() const
{
	return this->m_wHeight;
}

void ViewLayer::initialize(HWND window)
{
	this->m_window = window;
	this->initDeviceAndSwapChain();
	this->initViewPort();
	this->initDepthStencilBuffer();
	this->initVertexBuffer();
	this->initShaders();
}

void ViewLayer::render()
{	
	// Clear Frame
	this->m_deviceContext->ClearRenderTargetView(this->m_outputRTV.Get(), clearColor);
	this->m_deviceContext->ClearDepthStencilView(this->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set Render Target
	this->m_deviceContext->OMSetRenderTargets(1, this->m_outputRTV.GetAddressOf(), this->m_depthStencilView.Get());

	// Set Vertex Buffers
	UINT vertexOffset = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), &this->m_stride, &vertexOffset);

	// Set Shaders
	this->m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_deviceContext->IASetInputLayout(this->m_vertexLayout.Get());

	this->m_deviceContext->VSSetShader(this->m_vertexShader.Get(), nullptr, 0);
	this->m_deviceContext->PSSetShader(this->m_pixelShader.Get(), nullptr, 0);

	// Draw
	this->m_deviceContext->Draw(this->m_size, 0);

	// Swap Frames
	this->m_swapChain->Present(0, 0);
}
