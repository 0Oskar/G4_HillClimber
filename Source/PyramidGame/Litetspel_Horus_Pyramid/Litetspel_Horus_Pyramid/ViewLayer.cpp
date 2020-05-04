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
	swapChainDesc.BufferDesc.Width = this->m_options->width;
	swapChainDesc.BufferDesc.Height = this->m_options->height;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	D3D_FEATURE_LEVEL feature_level[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
//#if defined( DEBUG ) || defined( _DEBUG )
//	flags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif

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
	viewport.Width = (FLOAT)this->m_options->width;
	viewport.Height = (FLOAT)this->m_options->height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	this->m_deviceContext->RSSetViewports(1, &viewport);
}

void ViewLayer::initDepthStencilBuffer()
{
	// Create Depth Stencil Buffer Texture
	D3D11_TEXTURE2D_DESC dsBufferDesc;
	dsBufferDesc.Width = this->m_options->width;
	dsBufferDesc.Height = this->m_options->height;
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
		/*{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}*/
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			24,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	hr = this->m_device->CreateInputLayout(
		layoutDesc,
		3,
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

ViewLayer::ViewLayer()
{
	this->m_gameObjectsFromState = nullptr;
	this->m_modelsFromState = nullptr;
	this->m_wvpCBufferFromState = nullptr;

	this->resourceHandler = &ResourceHandler::get();
	this->m_crossHairSRV = nullptr;
	this->m_crosshairPosition = DirectX::XMFLOAT2();

	this->m_drawPrimitives = false;

	this->m_viewMatrix = nullptr;
	this->m_projectionMatrix = nullptr;
	this->m_fps = 0;
}

ViewLayer::~ViewLayer()
{
	
}

ID3D11Device* ViewLayer::getDevice()
{
	return this->m_device.Get();
}

ID3D11DeviceContext* ViewLayer::getContextDevice()
{
	return this->m_deviceContext.Get();
}

void ViewLayer::setViewMatrix(DirectX::XMMATRIX* newViewMatrix)
{
	this->m_viewMatrix = newViewMatrix;
}

void ViewLayer::setProjectionMatrix(DirectX::XMMATRIX* newProjectionMatrix)
{
	this->m_projectionMatrix = newProjectionMatrix;
}

void ViewLayer::setgameObjectsFromState(std::vector<GameObject*>* gameObjectsFromState)
{
	this->m_gameObjectsFromState = gameObjectsFromState;
}

void ViewLayer::setModelsFromState(std::vector<Model>* models)
{
	this->m_modelsFromState = models;
}

void ViewLayer::setWvpCBufferFromState(std::vector< ConstBuffer<VS_CONSTANT_BUFFER> >* buffers)
{
	this->m_wvpCBufferFromState = buffers;
}

void ViewLayer::initConstantBuffer()
{
	this->m_lightBuffer.init(this->m_device.Get(), this->m_deviceContext.Get());
	this->m_dirLightBuffer.init(this->m_device.Get(), this->m_deviceContext.Get());
}

void ViewLayer::initSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;

	HRESULT hr = this->m_device->CreateSamplerState(&samplerDesc, this->m_samplerState.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error when creating sampler state!");

}

void ViewLayer::initialize(HWND window, GameOptions* options)
{
	this->m_window = window;
	this->m_options = options;

	this->m_timer.start();

	this->initDeviceAndSwapChain();
	this->initViewPort();
	this->initSamplerState();
	this->initDepthStencilBuffer();
	this->initShaders();
	this->initConstantBuffer();

	// Ambient Light buffer
	PS_LIGHT_BUFFER lightBuffer;
	lightBuffer.lightColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	lightBuffer.strength = 0.2f;
	this->m_lightBuffer.m_data = lightBuffer;
	this->m_lightBuffer.upd();

	// Directional Light buffer
	PS_DIR_BUFFER dirBuffer;
	dirBuffer.lightColor = DirectX::XMFLOAT4(.8f, 0.8f, 0.8f, 1.f);
	dirBuffer.lightDirection = DirectX::XMFLOAT4(-1.0f, 1.0f, -0.7f, 0.0f);
	this->m_dirLightBuffer.m_data = dirBuffer;
	this->m_dirLightBuffer.upd();

	// Pyramid Frustum for drawing only(Seperate from)
	DirectX::XMFLOAT3 center(0.f, 52.f, 80.f);
	DirectX::XMFLOAT3 extents(80.f, 105.f, 1.f);
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(0.9f, 0.f, 0.f);
	DirectX::XMFLOAT4 orientation;
	DirectX::XMStoreFloat4(&orientation, quaternion);

	this->m_pyramidOBB = DirectX::BoundingOrientedBox(
		center,
		extents,
		orientation
	);

	this->resourceHandler->m_dContext = this->getContextDevice();
	this->resourceHandler->m_device = this->getDevice();

	// Crosshair
	this->m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->m_deviceContext.Get());
	this->m_crossHairSRV = this->resourceHandler->getTexture(L"Textures/crosshair.png");
	ID3D11Resource* crosshairTexture = 0;
	this->m_crossHairSRV->GetResource(&crosshairTexture);
	ID3D11Texture2D* crosshairTexture2D = 0;
	crosshairTexture->QueryInterface<ID3D11Texture2D>(&crosshairTexture2D);
	D3D11_TEXTURE2D_DESC desc;
	crosshairTexture2D->GetDesc(&desc);
	int crosshairX = (this->m_options->width / 2) - (desc.Width / 2);
	int crosshairY = (this->m_options->height / 2) - (desc.Height / 2);
	this->m_crosshairPosition = DirectX::XMFLOAT2((float)crosshairX, (float)crosshairY);

	// Primitive Batch
	this->m_states = std::make_unique<DirectX::CommonStates >(this->m_device.Get());
	this->m_batch = std::make_unique< DirectX::PrimitiveBatch<DirectX::VertexPositionColor> >(this->m_deviceContext.Get());

	this->m_effect = std::make_unique< DirectX::BasicEffect >(this->m_device.Get());
	this->m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	this->m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	HRESULT hr = this->m_device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements, 
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_batchInputLayout.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr) && "Error, failed to create input layout for primitives!");

	// FPS counter
	this->m_spriteFont = std::make_unique<DirectX::SpriteFont>(this->m_device.Get(), L"Fonts\\product_sans_16.spritefont");
	this->m_fpsString = "FPS: NULL";

}

void ViewLayer::update(float dt)
{

}

void ViewLayer::render()
{
	// Clear Frame
	this->m_deviceContext->ClearRenderTargetView(this->m_outputRTV.Get(), clearColor);
	this->m_deviceContext->ClearDepthStencilView(this->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set Render Target
	this->m_deviceContext->OMSetRenderTargets(1, this->m_outputRTV.GetAddressOf(), this->m_depthStencilView.Get());
	//this->m_deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	this->m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	this->m_deviceContext->RSSetState(this->m_states->CullCounterClockwise());

	// Set Shaders
	this->m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_deviceContext->IASetInputLayout(this->m_vertexLayout.Get());

	this->m_deviceContext->PSSetSamplers(0, 1, this->m_samplerState.GetAddressOf());

	this->m_deviceContext->VSSetShader(this->m_vertexShader.Get(), nullptr, 0);
	this->m_deviceContext->PSSetShader(this->m_pixelShader.Get(), nullptr, 0);

	// Set Constant Buffer
	this->m_deviceContext->PSSetConstantBuffers(1, 1, this->m_lightBuffer.GetAdressOf());
	this->m_deviceContext->PSSetConstantBuffers(2, 1, this->m_dirLightBuffer.GetAdressOf());

	// Draw
	DirectX::XMMATRIX viewPMtrx = (*m_viewMatrix) * (*m_projectionMatrix);
	for (size_t i = 0; i < this->m_gameObjectsFromState->size(); i++)
	{
		// Get indexes
		GameObject* gObject = this->m_gameObjectsFromState->at(i);
    if (gObject->visible())
		{
      int wvpIndex = gObject->getWvpCBufferIndex();
      int mIndex = gObject->getModelIndex();
      // Set Constant buffer
      this->m_deviceContext->VSSetConstantBuffers(0, 1, this->m_wvpCBufferFromState->at(wvpIndex).GetAdressOf());
      // Draw Model
      this->m_modelsFromState->at(mIndex).m_material.setTexture(gObject->getTexturePath().c_str());
      this->m_modelsFromState->at(mIndex).draw(viewPMtrx);
		}
	}

	// Draw Primitives
	if (this->m_drawPrimitives)
	{
		this->m_deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
		this->m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
		this->m_deviceContext->RSSetState(m_states->CullNone());

		this->m_deviceContext->IASetInputLayout(this->m_batchInputLayout.Get());

		this->m_batch->Begin();

		this->m_effect->SetView(*(this->m_viewMatrix));
		this->m_effect->SetProjection(*(this->m_projectionMatrix));
		// Apply Effect
		this->m_effect->Apply(this->m_deviceContext.Get());
		for (size_t i = 0; i < this->m_gameObjectsFromState->size(); i++)
		{
			if (this->m_gameObjectsFromState->at(i)->collidable())
			{
				// Draw Primitive
				DX::Draw(m_batch.get(), *(this->m_gameObjectsFromState->at(i)->getAABBPtr()), DirectX::Colors::Blue);
			}
		}
		DX::Draw(m_batch.get(), this->m_pyramidOBB, DirectX::Colors::Blue);

		m_batch->End();
	}

	//FPS Counter
	this->m_fps++;
	if (1.0 < m_timer.timeElapsed())
	{
		this->m_fpsString = "FPS: " + std::to_string(this->m_fps);
		m_timer.restart();
		this->m_fps = 0;
	}

	// Draw Sprites
	this->m_spriteBatch->Begin();
	this->m_spriteBatch->Draw(this->m_crossHairSRV, this->m_crosshairPosition);
	this->m_spriteFont->DrawString(this->m_spriteBatch.get(), this->m_fpsString.c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	this->m_spriteBatch->End();
	
	// Swap Frames
	this->m_swapChain->Present(0, 0);
}

void ViewLayer::toggleDrawPrimitives(bool toggle)
{
	//this->m_drawPrimitives = !this->m_drawPrimitives;
	this->m_drawPrimitives = toggle;
}