#include "pch.h"
#include "ShadowMapInstance.h"

ShadowMapInstance::ShadowMapInstance()
{
	this->m_deviceContext = nullptr;
	this->m_width = 0;
	this->m_height = 0;
}

ShadowMapInstance::~ShadowMapInstance() {}

void ShadowMapInstance::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
{
	// Device
	this->m_deviceContext = deviceContext;

	// Dimensions
	this->m_width = width;
	this->m_height = height;

	// Viewport
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = (float)this->m_width;
	m_viewport.Height = (float)this->m_height;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;

	// Resources
	// Texture 2D
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = this->m_width;
	textureDesc.Height = this->m_height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* depthMap;
	ZeroMemory(&depthMap, sizeof(depthMap));
	HRESULT hr = device->CreateTexture2D(&textureDesc, 0, &depthMap);
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map texture!");

	// Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthMap, &depthStencilViewDesc, this->m_shadowMapDSV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil view!");

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(depthMap, &shaderResourceViewDesc, this->m_shadowMapSRV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map shader resource view!");

	depthMap->Release();

	// Pipeline States
	// Rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.DepthBias = 100000;
	rasterizerDesc.DepthBiasClamp = 0.f;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.SlopeScaledDepthBias = 1.f;

	hr = device->CreateRasterizerState(&rasterizerDesc, this->m_rasterizerState.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map rasterizer state!");

	// Sampler
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 0.f;
	samplerDesc.BorderColor[1] = 0.f;
	samplerDesc.BorderColor[2] = 0.f;
	samplerDesc.BorderColor[3] = 0.f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device->CreateSamplerState(&samplerDesc, this->m_samplerState.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error when creating shadow map sampler state!");

	// Shaders
	ShaderFiles shaderFiles;
	shaderFiles.vs = L"Shader Files\\ShadowMapVS.hlsl";
	this->m_shadowMapShaders.initialize(device, deviceContext, shaderFiles);

	// World Bounding Sphere
	this->m_worldBoundingSphere.Center = { 0.f, 0.f, 0.f };
	this->m_worldBoundingSphere.Radius = sqrtf(10.f * 10.f + 15.f * 15.f);
}

void ShadowMapInstance::buildLightMatrix(PS_DIR_BUFFER directionalLight)
{
	// Light View Matrix
	XMVECTOR lightDirection = XMLoadFloat4(&directionalLight.lightDirection);
	XMVECTOR lightPosition = -2.0f * this->m_worldBoundingSphere.Radius * lightDirection;
	XMVECTOR targetPosition = XMLoadFloat3(&this->m_worldBoundingSphere.Center);
	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	this->m_lightViewMatrix = XMMatrixLookAtLH(lightPosition, targetPosition, up);

	// Transform World Bounding Sphere to Light Local View Space
	XMFLOAT3 worldSphereCenterLightSpace;
	XMStoreFloat3(&worldSphereCenterLightSpace, XMVector3TransformCoord(targetPosition, this->m_lightViewMatrix));

	// Construct Orthographic Frustum in Light View Space
	float viewLeft		= worldSphereCenterLightSpace.x - this->m_worldBoundingSphere.Radius;
	float viewRight		= worldSphereCenterLightSpace.y - this->m_worldBoundingSphere.Radius;
	float viewBottom	= worldSphereCenterLightSpace.z - this->m_worldBoundingSphere.Radius;
	float viewTop		= worldSphereCenterLightSpace.x + this->m_worldBoundingSphere.Radius;
	float nearZ			= worldSphereCenterLightSpace.y + this->m_worldBoundingSphere.Radius;
	float farZ			= worldSphereCenterLightSpace.z + this->m_worldBoundingSphere.Radius;

	// Local Projection Matrix
	this->m_lightProjectionMatrix = XMMatrixOrthographicOffCenterLH(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);

	// Shadow Texture Space Transformation
	XMMATRIX textureSpaceMatrix
	(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f
	);

	this->m_shadowTranformMatrix = this->m_lightViewMatrix * this->m_lightProjectionMatrix * textureSpaceMatrix;
}

void ShadowMapInstance::update()
{
}

ID3D11ShaderResourceView* ShadowMapInstance::getShadowMapSRV()
{
	return this->m_shadowMapSRV.Get();
}

void ShadowMapInstance::bindViewsAndRenderTarget()
{
	this->m_deviceContext->RSSetState(this->m_rasterizerState.Get());
	this->m_deviceContext->RSSetViewports(1, &this->m_viewport);
	this->m_deviceContext->OMSetRenderTargets(1, m_rendertarget, this->m_shadowMapDSV.Get());
	this->m_deviceContext->ClearDepthStencilView(this->m_shadowMapDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0.f);
	this->m_shadowMapShaders.setShaders();
}
