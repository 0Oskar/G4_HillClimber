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
	swapChainDesc.OutputWindow = m_window;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.Width = m_options->width;
	swapChainDesc.BufferDesc.Height = m_options->height;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	D3D_FEATURE_LEVEL feature_level[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	UINT flags = 0;
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
		&m_swapChain,
		&m_device,
		feature_level,
		&m_deviceContext
	);
	assert(SUCCEEDED(hr) && "Error, failed to create device and swapchain!");

	m_deviceContext->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)(&m_annotation));

	// Back Buffer Texture
	ID3D11Texture2D* backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	assert(SUCCEEDED(hr) && "Error, failed to set backbuffer!");

	// Render Targets
	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_outputRTV);
	assert(SUCCEEDED(hr) && "Error, failed to create ouput render target view!");

	backBuffer->Release();
}

void ViewLayer::initViewPort()
{
	RECT winRect;
	GetClientRect(m_window, &winRect);

	m_viewport.TopLeftX = (FLOAT)winRect.left;
	m_viewport.TopLeftY = (FLOAT)winRect.top;
	m_viewport.Width = (FLOAT)m_options->width;
	m_viewport.Height = (FLOAT)m_options->height;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;
}

void ViewLayer::initDepthStencilBuffer()
{
	// Create Depth Stencil Buffer Texture
	D3D11_TEXTURE2D_DESC dsBufferDesc;
	dsBufferDesc.Width = m_options->width;
	dsBufferDesc.Height = m_options->height;
	dsBufferDesc.MipLevels = 1;
	dsBufferDesc.ArraySize = 1;
	dsBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	dsBufferDesc.SampleDesc.Count = 1;
	dsBufferDesc.SampleDesc.Quality = 0;
	dsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	dsBufferDesc.CPUAccessFlags = 0;
	dsBufferDesc.MiscFlags = 0;

	HRESULT hr = m_device->CreateTexture2D(&dsBufferDesc, NULL, &m_depthStencilBuffer);
	assert(SUCCEEDED(hr) && "Error, failed to create depth buffer texture!");

	// Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	dsViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Flags = 0;
	dsViewDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &dsViewDesc, &m_depthStencilView);
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

	hr = m_device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);
	assert(SUCCEEDED(hr) && "Error, failed to create depth stencil state!");
}

void ViewLayer::initShaders()
{
	ShaderFiles shaders;
	shaders.vs = L"Shader Files\\VertexShader.hlsl";
	shaders.ps = L"Shader Files\\PixelShader.hlsl";
	m_shaders.initialize(m_device.Get(), m_deviceContext.Get(), shaders);

	CD3D11_RASTERIZER_DESC rastDesc(
		D3D11_FILL_SOLID, 
		D3D11_CULL_NONE, 
		FALSE,
		D3D11_DEFAULT_DEPTH_BIAS, 
		D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, 
		TRUE, 
		FALSE, 
		TRUE, 
		TRUE
	);

	HRESULT hr = m_device->CreateRasterizerState(&rastDesc, m_spriteRasterizerState.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create sprite rasterizer state!");
}

ViewLayer::ViewLayer()
{
	m_gameObjectsFromState = nullptr;
	m_modelsFromState = nullptr;
	m_wvpCBufferFromState = nullptr;

	resourceHandler = &ResourceHandler::get();
	

	m_drawPrimitives = false;

	m_viewMatrix = nullptr;
	m_projectionMatrix = nullptr;
	m_fps = 0;
}

ViewLayer::~ViewLayer()
{
	
}

ID3D11Device* ViewLayer::getDevice()
{
	return m_device.Get();
}

ID3D11DeviceContext* ViewLayer::getContextDevice()
{
	return m_deviceContext.Get();
}



void ViewLayer::setViewMatrix(DirectX::XMMATRIX* newViewMatrix)
{
	m_viewMatrix = newViewMatrix;
}

void ViewLayer::setProjectionMatrix(DirectX::XMMATRIX* newProjectionMatrix)
{
	m_projectionMatrix = newProjectionMatrix;
}

void ViewLayer::setgameObjectsFromState(std::vector<GameObject*>* gameObjectsFromState)
{
	m_gameObjectsFromState = gameObjectsFromState;
}

void ViewLayer::setgameObjectsFromActiveRoom(std::vector<GameObject*>* gameObjectsFromState)
{
	m_gameObjectsFromActiveRoom = gameObjectsFromState;
}

void ViewLayer::setBoundingBoxesFromActiveRoom(std::vector<BoundingBox>* bbFromRoom)
{
	m_boundingBoxesFromActiveRoom = bbFromRoom;
}

void ViewLayer::setOrientedBoundingBoxesFromActiveRoom(std::vector<BoundingOrientedBox>* bbFromRoom)
{
	m_orientedBoundingBoxesFromActiveRoom = bbFromRoom;
}

void ViewLayer::setTriggerBoxFromActiveRoom(std::vector<BoundingBox>* bbFromRoom)
{
	m_triggerBoxes = bbFromRoom;
}

void ViewLayer::setModelsFromState(std::unordered_map<std::string, Model>* models)
{
	m_modelsFromState = models;
}

void ViewLayer::setDirLightFromActiveRoom(PS_DIR_BUFFER dirLight)
{
	m_dirLight = dirLight;
	m_dirLightBuffer.m_data = m_dirLight;
	m_dirLightBuffer.upd();
}

void ViewLayer::setFogDataFromActiveRoom(PS_FOG_BUFFER fogData)
{
	m_fogBuffer.m_data = fogData;
}

void ViewLayer::setConstantBuffersFromGameState(constantBufferData* cbDataFromState)
{
	m_constantBufferDataFromStatePtr = cbDataFromState;
	m_lightBuffer.m_data = cbDataFromState->lightBuffer;
	m_dirLightBuffer.m_data = cbDataFromState->dirBuffer;
	m_dirLight = cbDataFromState->dirBuffer;
	m_fogBuffer.m_data = cbDataFromState->fogBuffer;
	m_dirLightBuffer.upd();
	m_fogBuffer.upd();
	m_lightBuffer.upd();
}

void ViewLayer::setLightDataFromActiveRoom(PS_LIGHT_BUFFER lightData)
{
	m_lightBuffer.m_data = lightData;
	m_lightBuffer.upd();
}

void ViewLayer::setWvpCBufferFromState(std::vector< ConstBuffer<VS_CONSTANT_BUFFER> >* buffers)
{
	m_wvpCBufferFromState = buffers;
}


void ViewLayer::initConstantBuffer()
{
	m_lightBuffer.init(m_device.Get(), m_deviceContext.Get());
	m_dirLightBuffer.init(m_device.Get(), m_deviceContext.Get());
	m_fogBuffer.init(m_device.Get(), m_deviceContext.Get());
	m_cameraBuffer.init(m_device.Get(), m_deviceContext.Get());
}

void ViewLayer::initSky()
{
	// Shader
	ShaderFiles shaders;
	shaders.vs = L"Shader Files\\SkyVS.hlsl";
	shaders.ps = L"Shader Files\\SkyPS.hlsl";
	m_skyShaders.initialize(m_device.Get(), m_deviceContext.Get(), shaders);

	// Cube Mesh
	MaterialData mat;
	mat.ambient = { 0.0f, 0.0f, 0.0f};
	mat.globalAmbientContribution = 1.f;
	mat.diffuse = { 0.9f, 0.9f, 0.9f, 1.0f };
	mat.specular = {1.0f, 1.0f, 1.0f, 1.0f };
	mat.shininess = 32;
	m_skyCube.loadModel(m_device.Get(), m_deviceContext.Get(), "cube2.obj", mat, L"");
}

void ViewLayer::initSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;

	HRESULT hr = m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error when creating sampler state!");
}

void ViewLayer::initialize(HWND window, GameOptions* options)
{
	m_window = window;
	m_options = options;

	m_timer.start();

	initDeviceAndSwapChain();
	initViewPort();
	initSamplerState();
	initDepthStencilBuffer();
	initShaders();
	initConstantBuffer();
	initSky();

	// Lights
	PointLight pLight;
	pLight.plPosition = { 0, 10, 0 };
	pLight.plDiffuse = { 0, 0, 0, 1 };
	pLight.plAmbient = { 0, 0, 0, 1 };
	pLight.plRange = 100;
	pLight.att = { 0, 1, 0 };
	// - Ambient Light buffer
	PS_LIGHT_BUFFER lightBuffer;
	lightBuffer.lightColor = DirectX::XMFLOAT3(1.f, 1.0f, 1.0f); //Set default behaviour in room.cpp and change for induvidual rooms in theire class by accesing m_lightData
	lightBuffer.strength = 0.1f;
	lightBuffer.nrOfPointLights = 1;
	lightBuffer.pointLights[0] = pLight;
	m_lightBuffer.m_data = lightBuffer;
	m_lightBuffer.upd();
	// - Directional Light buffer
	PS_DIR_BUFFER dirBuffer;
	dirBuffer.lightColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	dirBuffer.lightDirection = DirectX::XMFLOAT4(-0.8f, 1.0f, -0.7f, 0.0f);
	m_dirLightBuffer.m_data = dirBuffer;
	m_dirLightBuffer.upd();

	// Fog
	PS_FOG_BUFFER fogBuffer; //Just set for init, change in room with m_fogData member variable.
	fogBuffer.fogEnd = 100.0f;
	fogBuffer.fogStart = 50.0f;
	fogBuffer.fogColor = XMFLOAT3(0.5f, 0.5f, 0.5f);

	m_fogBuffer.m_data = fogBuffer;
	m_fogBuffer.upd();

	// Pyramid Oriented Bounding Box for drawing only(Seperate from box in gamestate)
	DirectX::XMFLOAT3 center(0.f, 62.f, 80.f);
	DirectX::XMFLOAT3 extents(450.f, 550.f, 1.f);
	float rotationX = XMConvertToRadians(46.f);
	LPCWSTR test = std::to_wstring(rotationX).c_str();
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotationX, 0.f, 0.f);
	DirectX::XMFLOAT4 orientation;
	DirectX::XMStoreFloat4(&orientation, quaternion);
	m_pyramidOBB = DirectX::BoundingOrientedBox(
		center,
		extents,
		orientation
	);
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());

	resourceHandler->m_dContext = getContextDevice();
	resourceHandler->m_device = getDevice();

	// Crosshair
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());

	// Map
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());

	// Map Player
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());

	// Primitive Batch
	m_states = std::make_unique< DirectX::CommonStates >(m_device.Get());
	m_batch = std::make_unique< DirectX::PrimitiveBatch<DirectX::VertexPositionColor> >(m_deviceContext.Get());

	m_effect = std::make_unique< DirectX::BasicEffect >(m_device.Get());
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	HRESULT hr = m_device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements, 
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_batchInputLayout.ReleaseAndGetAddressOf());

	assert(SUCCEEDED(hr) && "Error, failed to create input layout for primitives!");

	// Transition
	m_transition = &Transition::get();
	m_transition->initialize(m_device.Get(), m_deviceContext.Get());

	// FPS counter
	m_spriteFont16 = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"Fonts\\jost_16_bold.spritefont");
	m_spriteFont32 = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"Fonts\\jost_32_bold.spritefont");
	m_fpsString = "FPS: NULL";

	// Status Text Handler
	m_statusTextHandler = &StatusTextHandler::get();
	m_statusTextHandler->setWindowDimensions(m_options->width, m_options->height);

	// Shadowe Mapping
	m_shadowInstance.initialize(m_device.Get(), m_deviceContext.Get(), 4096, 4096);
}

void ViewLayer::reloadShaders()
{
	m_shaders.reloadShaders();
	m_skyShaders.reloadShaders();
}

void ViewLayer::update(float dt, XMFLOAT3 cameraPos)
{
	// Transition
	m_transition->update(dt);

	//FPS Counter
	m_fps++;
	if (1.0 < m_timer.timeElapsed())
	{
		m_fpsString = "FPS: " + std::to_string(m_fps);
		m_timer.restart();
		m_fps = 0;
	}

	clearColor[0] = m_fogBuffer.m_data.fogColor.x;
	clearColor[1] = m_fogBuffer.m_data.fogColor.y;
	clearColor[2] = m_fogBuffer.m_data.fogColor.z;

	m_shadowInstance.buildLightMatrix(m_constantBufferDataFromStatePtr->dirBuffer, cameraPos);

	m_lightBuffer.m_data = m_constantBufferDataFromStatePtr->lightBuffer;
	m_dirLightBuffer.m_data = m_constantBufferDataFromStatePtr->dirBuffer;
	m_dirLight = m_constantBufferDataFromStatePtr->dirBuffer;
	m_fogBuffer.m_data = m_constantBufferDataFromStatePtr->fogBuffer;
	m_dirLightBuffer.upd();
	m_fogBuffer.m_data.cameraPos = cameraPos;
	m_fogBuffer.upd();
	m_lightBuffer.upd();

	m_statusTextHandler->update(dt);
}

void ViewLayer::render(iGameState* gameState)
{
	// Clear Frame
	m_annotation->BeginEvent(L"Clear");
	m_deviceContext->ClearRenderTargetView(m_outputRTV.Get(), clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_annotation->EndEvent();

	// Shadow Mapping
	{
		m_annotation->BeginEvent(L"ShadowMapping");
		m_shadowInstance.bindViewsAndRenderTarget();
		DirectX::XMMATRIX viewPMtrx = (*m_viewMatrix) * (*m_projectionMatrix);
		for (size_t i = 0; i < m_gameObjectsFromState->size(); i++)
		{
			// Get indexes
			GameObject* gObject = m_gameObjectsFromState->at(i);
			if (gObject->visible())
			{
				Model* model = gObject->getModelPtr();
				if (!model->is_loaded()) { continue; }

				int wvpIndex = gObject->getWvpCBufferIndex();
				// Set Constant buffer
				m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAdressOf());
				// Draw Model
				model->draw();
			}
		}
		//Active room draw
		if (m_gameObjectsFromActiveRoom != nullptr)
		{
			for (size_t i = 0; i < m_gameObjectsFromActiveRoom->size(); i++)
			{
				// Get indexes
				GameObject* gObject = m_gameObjectsFromActiveRoom->at(i);
				if (gObject->visible())
				{
					Model* model = gObject->getModelPtr();
					if (!model->is_loaded()) { continue; }

					int wvpIndex = gObject->getWvpCBufferIndex();
					// Set Constant buffer
					m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAdressOf());
					// Draw Model
					model->draw();
				}
			}
		}
		m_annotation->EndEvent();
	}

	// Set Render Target
	m_annotation->BeginEvent(L"Draw Meshes");
	m_deviceContext->OMSetRenderTargets(1, m_outputRTV.GetAddressOf(), m_depthStencilView.Get());
	//m_deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	m_deviceContext->RSSetState(m_states->CullClockwise());
	m_deviceContext->RSSetViewports(1, &m_viewport);

	// Set Shaders
	m_shaders.setShaders();

	// Set Sampler
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

	// Set Constant Buffer
	m_deviceContext->PSSetConstantBuffers(1, 1, m_lightBuffer.GetAdressOf());
	m_deviceContext->PSSetConstantBuffers(2, 1, m_dirLightBuffer.GetAdressOf());
	m_deviceContext->PSSetConstantBuffers(3, 1, m_fogBuffer.GetAdressOf());

	// Set Shadow Map
	m_deviceContext->PSSetShaderResources(1, 1, m_shadowInstance.getShadowMapSRVConstPtr());

	// Draw
	DirectX::XMMATRIX viewPMtrx = (*m_viewMatrix) * (*m_projectionMatrix);
	for (size_t i = 0; i < m_gameObjectsFromState->size(); i++)
	{
		// Get indexes
		GameObject* gObject = m_gameObjectsFromState->at(i);
		if (gObject->visible())
		{
			Model* model = gObject->getModelPtr();
			if (!model->is_loaded()) { continue; }

			int wvpIndex = gObject->getWvpCBufferIndex();
			// Set Constant buffer
			m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAdressOf());
			// Draw Model
			model->m_material.setTexture(gObject->getTexturePath().c_str());
			model->draw();
		}
	}
	//Active room draw
	if (m_gameObjectsFromActiveRoom != nullptr)
	{
		for (size_t i = 0; i < m_gameObjectsFromActiveRoom->size(); i++)
		{
			// Get indexes
			GameObject* gObject = m_gameObjectsFromActiveRoom->at(i);
			if (gObject->visible())
			{
				Model* model = gObject->getModelPtr();
				if (!model->is_loaded()) { continue; }

				int wvpIndex = gObject->getWvpCBufferIndex();
				// Set Constant buffer
				m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAdressOf());
				// Draw Model
				model->m_material.setTexture(gObject->getTexturePath().c_str());
				model->draw();
			}
		}
	}
	m_deviceContext->PSSetShaderResources(1, 1, &m_nullSRV);
	m_annotation->EndEvent();

	// Sky
	m_annotation->BeginEvent(L"Sky");
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	m_skyShaders.setShaders();
	
	m_cameraBuffer.m_data = XMMatrixScaling(-1.f, -1.f, -1.f) * (*m_viewMatrix) * (*m_projectionMatrix);
	m_cameraBuffer.upd();
	m_deviceContext->VSSetConstantBuffers(0, 1, m_cameraBuffer.GetAdressOf());
	m_deviceContext->PSSetConstantBuffers(2, 1, m_dirLightBuffer.GetAdressOf());

	m_skyCube.draw();
	m_annotation->EndEvent();

	// Draw Transition Quad
	m_annotation->BeginEvent(L"Transition");
	m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_transition->render();
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	m_annotation->EndEvent();

	// Draw Primitives
	if (m_drawPrimitives)
	{
		m_annotation->BeginEvent(L"Primitives");
		m_deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
		m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
		m_deviceContext->RSSetState(m_states->CullNone());
		m_deviceContext->IASetInputLayout(m_batchInputLayout.Get());

		m_batch->Begin();

		m_effect->SetView(*(m_viewMatrix));
		m_effect->SetProjection(*(m_projectionMatrix));
		// Apply Effect
		m_effect->Apply(m_deviceContext.Get());
		for (size_t i = 0; i < m_gameObjectsFromState->size(); i++)
		{
			if (m_gameObjectsFromState->at(i)->getDrawBB())
			{
				// Draw Primitive
				DX::Draw(m_batch.get(), *(m_gameObjectsFromState->at(i)->getAABBPtr()), DirectX::Colors::Blue);
			}
		}
		if (m_gameObjectsFromActiveRoom != nullptr)
		{
			for (size_t i = 0; i < m_gameObjectsFromActiveRoom->size(); i++)
			{
				if (m_gameObjectsFromActiveRoom->at(i)->collidable())
				{
					// Draw Primitive
					DX::Draw(m_batch.get(), *(m_gameObjectsFromActiveRoom->at(i)->getAABBPtr()), DirectX::Colors::Blue);
				}
			}
		}
		if (m_orientedBoundingBoxesFromActiveRoom != nullptr)
		{
			for (size_t i = 0; i < m_orientedBoundingBoxesFromActiveRoom->size(); i++)
			{
				DX::Draw(m_batch.get(), m_orientedBoundingBoxesFromActiveRoom->at(i), DirectX::Colors::Red);
			}
		}
		if (m_boundingBoxesFromActiveRoom != nullptr)
		{
			for (size_t i = 0; i < m_boundingBoxesFromActiveRoom->size(); i++)
			{
				DX::Draw(m_batch.get(), m_boundingBoxesFromActiveRoom->at(i), DirectX::Colors::Red);
			}
		}
		if (m_triggerBoxes != nullptr)
		{
			for (size_t i = 0; i < m_triggerBoxes->size(); i++)
			{
				DX::Draw(m_batch.get(), m_triggerBoxes->at(i), DirectX::Colors::DarkTurquoise);
			}
		}
		//DX::Draw(m_batch.get(), m_pyramidOBB, DirectX::Colors::Blue);

		m_batch->End();
		m_annotation->EndEvent();
	}

	m_annotation->BeginEvent(L"Sprites");
	// Draw Sprites
	m_deviceContext->RSSetState(m_spriteRasterizerState.Get());
	
	// - Sprite End
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->AlphaBlend());
	gameState->drawUI(m_spriteBatch.get(), m_spriteFont16.get());

	m_annotation->EndEvent();

	// - Shadowmap Debug
	if (drawShadowmapDebug)
	{
		m_annotation->BeginEvent(L"Shadowmap Debug");
		RECT shadowMapImg;
		shadowMapImg.left = 0;
		shadowMapImg.top = 0;
		shadowMapImg.right = 500;
		shadowMapImg.bottom = 500;
		m_spriteBatch->Draw(m_shadowInstance.getShadowMapSRV(), shadowMapImg);
		m_annotation->EndEvent();
	}
	
	// - Sprite End
	m_spriteFont16->DrawString(m_spriteBatch.get(), m_fpsString.c_str(), DirectX::XMFLOAT2((float)m_options->width - 110.f, 10.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	m_statusTextHandler->render(m_spriteFont32.get(), m_spriteBatch.get());
	m_spriteBatch->End();
	m_annotation->EndEvent();

	// Swap Frames
	m_swapChain->Present(0, 0);
}

void ViewLayer::toggleDrawPrimitives(bool toggle)
{
	m_drawPrimitives = toggle;
}