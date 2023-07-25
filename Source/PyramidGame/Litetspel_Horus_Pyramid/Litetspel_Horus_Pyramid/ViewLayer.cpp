#include "pch.h"
#include "ViewLayer.h"
#include "PyramidMathTools.h"

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

void ViewLayer::initRenderTarget(RenderTexture& rtv, std::string name, UINT width, UINT height, DXGI_FORMAT format, UINT mipLevels)
{
	size_t length = name.copy(rtv.name, name.length(), 0);
	rtv.name[length] = '\0';
	
	// Texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = mipLevels;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	if (mipLevels > 1)
		textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

	HRESULT hr = m_device->CreateTexture2D(&textureDesc, NULL, &rtv.rtt);
	assert(SUCCEEDED(hr) && "Error, render target texture could not be created!");

	// Render Rarget View
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateRenderTargetView(rtv.rtt, &renderTargetViewDesc, &rtv.rtv);
	assert(SUCCEEDED(hr) && "Error, render target view could not be created!");

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = mipLevels;

	hr = m_device->CreateShaderResourceView(rtv.rtt, &srvDesc, &rtv.srv);
	assert(SUCCEEDED(hr) && "Error, shader resource view could not be created!");

	// Unordered Access View
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = textureDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateUnorderedAccessView(rtv.rtt, &uavDesc, &rtv.uav);
	assert(SUCCEEDED(hr) && "Error, unordered access view could not be created!");
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
	dsBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
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
	dsViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Flags = 0;
	dsViewDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &dsViewDesc, &m_depthStencilView);
	assert(SUCCEEDED(hr) && "Error, failed to create depth stencil view!");

	// Create Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = m_gBuffer[DEPTH_GB].format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = m_device->CreateShaderResourceView(m_depthStencilBuffer.Get(), &srvDesc, &m_gBuffer[DEPTH_GB].srv);
	assert(SUCCEEDED(hr) && "Error, shader resource view could not be created!");

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
	shaders.vs = L"Shader Files\\GeneralVS.hlsl";
	shaders.ps = L"Shader Files\\GBufferPS.hlsl";
	m_GBufferShaders.initialize(m_device.Get(), m_deviceContext.Get(), shaders);

	shaders.vs = L"Shader Files\\FullscreenQuadVS.hlsl";
	shaders.ps = L"Shader Files\\LightingPS.hlsl";
	m_lightingShaders.initialize(m_device.Get(), m_deviceContext.Get(), shaders);

	shaders.vs = L"Shader Files\\GeneralVS.hlsl";
	shaders.ps = L"Shader Files\\ForwardLightingPS.hlsl";
	m_forwardLightingShaders.initialize(m_device.Get(), m_deviceContext.Get(), shaders);

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
	
	m_cameraPtr= nullptr;
	m_fps = 0;
}

ViewLayer::~ViewLayer()
{
	// RT already released in ssao instance
	m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].rtt = nullptr;
	m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].rtv = nullptr;
	m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].srv = nullptr;
	m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].uav = nullptr;
}

ID3D11Device* ViewLayer::getDevice()
{
	return m_device.Get();
}

ID3D11DeviceContext* ViewLayer::getContextDevice()
{
	return m_deviceContext.Get();
}

void ViewLayer::setCamera(Camera* camera)
{
	m_cameraPtr = camera;
	m_SSAOInstance.updateProjMatrix(m_cameraPtr->getProjectionMatrix());
	m_blurCData.projectionMatrix = m_cameraPtr->getProjectionMatrix();
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

void ViewLayer::setPerFrameDataFromState(PS_PER_FRAME_BUFFER* perFrameDataFromState)
{
	m_constantBufferDataFromStatePtr = perFrameDataFromState;
	m_perFrameBuffer.m_data = *m_constantBufferDataFromStatePtr;
	m_perFrameBuffer.upd();
}


void ViewLayer::setWvpCBufferFromState(std::vector< ConstBuffer<VS_WVPW_CONSTANT_BUFFER> >* buffers)
{
	m_wvpCBufferFromState = buffers;
}


void ViewLayer::initConstantBuffer()
{
	m_perFrameBuffer.init(m_device.Get(), m_deviceContext.Get());
	m_skyboxCameraBuffer.init(m_device.Get(), m_deviceContext.Get());
	m_inverseMatricesBuffer.init(m_device.Get(), m_deviceContext.Get());
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

void ViewLayer::initSSAOBlurPass()
{
	ShaderFiles shaderFiles;
	shaderFiles.cs = L"Shader Files\\EdgePreservingBlurCS.hlsl";
	m_edgePreservingBlurCS.initialize(m_device.Get(), m_deviceContext.Get(), shaderFiles, LayoutType::POS_TEX);
	
	m_blurCData.direction = 0;
	float twoSigma2 = 2.0f * m_ssaoBlurSigma * m_ssaoBlurSigma;

	// Estimate the blur radius based on sigma since sigma controls the "width" of the bell curve.
	// For example, for sigma = 3, the width of the bell curve is 
	int blurRadius = (int)ceil(2.f * m_ssaoBlurSigma);
	m_blurCData.radius = blurRadius;

	assert(blurRadius <= MAX_BLUR_RADIUS);

	float weightSum = 0.0f;

	for (int i = -blurRadius; i <= blurRadius; ++i)
	{
		float x = (float)i;

		m_blurCData.weights[i + blurRadius] = expf(-x * x / twoSigma2);
		weightSum += m_blurCData.weights[i + blurRadius];
	}

	// Divide by the sum so all the weights add up to 1.0.
	float weightLength = 2.f * (float)blurRadius + 1.f;
	for (int i = 0; i < weightLength; ++i)
		m_blurCData.weights[i] /= weightSum;

	m_blurCBuffer.init(m_device.Get(), m_deviceContext.Get());
	m_blurCBuffer.m_data = m_blurCData;

	// Texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = m_options->width;
	textureDesc.Height = m_options->height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_gBuffer[AMBIENT_OCCLUSION_GB].format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* texture;
	HRESULT hr = m_device->CreateTexture2D(&textureDesc, NULL, &texture);
	assert(SUCCEEDED(hr) && "Error, blur pass ping pong texture could not be created!");

	// SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = m_gBuffer[AMBIENT_OCCLUSION_GB].format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = m_device->CreateShaderResourceView(texture, &srvDesc, m_blurPingPongSRV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, blur pass ping pong shader resource view could not be created!");

	// UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = textureDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateUnorderedAccessView(texture, &uavDesc, m_blurPingPongUAV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, blur pass ping pong unordered access view could not be created!");

	texture->Release();
}

void ViewLayer::blurSSAOPass()
{
	UINT cOffset = -1;

	m_deviceContext->OMSetRenderTargets(1, &m_nullRTV, NULL);
	m_deviceContext->CSSetUnorderedAccessViews(0, 1, &m_nullUAV, &cOffset);
	m_deviceContext->PSSetShaderResources(0, 2, &m_nullSRV);

	m_edgePreservingBlurCS.setShaders();

	static uint32_t AOiterator = 0;
	AOiterator++;
	RenderTexture& aoRT = m_SSAOInstance.getAORenderTexture();
	aoRT.name[9] = std::to_string(AOiterator).c_str()[0];
	ID3D11ShaderResourceView* blurSRVs[] = { aoRT.srv, m_blurPingPongSRV.Get() };
	ID3D11UnorderedAccessView* blurUAVs[] = { m_blurPingPongUAV.Get(), aoRT.uav };

	for (UINT i = 0; i < 2; i++)
	{
		// Blur Constant Buffer
		m_blurCData.direction = i;

		m_blurCBuffer.m_data = m_blurCData;
		m_blurCBuffer.upd();
		m_deviceContext->CSSetConstantBuffers(0, 1, m_blurCBuffer.GetAddressOf());

		// Set Rescources
		m_deviceContext->CSSetShaderResources(0, 1, &blurSRVs[m_blurCData.direction]);
		m_deviceContext->CSSetShaderResources(1, 1, &m_gBuffer[DEPTH_GB].srv);
		m_deviceContext->CSSetShaderResources(2, 1, &m_gBuffer[NORMAL_SHADOWMASK_GB].srv);
		m_deviceContext->CSSetUnorderedAccessViews(0, 1, &blurUAVs[m_blurCData.direction], &cOffset);

		// Dispatch Shader
		UINT clientWidth = (UINT)std::ceil((m_options->width / 16.f) + 0.5f);
		UINT clientHeight = (UINT)std::ceil((m_options->height / 16.f) + 0.5f);
		m_deviceContext->Dispatch(clientWidth, clientHeight, 1);

		// Unbind Unordered Access View and Shader Resource View
		m_deviceContext->CSSetShaderResources(0, 1, &m_nullSRV);
		m_deviceContext->CSSetUnorderedAccessViews(0, 1, &m_nullUAV, &cOffset);
	}

	m_deviceContext->OMSetRenderTargets(1, &m_nullRTV, NULL);
	m_deviceContext->CSSetUnorderedAccessViews(0, 1, &m_nullUAV, &cOffset);
	m_deviceContext->CSSetShaderResources(0, 3, &m_nullSRV);
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

	// G-Buffer
	// - Diffuse
	initRenderTarget(m_gBuffer[GBufferType::DIFFUSE_GB], "DIFFUSE_GB", m_options->width, m_options->height, DXGI_FORMAT_R8G8B8A8_UNORM);
	// - Normal, Shadow Mask
	initRenderTarget(m_gBuffer[GBufferType::NORMAL_SHADOWMASK_GB], "NORMAL_SHADOWMASK_GB", m_options->width, m_options->height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	// - Specular, Shininess
	initRenderTarget(m_gBuffer[GBufferType::SPECULAR_SHININESS_GB], "SPECULAR_SHININESS_GB", m_options->width, m_options->height, DXGI_FORMAT_R8G8B8A8_UNORM);
	// - Ambient, Global Ambient Contribution
	initRenderTarget(m_gBuffer[GBufferType::AMBIENT_GLOBALAMBIENTCONTR_GB], "AMBIENT_GLOBALAMBIENTCONTR_GB", m_options->width, m_options->height, DXGI_FORMAT_R8G8B8A8_UNORM);

	// SSAO
	m_SSAOInstance.initialize(m_device.Get(), m_deviceContext.Get(), m_options->width, m_options->height);
	m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB] = m_SSAOInstance.getAORenderTexture();

	initSSAOBlurPass();

	// Lights
	PointLight pLight;
	pLight.position = { 0, 10, 0 };
	pLight.diffuse = { 0, 0, 0};
	pLight.range = 100;
	// - Ambient Light buffer
	PS_PER_FRAME_BUFFER perFrameData;
	perFrameData.nrOfPointLights = 1;
	perFrameData.pointLights[0] = pLight;
	// - Directional Light buffer
	perFrameData.skyLightColor = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	perFrameData.skyLightIntensity = 1.f;
	perFrameData.skyLightDirection = DirectX::XMFLOAT3(-0.8f, 1.0f, -0.7f);
	// - Fog, Just set for init, change in room with m_fogData member variable.
	perFrameData.fogStart = 50.0f;
	perFrameData.fogEnd = 100.0f;
	perFrameData.ambientColor = XMFLOAT3(0.8f, 0.8f, 0.8f);

	m_perFrameBuffer.m_data = perFrameData;
	m_perFrameBuffer.upd();

	// Pyramid Oriented Bounding Box for drawing only(Seperate from box in gamestate)
	DirectX::XMFLOAT3 center(0.f, 62.f, 80.f);
	DirectX::XMFLOAT3 extents(450.f, 550.f, 1.f);
	constexpr float rotationX = XMConvertToRadians(46.f);
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
	m_GBufferShaders.reloadShaders();
	m_lightingShaders.reloadShaders();
	m_forwardLightingShaders.reloadShaders();
	m_SSAOInstance.updateShaders();
	m_skyShaders.reloadShaders();
}

void ViewLayer::update(float dt)
{
	// Transition
	m_transition->update(dt);

	// FPS Counter
	m_fps++;
	if (1.0 < m_timer.timeElapsed())
	{
		m_fpsString = "FPS: " + std::to_string(m_fps);
		m_timer.restart();
		m_fps = 0;
	}

	// SSAO
	if (m_ssaoToggle)
	{
		m_SSAOInstance.updateViewMatrix(m_cameraPtr->getViewMatrix());
	}

	// Shadow matrix and per frame buffer
	if (m_shadowMappingToggle) {
		XMFLOAT3 cameraPos = m_cameraPtr->getPosition();
		m_shadowInstance.buildLightMatrix(m_perFrameBuffer.m_data.skyLightDirection, cameraPos);
		m_perFrameBuffer.m_data.cameraPos = cameraPos;
	}

	// Status text
	m_statusTextHandler->update(dt);
}

void ViewLayer::render(iGameState* gameState)
{
	m_shadowmapDrawCallCount = 0;
	m_drawCallCount = 0;
	m_postProcessDrawCallCount = 0;

	// Clear Frame
	m_annotation->BeginEvent(L"Clear");
	m_deviceContext->ClearRenderTargetView(m_outputRTV.Get(), m_clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	ID3D11RenderTargetView* gBufferRTVs[] = {
		m_gBuffer[GBufferType::DIFFUSE_GB].rtv,
		m_gBuffer[GBufferType::NORMAL_SHADOWMASK_GB].rtv,
		m_gBuffer[GBufferType::SPECULAR_SHININESS_GB].rtv,
		m_gBuffer[GBufferType::AMBIENT_GLOBALAMBIENTCONTR_GB].rtv,
		m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].rtv
	};
	m_deviceContext->ClearRenderTargetView(gBufferRTVs[DIFFUSE_GB], m_clearColorBlack);
	m_deviceContext->ClearRenderTargetView(gBufferRTVs[NORMAL_SHADOWMASK_GB], m_clearColorWhite);
	m_deviceContext->ClearRenderTargetView(gBufferRTVs[SPECULAR_SHININESS_GB], m_clearColorBlack);
	m_deviceContext->ClearRenderTargetView(gBufferRTVs[AMBIENT_GLOBALAMBIENTCONTR_GB], m_clearColorBlack);
	if (m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].rtv)
	{
		m_deviceContext->ClearRenderTargetView(gBufferRTVs[AMBIENT_OCCLUSION_GB], m_clearColorWhite);
	}

	if (!m_shadowMappingToggle) 
	{
		m_shadowInstance.clearShadowmap();
	}

	// Set Samplers
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
	m_shadowInstance.setComparisonSampler();

	m_annotation->EndEvent();

	DirectX::XMMATRIX viewMatrix = m_cameraPtr->getViewMatrix();
	DirectX::XMMATRIX projMatrix = m_cameraPtr->getProjectionMatrix();
	DirectX::BoundingFrustum cameraFrustum;
	cameraFrustum.CreateFromMatrix(cameraFrustum, projMatrix);
	cameraFrustum.Transform(cameraFrustum, 1.f, DirectX::XMQuaternionRotationRollPitchYawFromVector(m_cameraPtr->getRotationVector()), m_cameraPtr->getPositionVector());

	// Shadow Mapping
	DirectX::BoundingOrientedBox shadowCameraBounds;
	if (m_shadowMappingToggle) {
		shadowCameraBounds = m_shadowInstance.getLightBoundingBox();
		m_annotation->BeginEvent(L"ShadowMapping");
		m_shadowInstance.bindViewsAndRenderTarget();
		for (size_t i = 0; i < m_gameObjectsFromState->size(); i++)
		{
			// Get indexes
			GameObject* gObject = m_gameObjectsFromState->at(i);
			if (gObject->visible())
			{
				// Get Model Mesh
				Model* model = gObject->getModelPtr();
				if (!model->is_loaded()) { continue; }

				// Frustum Culling
				if (m_frustumCullingToggle)
				{
					assert(!DirectX::XMVector3IsNaN(gObject->getMoveCompPtr()->position));

					DirectX::BoundingOrientedBox obb;
					DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, model->m_aabb);
					obb.Transform(obb, gObject->getWorldMatrix());
					if (!shadowCameraBounds.Intersects(obb)) { continue; }
				}

				// Set Constant buffer
				int wvpIndex = gObject->getWvpCBufferIndex();
				m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAddressOf());

				// Draw Model
				model->draw();
				m_shadowmapDrawCallCount++;
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
					// Get Model Mesh
					Model* model = gObject->getModelPtr();
					if (!model->is_loaded()) { continue; }

					// Set Constant buffer
					int wvpIndex = gObject->getWvpCBufferIndex();
					m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAddressOf());
					
					// Draw Model
					model->draw();
					m_shadowmapDrawCallCount++;
				}
			}
		}
		m_annotation->EndEvent();
	}

	m_annotation->BeginEvent(L"G-Buffer");

	// Set Render Target
	m_deviceContext->OMSetRenderTargets(GB_NUM - 1, gBufferRTVs, m_depthStencilView.Get());

	// Set states
	m_deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	m_deviceContext->RSSetState(m_states->CullClockwise());
	m_deviceContext->RSSetViewports(1, &m_viewport);

	// Set Shaders
	m_GBufferShaders.setShaders();

	// Set Shadow Map
	m_deviceContext->PSSetShaderResources(1, 1, m_shadowInstance.getShadowMapSRVConstPtr());

	// G-Buffer Draw
	std::vector<GameObject*> transparentObjects;
	for (size_t i = 0; i < m_gameObjectsFromState->size(); i++)
	{
		// Get indexes
		GameObject* gObject = m_gameObjectsFromState->at(i);
		if (gObject->visible())
		{
			Model* model = gObject->getModelPtr();
			if (!model->is_loaded()) { continue; }

			// Frustum Culling
			if (m_frustumCullingToggle)
			{
				DirectX::BoundingOrientedBox obb;
				DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, model->m_aabb);
				obb.Transform(obb, gObject->getWorldMatrix());
				if (!cameraFrustum.Intersects(obb)) { continue; }
			}

			if (model->m_material.getMaterial()->diffuse.w < 1.f)
			{
				transparentObjects.push_back(gObject);
				continue;
			}

			int wvpIndex = gObject->getWvpCBufferIndex();
			// Set Constant buffer
			m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAddressOf());
			// Draw Model
			model->m_material.setTexture(gObject->getTexturePath().c_str());
			model->draw();
			m_drawCallCount++;
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
				// Get Model Mesh
				Model* model = gObject->getModelPtr();
				if (!model->is_loaded()) { continue; }

				// Frustum Culling
				if (m_frustumCullingToggle)
				{
					DirectX::BoundingOrientedBox obb;
					DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, model->m_aabb);
					obb.Transform(obb, gObject->getWorldMatrix());
					if (!cameraFrustum.Intersects(obb)) { continue; }
				}

				if (model->m_material.getMaterial()->diffuse.w < 1.f)
				{
					transparentObjects.push_back(gObject);
					continue;
				}

				// Set Constant buffer
				int wvpIndex = gObject->getWvpCBufferIndex();
				m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAddressOf());

				// Draw Model
				model->m_material.setTexture(gObject->getTexturePath().c_str());
				model->draw();
				m_drawCallCount++;
			}
		}
	}
	
	m_deviceContext->PSSetShaderResources(0, GB_NUM, m_nullSRVs);
	m_deviceContext->OMSetRenderTargets(GB_NUM - 1, m_nullRTVs, nullptr);
	m_annotation->EndEvent();

	// SSAO
	if (m_ssaoToggle)
	{
		m_annotation->BeginEvent(L"SSAO");
		m_deviceContext->OMSetRenderTargets(1, &m_nullRTV, nullptr);
		m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
		m_deviceContext->RSSetState(m_states->CullNone());
		m_deviceContext->PSSetShaderResources(0, 1, &m_gBuffer[DEPTH_GB].srv);
		m_deviceContext->PSSetShaderResources(1, 1, &m_gBuffer[NORMAL_SHADOWMASK_GB].srv);

		m_SSAOInstance.render();
		m_postProcessDrawCallCount++;
		m_annotation->EndEvent();

		// Blur
		blurSSAOPass();
	}

	// Lighting
	m_annotation->BeginEvent(L"Lighting");
	m_lightingShaders.setShaders();
	m_deviceContext->OMSetRenderTargets(1, m_outputRTV.GetAddressOf(), nullptr);
	m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_deviceContext->RSSetState(m_states->CullNone());

	// Set Constant Buffer
	m_perFrameBuffer.upd();
	m_deviceContext->PSSetConstantBuffers(1, 1, m_perFrameBuffer.GetAddressOf());

	XMVECTOR det = XMMatrixDeterminant(m_cameraPtr->getViewMatrix());
	det = XMMatrixDeterminant(m_cameraPtr->getProjectionMatrix());
	m_inverseMatricesBuffer.m_data.viewMatrix = XMMatrixTranspose(XMMatrixInverse(&det, viewMatrix));
	m_inverseMatricesBuffer.m_data.projMatrix = XMMatrixTranspose(XMMatrixInverse(&det, projMatrix));
	m_inverseMatricesBuffer.upd();
	m_deviceContext->PSSetConstantBuffers(2, 1, m_inverseMatricesBuffer.GetAddressOf());

	// Textures
	ID3D11ShaderResourceView* gBufferSRVs[] = {
		m_gBuffer[GBufferType::DIFFUSE_GB].srv,
		m_gBuffer[GBufferType::NORMAL_SHADOWMASK_GB].srv,
		m_gBuffer[GBufferType::SPECULAR_SHININESS_GB].srv,
		m_gBuffer[GBufferType::AMBIENT_GLOBALAMBIENTCONTR_GB].srv,
		m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].srv,
		m_gBuffer[GBufferType::DEPTH_GB].srv
	};
	m_deviceContext->PSSetShaderResources(0, GB_NUM, gBufferSRVs);

	m_deviceContext->Draw(4, 0);
	m_postProcessDrawCallCount++;
	m_deviceContext->PSSetShaderResources(0, GB_NUM, m_nullSRVs);
	m_annotation->EndEvent();

	// Sky
	m_annotation->BeginEvent(L"Sky");
	m_deviceContext->OMSetRenderTargets(1, m_outputRTV.GetAddressOf(), m_depthStencilView.Get());
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	m_deviceContext->RSSetState(m_states->CullClockwise());
	m_skyShaders.setShaders();
	
	m_skyboxCameraBuffer.m_data = XMMatrixScaling(-1.f, -1.f, -1.f) * viewMatrix * projMatrix;
	m_skyboxCameraBuffer.upd();
	m_deviceContext->VSSetConstantBuffers(0, 1, m_skyboxCameraBuffer.GetAddressOf());
	m_deviceContext->PSSetConstantBuffers(2, 1, m_skyboxSunLightBuffer.GetAddressOf());

	m_skyCube.draw();
	m_drawCallCount++;
	m_annotation->EndEvent();

	// Transparent Meshes
	m_annotation->BeginEvent(L"Transparent Meshes");
	m_deviceContext->OMSetBlendState(m_states->AlphaBlend(), nullptr, 0xFFFFFFFF);
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	m_deviceContext->RSSetViewports(1, &m_viewport);

	m_forwardLightingShaders.setShaders();
	m_deviceContext->PSSetShaderResources(1, 1, m_shadowInstance.getShadowMapSRVConstPtr());
	m_deviceContext->PSSetShaderResources(2, 1, &m_gBuffer[GBufferType::AMBIENT_OCCLUSION_GB].srv);

	m_deviceContext->PSSetConstantBuffers(1, 1, m_perFrameBuffer.GetAddressOf());

	for (size_t i = 0; i < transparentObjects.size(); i++)
	{
		// Get Model Mesh
		GameObject* gObject = transparentObjects[i];
		Model* model = gObject->getModelPtr();

		// Set Constant buffer
		int wvpIndex = gObject->getWvpCBufferIndex();
		m_deviceContext->VSSetConstantBuffers(0, 1, m_wvpCBufferFromState->at(wvpIndex).GetAddressOf());

		// Draw Model
		model->m_material.setTexture(gObject->getTexturePath().c_str());
		model->draw();
		m_drawCallCount++;
	}
	m_deviceContext->PSSetShaderResources(1, 1, &m_nullSRV);
	m_deviceContext->PSSetShaderResources(2, 1, &m_nullSRV);
	m_deviceContext->OMSetRenderTargets(1, m_outputRTV.GetAddressOf(), nullptr);
	m_annotation->EndEvent();

	// Draw Transition Quad
	m_annotation->BeginEvent(L"Transition");
	m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_transition->render();
	m_postProcessDrawCallCount++;
	m_deviceContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	m_annotation->EndEvent();

	// Draw Primitives
	if (m_drawPhysicsPrimitives || m_drawLightsDebug || m_drawModelBoxPrimitives)
	{
		m_annotation->BeginEvent(L"Primitives");
		m_deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
		m_deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
		m_deviceContext->RSSetState(m_states->CullNone());
		m_deviceContext->IASetInputLayout(m_batchInputLayout.Get());

		m_batch->Begin();

		m_effect->SetView(viewMatrix);
		m_effect->SetProjection(projMatrix);
		// Apply Effect
		m_effect->Apply(m_deviceContext.Get());

		// Physics
		if (m_drawPhysicsPrimitives)
		{
			m_annotation->BeginEvent(L"Physics");
			for (size_t i = 0; i < m_gameObjectsFromState->size(); i++)
			{
				if (m_gameObjectsFromState->at(i)->shouldDrawBB())
				{
					DX::Draw(m_batch.get(), *(m_gameObjectsFromState->at(i)->getAABBPtr()), DirectX::Colors::Blue);
				}
			}
			if (m_gameObjectsFromActiveRoom != nullptr)
			{
				for (size_t i = 0; i < m_gameObjectsFromActiveRoom->size(); i++)
				{
					if (m_gameObjectsFromActiveRoom->at(i)->collidable())
					{
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
			m_annotation->EndEvent();
		}

		// Model Bounding Boxes
		if (m_drawModelBoxPrimitives)
		{
			for (size_t i = 0; i < m_gameObjectsFromState->size(); i++)
			{
				DX::Draw(m_batch.get(), shadowCameraBounds, DirectX::Colors::Firebrick);

				// Get indexes
				GameObject* gObject = m_gameObjectsFromState->at(i);
				if (gObject->visible())
				{
					Model* model = gObject->getModelPtr();
					if (!model->is_loaded()) { continue; }

					// Oriented Bounding Box Setup
					DirectX::BoundingOrientedBox obb;
					DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, model->m_aabb);
					obb.Transform(obb, gObject->getWorldMatrix());

					DX::Draw(m_batch.get(), obb, DirectX::Colors::DodgerBlue);
				}
			}
			// Active room draw
			if (m_gameObjectsFromActiveRoom != nullptr)
			{
				for (size_t i = 0; i < m_gameObjectsFromActiveRoom->size(); i++)
				{
					// Get indexes
					GameObject* gObject = m_gameObjectsFromActiveRoom->at(i);
					if (gObject->visible())
					{
						// Get Model Mesh
						Model* model = gObject->getModelPtr();
						if (!model->is_loaded()) { continue; }

						// Oriented Bounding Box Setup
						DirectX::BoundingOrientedBox obb;
						DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, model->m_aabb);
						obb.Transform(obb, gObject->getWorldMatrix());

						if (!shadowCameraBounds.Intersects(obb)) { continue; }

						DX::Draw(m_batch.get(), obb, DirectX::Colors::DodgerBlue);
					}
				}
			}
		}

		// Lights
		if (m_drawLightsDebug)
		{
			// Point Lights
			BoundingSphere sphere;
			for (uint32_t i = 0; i < m_perFrameBuffer.m_data.nrOfPointLights; i++)
			{
				sphere.Center = m_perFrameBuffer.m_data.pointLights[i].position;
				sphere.Radius = m_perFrameBuffer.m_data.pointLights[i].range;
				DX::Draw(m_batch.get(), sphere, DirectX::Colors::LightGoldenrodYellow);
			}
		}

		m_batch->End();
		m_annotation->EndEvent();
	}

	// Draw Sprites
	m_annotation->BeginEvent(L"Sprites");
	m_deviceContext->RSSetState(m_spriteRasterizerState.Get());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->AlphaBlend());

	// - Render Pass Textures Debug
	if (m_drawShadowmapDebug)
	{
		m_annotation->BeginEvent(L"Shadowmap Debug");
		long dimension = 500;
		RECT shadowMapImg;
		shadowMapImg.top = 0;
		shadowMapImg.bottom = 500;
		shadowMapImg.left = (long)m_options->width - dimension;
		shadowMapImg.right = (long)m_options->width;
		m_spriteBatch->Draw(m_shadowInstance.getShadowMapSRV(), shadowMapImg);
		m_annotation->EndEvent();
	}

	uint32_t gBufferDebugtextureCount = GB_NUM;
	if (m_drawGBufferDebug)
	{
		m_annotation->BeginEvent(L"G-Buffer Debug");
		uint32_t width = m_options->width / gBufferDebugtextureCount;
		uint32_t height = m_options->height / gBufferDebugtextureCount;

		RECT img;
		img.top = 0;
		img.left = 0;
		img.right = width;
		img.bottom = height;

		for (uint32_t i = 0; i < GB_NUM; i++)
		{
			m_spriteBatch->Draw(m_gBuffer[i].srv, img);
			img.top = img.bottom;
			img.bottom = img.bottom + height;
		}
		m_annotation->EndEvent();
	}

	// - Scene UI
	gameState->drawUI(m_spriteBatch.get(), m_spriteFont16.get());

	// - Render Debug Stats UI
	if (m_drawDrawCallStatsDebug)
	{
		std::string drawCallCountStr = "Shadow Draws: " + std::to_string(m_shadowmapDrawCallCount);
		m_spriteFont16->DrawString(m_spriteBatch.get(), drawCallCountStr.c_str(), DirectX::XMFLOAT2(10.f, 50.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	
		drawCallCountStr = "Geometry Draws: " + std::to_string(m_drawCallCount);
		m_spriteFont16->DrawString(m_spriteBatch.get(), drawCallCountStr.c_str(), DirectX::XMFLOAT2(10.f, 70.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	
		drawCallCountStr = "Post Process Draws: " + std::to_string(m_postProcessDrawCallCount);
		m_spriteFont16->DrawString(m_spriteBatch.get(), drawCallCountStr.c_str(), DirectX::XMFLOAT2(10.f, 90.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	}

	// - FPS Counter UI
	m_spriteFont16->DrawString(m_spriteBatch.get(), m_fpsString.c_str(), DirectX::XMFLOAT2((float)m_options->width - 110.f, 10.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f));
	
	// - Status Text UI
	m_statusTextHandler->render(m_spriteFont32.get(), m_spriteBatch.get());
	
	// Draw Sprites End
	m_spriteBatch->End();
	m_annotation->EndEvent();

	// Swap Frames
	m_swapChain->Present(0, 0);
}