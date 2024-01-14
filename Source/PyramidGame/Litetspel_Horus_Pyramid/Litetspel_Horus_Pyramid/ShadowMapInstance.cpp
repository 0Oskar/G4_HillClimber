#include "pch.h"
#include "ShadowMapInstance.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

void ShadowMapInstance::initializeDepthTexture(ID3D11Device* device, ShadowTextureData& data)
{
	// Texture 2D
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = (UINT)data.textureDimensions.x;
	textureDesc.Height = (UINT)data.textureDimensions.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
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
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthMap, &depthStencilViewDesc, data.depthStencilView.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil view!");

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(depthMap, &shaderResourceViewDesc, data.shaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map shader resource view!");

	depthMap->Release();
}

void ShadowMapInstance::initializeRendertargetTexture(ID3D11Device* device, ShadowTextureData& data)
{
	// Texture 2D
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = (UINT)data.textureDimensions.x;
	textureDesc.Height = (UINT)data.textureDimensions.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* translucentShadowMap;
	ZeroMemory(&translucentShadowMap, sizeof(translucentShadowMap));
	HRESULT hr = device->CreateTexture2D(&textureDesc, 0, &translucentShadowMap);
	assert(SUCCEEDED(hr) && "Error, failed to create translucent shadow map texture!");

	// Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(translucentShadowMap, &renderTargetViewDesc, &data.renderTargetView);
	assert(SUCCEEDED(hr) && "Error, failed to create translucent shadow map render target view!");

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(translucentShadowMap, &shaderResourceViewDesc, data.shaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create translucent shadow map shader resource view!");

	translucentShadowMap->Release();
}

ShadowMapInstance::ShadowMapInstance()
{
	m_deviceContext = nullptr;

	for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; i++)
	{
		m_viewport[i] = D3D11_VIEWPORT();
	}
}

ShadowMapInstance::~ShadowMapInstance() {}

BoundingOrientedBox ShadowMapInstance::getLightBoundingBox(uint32_t index) const
{
	if (index == UINT_MAX)
		return m_singleMapData.obb;

	assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
	return m_cascadeMapsData[index].obb;
}

void ShadowMapInstance::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, SingleShadowDesc singleMapDesc, CascadingShadowDesc cascadingMapDesc, bool cascadedShadowMapsEnabled, bool translucent)
{
	// Device
	m_deviceContext = deviceContext;

	// Shadow Data
	m_cascadedShadowMapsEnabled = cascadedShadowMapsEnabled;
	m_translucentShadowMapsToggle = translucent;

	ShadowTextureData* shadowTextureData = nullptr;

	if (!m_cascadedShadowMapsEnabled)
	{
		// Texture
		m_singleMapTextureData.textureDimensions = XMFLOAT2((float)singleMapDesc.textureSize, (float)singleMapDesc.textureSize);
		shadowTextureData = &m_singleMapTextureData;
		
		// World Bounding Sphere
		m_worldBoundingSphere.Center = { 0.f, 0.f, 0.f };
		m_worldBoundingSphere.Radius = singleMapDesc.radius;

		// Viewport
		m_viewport[0].TopLeftX = 0.f;
		m_viewport[0].TopLeftY = 0.f;
		m_viewport[0].Width = m_singleMapTextureData.textureDimensions.x;
		m_viewport[0].Height = m_singleMapTextureData.textureDimensions.y;
		m_viewport[0].MinDepth = 0.f;
		m_viewport[0].MaxDepth = 1.f;
	}
	else
	{
		// Texture
		m_cascadeMapsTextureData.textureDimensions = XMFLOAT2((float)cascadingMapDesc.textureSize * SHADOW_CASCADE_COUNT, (float)cascadingMapDesc.textureSize);
		shadowTextureData = &m_cascadeMapsTextureData;

		float textureWidth = m_cascadeMapsTextureData.textureDimensions.x;
		float cascadeWidth = textureWidth / SHADOW_CASCADE_COUNT;
		for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; i++)
		{
			// Frustum Coverage
			m_cascadeMapsData[i].frustumCoveragePercentage = cascadingMapDesc.frustumCoveragePercentage[i];

			// Viewport
			m_viewport[i].TopLeftX = i * cascadeWidth;
			m_viewport[i].TopLeftY = 0.f;
			m_viewport[i].Width = cascadeWidth;
			m_viewport[i].Height = m_cascadeMapsTextureData.textureDimensions.y;
			m_viewport[i].MinDepth = 0.f;
			m_viewport[i].MaxDepth = 1.f;
		}
	}

	if (m_translucentShadowMapsToggle) {
		m_translucentTextureData.textureDimensions = shadowTextureData->textureDimensions;
		initializeRendertargetTexture(device, m_translucentTextureData);
	}
	
	initializeDepthTexture(device, *shadowTextureData);
	

	// Pipeline States
	
	// Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
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

	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil state!");
	
	// Rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 4.f;
	rasterizerDesc.SlopeScaledDepthBias = 0.f;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = m_translucentShadowMapsToggle ? D3D11_CULL_NONE : D3D11_CULL_BACK;
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

	// Shader
	ShaderFiles shaderFiles;
	shaderFiles.vs = L"Shader Files\\ShadowVS.hlsl";
	if (m_translucentShadowMapsToggle)
		shaderFiles.ps = L"Shader Files\\TranslucentShadowPS.hlsl";
	
	m_shader.initialize(device, deviceContext, shaderFiles);

	// Constant Buffer
	for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; i++)
	{
		m_lightMatrixCBuffer[i].init(device, deviceContext);
	}
}

ShadowFrustumData* ShadowMapInstance::getShadowData(uint32_t index)
{
	if (index == UINT_MAX)
		return &m_singleMapData;

	assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
	return &m_cascadeMapsData[index];
}
XMFLOAT2 ShadowMapInstance::getShadowTextureSize()
{
	if (m_cascadedShadowMapsEnabled)
		return m_cascadeMapsTextureData.textureDimensions;
	else
		return m_singleMapTextureData.textureDimensions;
}

//--------------------------------------------------------------------------------------
// This function takes the camera's projection matrix and returns the 8
// points that make up a view frustum.
// The frustum is scaled to fit within the Begin and End interval paramaters.
//--------------------------------------------------------------------------------------
void CreateFrustumPointsFromCascadeInterval(float fCascadeIntervalBegin,
	FLOAT fCascadeIntervalEnd,
	CXMMATRIX vProjection,
	XMVECTOR* pvCornerPointsWorld)
{

	BoundingFrustum vViewFrust(vProjection);
	vViewFrust.Near = fCascadeIntervalBegin;
	vViewFrust.Far = fCascadeIntervalEnd;

	static const XMVECTORU32 vGrabY = { 0x00000000,0xFFFFFFFF,0x00000000,0x00000000 };
	static const XMVECTORU32 vGrabX = { 0xFFFFFFFF,0x00000000,0x00000000,0x00000000 };

	XMVECTORF32 vRightTop = { vViewFrust.RightSlope,vViewFrust.TopSlope,1.0f,1.0f };
	XMVECTORF32 vLeftBottom = { vViewFrust.LeftSlope,vViewFrust.BottomSlope,1.0f,1.0f };
	XMVECTORF32 vNear = { vViewFrust.Near,vViewFrust.Near,vViewFrust.Near,1.0f };
	XMVECTORF32 vFar = { vViewFrust.Far,vViewFrust.Far,vViewFrust.Far,1.0f };
	XMVECTOR vRightTopNear = XMVectorMultiply(vRightTop, vNear);
	XMVECTOR vRightTopFar = XMVectorMultiply(vRightTop, vFar);
	XMVECTOR vLeftBottomNear = XMVectorMultiply(vLeftBottom, vNear);
	XMVECTOR vLeftBottomFar = XMVectorMultiply(vLeftBottom, vFar);

	pvCornerPointsWorld[0] = vRightTopNear;
	pvCornerPointsWorld[1] = XMVectorSelect(vRightTopNear, vLeftBottomNear, vGrabX);
	pvCornerPointsWorld[2] = vLeftBottomNear;
	pvCornerPointsWorld[3] = XMVectorSelect(vRightTopNear, vLeftBottomNear, vGrabY);

	pvCornerPointsWorld[4] = vRightTopFar;
	pvCornerPointsWorld[5] = XMVectorSelect(vRightTopFar, vLeftBottomFar, vGrabX);
	pvCornerPointsWorld[6] = vLeftBottomFar;
	pvCornerPointsWorld[7] = XMVectorSelect(vRightTopFar, vLeftBottomFar, vGrabY);

}

void ShadowMapInstance::buildLightMatrix(XMFLOAT3 lightDirection, XMFLOAT3 position)
{
	// Light View Matrix
	m_worldBoundingSphere.Center = position;
	XMVECTOR lightDirectionVec = XMLoadFloat3(&lightDirection);
	XMVECTOR lookAt = XMLoadFloat3(&position);
	lookAt = XMVectorSetW(lookAt, 1.f);
	XMVECTOR lightPosition = XMVectorAdd(XMVectorScale(lightDirectionVec, m_worldBoundingSphere.Radius), lookAt);

	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	XMVECTOR eyeDirection = XMVectorSubtract(lookAt, lightPosition);
	if (XMVector3Equal(eyeDirection, XMVectorZero()))
	{
		lookAt = XMVectorAdd(lookAt, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	}
	m_singleMapData.matrixData.viewMatrix = XMMatrixLookAtLH(lightPosition, lookAt, up);

	// Transform World Bounding Sphere to Light Local View Space
	XMFLOAT3 worldSphereCenterLightSpace;
	XMStoreFloat3(&worldSphereCenterLightSpace, XMVector3TransformCoord(lookAt, m_singleMapData.matrixData.viewMatrix));

	// Construct Orthographic Frustum in Light View Space
	float l = worldSphereCenterLightSpace.x - m_worldBoundingSphere.Radius;
	float b = worldSphereCenterLightSpace.y - m_worldBoundingSphere.Radius;
	//float n = worldSphereCenterLightSpace.z - m_worldBoundingSphere.Radius;
	float n = 0.f;
	float r = worldSphereCenterLightSpace.x + m_worldBoundingSphere.Radius;
	float t = worldSphereCenterLightSpace.y + m_worldBoundingSphere.Radius;
	//float f = worldSphereCenterLightSpace.z + m_worldBoundingSphere.Radius;
	float f = m_worldBoundingSphere.Radius * SHADOW_DEPTH_EXTENSION;

	// Local Projection Matrix
	m_singleMapData.matrixData.projMatrix = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Bounding Box
	float distanceMultiplier = 1.1f;
	m_singleMapData.obb.Center = m_worldBoundingSphere.Center;
	float distance = m_worldBoundingSphere.Radius * distanceMultiplier;
	m_singleMapData.obb.Extents = XMFLOAT3(distance * SHADOW_DEPTH_EXTENSION, distance, distance);

	XMFLOAT3 directionF3 = pMath::F3Multiply(lightDirection, m_worldBoundingSphere.Radius);
	XMFLOAT3 lookAtF3 = pMath::F3Add(m_singleMapData.obb.Center, directionF3);
	lookAt = XMLoadFloat3(&lookAtF3);

	if (DirectX::XMVector3Equal(lookAt, (XMVECTOR)g_XMZero))
		lookAt = XMVectorAdd(lookAt, XMVectorSet(0.f, 0.f, 1.f, 0.f));

	XMFLOAT3 OBBpositionF3 = pMath::F3Subtract(m_singleMapData.obb.Center, directionF3);
	XMVECTOR OBBposition = XMLoadFloat3(&OBBpositionF3);

	XMMATRIX rotationMatrix = XMMatrixLookAtLH(OBBposition, lookAt, up);
	XMStoreFloat4(&m_singleMapData.obb.Orientation, XMQuaternionRotationMatrix(rotationMatrix));
}

void ShadowMapInstance::buildCascadeLightMatrix(uint32_t index, XMFLOAT3 lightDirection, Camera* camera)
{
	assert(index < SHADOW_CASCADE_COUNT && "Shadow Map Cascade index out of bounds!");

	XMMATRIX cameraProjectionMatrix = camera->getProjectionMatrix();
	XMMATRIX cameraViewMatrix = camera->getViewMatrix();

	XMVECTOR lightPosition = (XMLoadFloat3(&lightDirection)) * 1000.f;
	if (DirectX::XMVector3Equal(lightPosition, (XMVECTOR)g_XMZero))
		lightPosition = DirectX::XMVectorSet(0.f, 1000.f, 0.f, 1.f);
	
	XMMATRIX lightViewMatrix = XMMatrixLookAtLH(lightPosition, g_XMZero, g_XMIdentityR1);

	XMMATRIX inverseViewCameraMatrix = XMMatrixInverse(nullptr, cameraViewMatrix);

	float sceneRange = (m_cascadeMapsData[SHADOW_CASCADE_COUNT - 1].frustumCoveragePercentage / 100.f) * camera->getFarZ();
	XMVECTOR sceneAABBMin = { sceneRange, sceneRange, sceneRange, sceneRange };
	XMVECTOR sceneAABBMax = { -sceneRange, -sceneRange, -sceneRange, -sceneRange };

	BoundingBox bb;
	BoundingBox::CreateFromPoints(bb, sceneAABBMin, sceneAABBMax);

	XMFLOAT3 tmp[8];
	bb.GetCorners(tmp);

	// Transform the scene AABB to Light space.
	XMVECTOR sceneAABBPointsLightSpace[8];
	for (int i = 0; i < 8; ++i)
	{
		XMVECTOR v = XMLoadFloat3(&tmp[i]);
		sceneAABBPointsLightSpace[i] = XMVector3Transform(v, lightViewMatrix);
	}

	float frustumIntervalBegin, frustumIntervalEnd;
	XMVECTOR lightCameraOrthographicMin;  // light space frustrum aabb 
	XMVECTOR lightCameraOrthographicMax;
	float fCameraNearFarRange = camera->getFarZ() - camera->getNearZ();

	XMVECTOR worldUnitsPerTexel = g_XMZero;

	float textureOffsetX = -SHADOW_CASCADE_OFFSET;

	// We loop over the cascades to calculate the orthographic projection for each cascade.
	for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; ++i)
	{
		// In the FIT_TO_SCENE technique the Cascades overlap eachother.  In other words, interval 1 is coverd by
		// cascades 1 to 8, interval 2 is covered by cascades 2 to 8 and so forth.
		frustumIntervalBegin = 0.0f;

		// Scale the intervals between 0 and 1. They are now percentages that we can scale with.
		frustumIntervalEnd = m_cascadeMapsData[i].frustumCoveragePercentage;
		frustumIntervalBegin /= 100.f;
		frustumIntervalEnd /= 100.f;
		frustumIntervalBegin = frustumIntervalBegin * fCameraNearFarRange;
		frustumIntervalEnd = frustumIntervalEnd * fCameraNearFarRange;
		XMVECTOR vFrustumPoints[8];

		// This function takes the began and end intervals along with the projection matrix and returns the 8
		// points that repreresent the cascade Interval
		CreateFrustumPointsFromCascadeInterval(frustumIntervalBegin, frustumIntervalEnd,
			cameraProjectionMatrix, vFrustumPoints);

		XMVECTOR fltMaxVector = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };
		XMVECTOR fltMinVector = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };

		lightCameraOrthographicMin = fltMaxVector;
		lightCameraOrthographicMax = fltMinVector;

		XMVECTOR vTempTranslatedCornerPoint;
		// This next section of code calculates the min and max values for the orthographic projection.
		for (int icpIndex = 0; icpIndex < 8; ++icpIndex)
		{
			// Transform the frustum from camera view space to world space.
			vFrustumPoints[icpIndex] = XMVector4Transform(vFrustumPoints[icpIndex], inverseViewCameraMatrix);
			// Transform the point from world space to Light Camera Space.
			vTempTranslatedCornerPoint = XMVector4Transform(vFrustumPoints[icpIndex], lightViewMatrix);
			// Find the closest point.
			lightCameraOrthographicMin = XMVectorMin(vTempTranslatedCornerPoint, lightCameraOrthographicMin);
			lightCameraOrthographicMax = XMVectorMax(vTempTranslatedCornerPoint, lightCameraOrthographicMax);
		}

		// Fit the ortho projection to the cascades far plane and a near plane of zero. 
		// Pad the projection to be the size of the diagonal of the Frustum partition. 
		// 
		// To do this, we pad the ortho transform so that it is always big enough to cover 
		// the entire camera view frustum.
		XMVECTOR vDiagonal = vFrustumPoints[0] - vFrustumPoints[6];
		vDiagonal = XMVector3Length(vDiagonal);

		// The bound is the length of the diagonal of the frustum interval.
		FLOAT fCascadeBound = XMVectorGetX(vDiagonal);

		// The offset calculated will pad the ortho projection so that it is always the same size 
		// and big enough to cover the entire cascade interval.
		XMVECTOR halfVector = { 0.5f, 0.5f, 0.5f, 0.5f };
		XMVECTOR multiplySetZWToZero = { 1.0f, 1.0f, 0.0f, 0.0f };
		XMVECTOR vBoarderOffset = (vDiagonal - (lightCameraOrthographicMax - lightCameraOrthographicMin)) * halfVector;
		// Set the Z and W components to zero.
		vBoarderOffset *= multiplySetZWToZero;

		// Add the offsets to the projection.
		lightCameraOrthographicMax += vBoarderOffset;
		lightCameraOrthographicMin -= vBoarderOffset;

		// The world units per texel are used to snap the shadow the orthographic projection
		// to texel sized increments.  This keeps the edges of the shadows from shimmering.
		FLOAT fWorldUnitsPerTexel = fCascadeBound / (float)m_cascadeMapsTextureData.textureDimensions.y;
		worldUnitsPerTexel = XMVectorSet(fWorldUnitsPerTexel, fWorldUnitsPerTexel, 0.0f, 0.0f);

		float fLightCameraOrthographicMinZ = XMVectorGetZ(lightCameraOrthographicMin);

		// We snape the camera to 1 pixel increments so that moving the camera does not cause the shadows to jitter.
		// This is a matter of integer dividing by the world space size of a texel
		lightCameraOrthographicMin /= worldUnitsPerTexel;
		lightCameraOrthographicMin = XMVectorFloor(lightCameraOrthographicMin);
		lightCameraOrthographicMin *= worldUnitsPerTexel;

		lightCameraOrthographicMax /= worldUnitsPerTexel;
		lightCameraOrthographicMax = XMVectorFloor(lightCameraOrthographicMax);
		lightCameraOrthographicMax *= worldUnitsPerTexel;
		
		//These are the unconfigured near and far plane values.  They are purposly awful to show 
		// how important calculating accurate near and far planes is.
		FLOAT fNearPlane = 0.0f;
		FLOAT fFarPlane = 10000.0f;

		XMVECTOR lightSpaceSceneAABBminValue = fltMaxVector;  // world space scene aabb 
		XMVECTOR lightSpaceSceneAABBmaxValue = fltMinVector;
		// We calculate the min and max vectors of the scene in light space. The min and max "Z" values of the  
		// light space AABB can be used for the near and far plane. This is easier than intersecting the scene with the AABB
		// and in some cases provides similar results.
		for (int i = 0; i < 8; ++i)
		{
			lightSpaceSceneAABBminValue = XMVectorMin(sceneAABBPointsLightSpace[i], lightSpaceSceneAABBminValue);
			lightSpaceSceneAABBmaxValue = XMVectorMax(sceneAABBPointsLightSpace[i], lightSpaceSceneAABBmaxValue);
		}

		// The min and max z values are the near and far planes.
		fNearPlane = XMVectorGetZ(lightSpaceSceneAABBminValue);
		fFarPlane = XMVectorGetZ(lightSpaceSceneAABBmaxValue);
		
		// Create the orthographic projection for this cascade.
		m_cascadeMapsData[i].matrixData.projMatrix = XMMatrixOrthographicOffCenterLH(
			XMVectorGetX(lightCameraOrthographicMin), XMVectorGetX(lightCameraOrthographicMax),
			XMVectorGetY(lightCameraOrthographicMin), XMVectorGetY(lightCameraOrthographicMax),
			fNearPlane, fFarPlane);

		m_cascadeMapsData[i].matrixData.viewMatrix = lightViewMatrix;
		m_cascadeMapsData[i].matrixData.textureOffset = XMFLOAT2(textureOffsetX, 0.f);
		textureOffsetX += SHADOW_CASCADE_OFFSET;
		m_cascadeMapsData[i].frustumCoverage = frustumIntervalEnd;
	}
}

void ShadowMapInstance::reloadShaders() {
	m_shader.reloadShaders();
}

ID3D11ShaderResourceView* ShadowMapInstance::getShadowMapSRV()
{
	if (m_cascadedShadowMapsEnabled)
		return m_cascadeMapsTextureData.shaderResourceView.Get();
	else
		return m_singleMapTextureData.shaderResourceView.Get();
}

ID3D11ShaderResourceView* ShadowMapInstance::getTranslucentShadowMapSRV()
{
	if (m_translucentShadowMapsToggle)
		return m_translucentTextureData.shaderResourceView.Get();
	return nullptr;
}

ID3D11ShaderResourceView* const* ShadowMapInstance::getShadowMapSRVConstPtr()
{
	if (m_cascadedShadowMapsEnabled)
		return m_cascadeMapsTextureData.shaderResourceView.GetAddressOf();
	else
		return m_singleMapTextureData.shaderResourceView.GetAddressOf();
}

ID3D11ShaderResourceView* const* ShadowMapInstance::getTranslucentShadowMapSRVConstPtr()
{
	if (m_translucentShadowMapsToggle)
		return m_translucentTextureData.shaderResourceView.GetAddressOf();
	return nullptr;
}

ID3D11DepthStencilView* ShadowMapInstance::getShadowMapDSV()
{
	if (m_cascadedShadowMapsEnabled)
		return m_cascadeMapsTextureData.depthStencilView.Get();
	else
		return m_singleMapTextureData.depthStencilView.Get();

}

ID3D11RenderTargetView* const* ShadowMapInstance::getTranslucentShadowMapRTVConstPtr()
{
	if (m_translucentShadowMapsToggle)
		return m_translucentTextureData.renderTargetView.GetAddressOf();
	return nullptr;
}

void ShadowMapInstance::clearShadowmap()
{
	float clearColor[] = { 0.f, 0.f, 0.f, 1.f };
	if (m_cascadedShadowMapsEnabled)
	{
		if (m_translucentShadowMapsToggle)
			m_deviceContext->ClearRenderTargetView(m_translucentTextureData.renderTargetView.Get(), clearColor);
		
		m_deviceContext->ClearDepthStencilView(m_cascadeMapsTextureData.depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	else
	{
		if (m_translucentShadowMapsToggle)
			m_deviceContext->ClearRenderTargetView(m_translucentTextureData.renderTargetView.Get(), clearColor);
		
		m_deviceContext->ClearDepthStencilView(m_singleMapTextureData.depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

void ShadowMapInstance::setComparisonSampler()
{
	m_deviceContext->PSSetSamplers(1, 1, m_comparisonSampler.GetAddressOf());
}

void ShadowMapInstance::bindLightMatrixBufferVS(uint32_t index, uint32_t slot)
{
	if (index == UINT_MAX)
	{
		m_lightMatrixCBuffer[0].upd(&m_singleMapData.matrixData);
		m_deviceContext->VSSetConstantBuffers(slot, 1, m_lightMatrixCBuffer[0].GetAddressOf());
	}
	else
	{
		assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
		m_lightMatrixCBuffer[index].upd(&m_cascadeMapsData[index].matrixData);
		m_deviceContext->VSSetConstantBuffers(slot, 1, m_lightMatrixCBuffer[index].GetAddressOf());
	}
}

void ShadowMapInstance::bindStatesAndShader(uint32_t index)
{
	uint32_t shadowIndex = index;
	// Set to 0 if Cascaded Shadow Maps is not enabled
	shadowIndex *= (uint32_t)m_cascadedShadowMapsEnabled;

	m_deviceContext->RSSetViewports(1, &m_viewport[shadowIndex]);

	m_shader.setShaders();
	m_deviceContext->RSSetState(m_rasterizerState.Get());
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
}