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
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = this->m_width;
	textureDesc.Height = this->m_height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, 0, &this->m_shadowMapTexture);
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map texture!");

	// Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(this->m_shadowMapTexture.Get(), &depthStencilViewDesc, this->m_shadowMapDSV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil view!");

	//// Depth Stencil State
	//D3D11_DEPTH_STENCIL_DESC dsDesc;
	//ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	//// Stencil test parameters
	//dsDesc.StencilEnable = true;
	//dsDesc.StencilReadMask = 0xFF;
	//dsDesc.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing
	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing
	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//hr = device->CreateDepthStencilState(&dsDesc, &this->m_depthStencilState);
	//assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil state!");

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(this->m_shadowMapTexture.Get(), &shaderResourceViewDesc, this->m_shadowMapSRV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map shader resource view!");

	// Pipeline States

	// Sampler
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.0f;
	comparisonSamplerDesc.BorderColor[1] = 1.0f;
	comparisonSamplerDesc.BorderColor[2] = 1.0f;
	comparisonSamplerDesc.BorderColor[3] = 1.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

	hr = device->CreateSamplerState(&comparisonSamplerDesc, this->m_comparisonSampler.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error when creating shadow map sampler state!");

	// Rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.DepthBias = 100000;
	rasterizerDesc.DepthBiasClamp = 0.f;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.SlopeScaledDepthBias = 1.f;
	rasterizerDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&rasterizerDesc, this->m_rasterizerState.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map rasterizer state!");

	// Shaders
	ShaderFiles shaderFiles;
	shaderFiles.vs = L"Shader Files\\ShadowMapVS.hlsl";
	this->m_shadowMapShaders.initialize(device, deviceContext, shaderFiles);

	// World Bounding Sphere
	this->m_worldBoundingSphere.Center = { 0.f, 0.f, 0.f };
	this->m_worldBoundingSphere.Radius = sqrtf(10.f * 10.f + 15.f * 15.f);

	// Constant Buffer
	this->m_shadowCBuffer.init(device, deviceContext);
}

void ShadowMapInstance::buildLightMatrix(PS_DIR_BUFFER directionalLight)
{
	//// Light View Matrix
	//XMVECTOR lightDirection = XMLoadFloat4(&directionalLight.lightDirection);
	//XMVECTOR lightPosition = -2.0f * this->m_worldBoundingSphere.Radius * lightDirection;
	//XMVECTOR targetPosition = XMLoadFloat3(&this->m_worldBoundingSphere.Center);
	//XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	//this->m_lightViewMatrix = XMMatrixLookAtLH(lightPosition, targetPosition, up);

	//// Transform World Bounding Sphere to Light Local View Space
	//XMFLOAT3 worldSphereCenterLightSpace;
	//XMStoreFloat3(&worldSphereCenterLightSpace, XMVector3TransformCoord(targetPosition, this->m_lightViewMatrix));

	//// Construct Orthographic Frustum in Light View Space
	//float l = worldSphereCenterLightSpace.x - this->m_worldBoundingSphere.Radius;
	//float r = worldSphereCenterLightSpace.x + this->m_worldBoundingSphere.Radius;
	//float b = worldSphereCenterLightSpace.y - this->m_worldBoundingSphere.Radius;
	//float t = worldSphereCenterLightSpace.y + this->m_worldBoundingSphere.Radius;
	//float n = worldSphereCenterLightSpace.z - this->m_worldBoundingSphere.Radius;
	//float f = worldSphereCenterLightSpace.z + this->m_worldBoundingSphere.Radius;

	//// Local Projection Matrix
	//this->m_lightProjectionMatrix = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	//// Shadow Texture Space Transformation
	//XMMATRIX textureSpaceMatrix
	//(
	//	0.5f, 0.0f, 0.0f, 0.0f,
	//	0.0f, -.5f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.5f, 0.5f, 0.0f, 1.0f
	//);
	//this->m_shadowTranformMatrix = this->m_lightViewMatrix * this->m_lightProjectionMatrix * textureSpaceMatrix;

	this->m_lightProjectionMatrix = XMMatrixPerspectiveFovRH(
		XM_PIDIV2,
		1.0f,
		12.f,
		50.f
	);

	// Point light at (20, 15, 20), pointed at the origin. POV up-vector is along the y-axis.
	static const XMVECTORF32 eye = { 20.0f, 15.0f, 20.0f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	this->m_lightViewMatrix = XMMatrixLookAtRH(eye, at, up);

	this->m_shadowTranformMatrix = XMMatrixIdentity();

	// Update data
	this->m_shadowCBuffer.m_data.lightViewMatrix = this->m_lightViewMatrix;
	this->m_shadowCBuffer.m_data.lightProjectionMatrix = this->m_lightProjectionMatrix;
	this->m_shadowCBuffer.m_data.textureTransformMatrix = this->m_shadowTranformMatrix;
	this->m_shadowCBuffer.m_data.lightPosition = eye;
	this->m_shadowCBuffer.upd();
}

void ShadowMapInstance::update()
{
}

ID3D11ShaderResourceView* const* ShadowMapInstance::getShadowMapSRV()
{
	return this->m_shadowMapSRV.GetAddressOf();
}

void ShadowMapInstance::bindViewsAndRenderTarget()
{
	this->m_deviceContext->ClearDepthStencilView(this->m_shadowMapDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.f);

	this->m_deviceContext->OMSetRenderTargets(0, nullptr, this->m_shadowMapDSV.Get());
	this->m_deviceContext->RSSetState(this->m_rasterizerState.Get());
	this->m_deviceContext->RSSetViewports(1, &this->m_viewport);

	this->m_deviceContext->OMSetDepthStencilState(this->m_depthStencilState.Get(), 0);
	this->m_deviceContext->VSSetConstantBuffers(1, 1, this->m_shadowCBuffer.GetAddressOf());
	this->m_deviceContext->PSSetSamplers(1, 1, this->m_comparisonSampler.GetAddressOf());

	this->m_shadowMapShaders.setShaders();
}

void ShadowMapInstance::setLightMatrixConstantBuffer()
{
	// Should be in slot 1, slot 0 is for gameobject matrices
	this->m_deviceContext->VSSetConstantBuffers(1, 1, this->m_shadowCBuffer.GetAddressOf());
}
