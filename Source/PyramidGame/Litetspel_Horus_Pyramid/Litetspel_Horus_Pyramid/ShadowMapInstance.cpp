#include "pch.h"
#include "ShadowMapInstance.h"

ShadowMapInstance::ShadowMapInstance()
{
	m_deviceContext = nullptr;
	m_width = 0;
	m_height = 0;
}

ShadowMapInstance::~ShadowMapInstance() {}

void ShadowMapInstance::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
{
	// Device
	m_deviceContext = deviceContext;

	// Dimensions
	m_width = width;
	m_height = height;

	// Viewport
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = (float)m_width;
	m_viewport.Height = (float)m_height;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;

	// Resources
	// Texture 2D
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
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
	hr = device->CreateDepthStencilView(depthMap, &depthStencilViewDesc, m_shadowMapDSV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil view!");

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(depthMap, &shaderResourceViewDesc, m_shadowMapSRV.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map shader resource view!");

	depthMap->Release();

	// Pipeline States
	
	// Depth Stencil State
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

	hr = device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil state!");
	 
	// Rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.DepthBias = 1000;
	rasterizerDesc.DepthBiasClamp = 0.f;
	rasterizerDesc.SlopeScaledDepthBias = 0.f;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map rasterizer state!");

	// Sampler
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.f;
	comparisonSamplerDesc.BorderColor[1] = 1.f;
	comparisonSamplerDesc.BorderColor[2] = 1.f;
	comparisonSamplerDesc.BorderColor[3] = 1.f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;

	hr = device->CreateSamplerState(&comparisonSamplerDesc, m_comparisonSampler.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error when creating shadow map sampler state!");

	// World Bounding Sphere
	m_worldBoundingSphere.Center = { 0.f, 0.f, 0.f };
	m_worldBoundingSphere.Radius = 100.f;

	// Shader
	ShaderFiles shaderFiles;
	shaderFiles.vs = L"Shader Files\\ShadowVS.hlsl";
	m_shader.initialize(device, deviceContext, shaderFiles);

	// Constant Buffer
	m_lightMatrixCBuffer.init(device, deviceContext);
}

void ShadowMapInstance::buildLightMatrix(PS_DIR_BUFFER directionalLight, XMFLOAT3 position)
{
	// Light View Matrix
	VS_DIRECTIONAL_CBUFFER lightMatrices;
	XMVECTOR lightDirection = XMLoadFloat4(&directionalLight.lightDirection);
	XMVECTOR lookAt = XMLoadFloat3(&position);
	lookAt = DirectX::XMVectorSetW(lookAt, 1.f);
	XMVECTOR lightPosition = DirectX::XMVectorAdd(DirectX::XMVectorScale(lightDirection, m_worldBoundingSphere.Radius), lookAt);

	XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	XMVECTOR eyeDirection = DirectX::XMVectorSubtract(lookAt, lightPosition);
	if (DirectX::XMVector3Equal(eyeDirection, DirectX::XMVectorZero()))
	{
		lookAt = DirectX::XMVectorAdd(lookAt, DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f));
	}
	lightMatrices.lightViewMatrix = DirectX::XMMatrixLookAtLH(lightPosition, lookAt, up);

	// Transform World Bounding Sphere to Light Local View Space
	XMFLOAT3 worldSphereCenterLightSpace;
	XMStoreFloat3(&worldSphereCenterLightSpace, DirectX::XMVector3TransformCoord(lookAt, lightMatrices.lightViewMatrix));

	// Construct Orthographic Frustum in Light View Space
	float l = worldSphereCenterLightSpace.x - m_worldBoundingSphere.Radius;
	float b = worldSphereCenterLightSpace.y - m_worldBoundingSphere.Radius;
	//float n = worldSphereCenterLightSpace.z - m_worldBoundingSphere.Radius;
	float n = 0.f;
	float r = worldSphereCenterLightSpace.x + m_worldBoundingSphere.Radius;
	float t = worldSphereCenterLightSpace.y + m_worldBoundingSphere.Radius;
	//float f = worldSphereCenterLightSpace.z + m_worldBoundingSphere.Radius;
	float f = m_worldBoundingSphere.Radius * 6.f;

	// Local Projection Matrix
	lightMatrices.lightProjMatrix = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Update data
	m_lightMatrixCBuffer.upd(&lightMatrices);
}

void ShadowMapInstance::update()
{

}

ID3D11ShaderResourceView* ShadowMapInstance::getShadowMapSRV()
{
	return m_shadowMapSRV.Get();
}

ID3D11ShaderResourceView* const* ShadowMapInstance::getShadowMapSRVConstPtr()
{
	return m_shadowMapSRV.GetAddressOf();
}

void ShadowMapInstance::bindViewsAndRenderTarget()
{
	ID3D11ShaderResourceView* shaderResourceNullptr = nullptr;
	m_deviceContext->PSSetShaderResources(3, 1, &shaderResourceNullptr);

	m_deviceContext->OMSetRenderTargets(1, m_rendertarget, m_shadowMapDSV.Get());
	m_deviceContext->ClearDepthStencilView(m_shadowMapDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_deviceContext->RSSetState(m_rasterizerState.Get());
	m_deviceContext->RSSetViewports(1, &m_viewport);

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
	m_deviceContext->VSSetConstantBuffers(1, 1, m_lightMatrixCBuffer.GetAdressOf()); // GetAddressOf XD
	m_deviceContext->PSSetSamplers(1, 1, m_comparisonSampler.GetAddressOf());

	m_shader.setShaders();
}
