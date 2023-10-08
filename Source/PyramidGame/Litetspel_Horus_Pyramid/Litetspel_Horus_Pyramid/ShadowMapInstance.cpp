#include "pch.h"
#include "ShadowMapInstance.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

void ShadowMapInstance::initializeTexture(ID3D11Device* device, ShadowData& data)
{
	// Texture 2D
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = data.textureSize;
	textureDesc.Height = data.textureSize;
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
	hr = device->CreateDepthStencilView(depthMap, &depthStencilViewDesc, data.depthStencilView.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map depth stencil view!");

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(depthMap, &shaderResourceViewDesc, data.shaderResourceView.GetAddressOf());
	assert(SUCCEEDED(hr) && "Error, failed to create shadow map shader resource view!");

	depthMap->Release();
}

ShadowMapInstance::ShadowMapInstance()
{
	m_deviceContext = nullptr;
}

ShadowMapInstance::~ShadowMapInstance() {}

BoundingOrientedBox ShadowMapInstance::getLightBoundingBox(uint32_t index) const
{
	if (index == UINT_MAX)
		return m_singleMapData.obb;

	assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
	return m_cascadeMapsData[index].obb;
}

std::vector<XMVECTOR> getFrustumCornersWorldSpace(const XMMATRIX& proj, const XMMATRIX& view)
{
	const XMMATRIX inv = XMMatrixInverse(nullptr, proj * view);

	std::vector<XMVECTOR> frustumCorners;
	frustumCorners.reserve(2 * 2 * 2);

	for (uint32_t x = 0; x < 2; ++x)
	{
		for (uint32_t y = 0; y < 2; ++y)
		{
			for (uint32_t z = 0; z < 2; ++z)
			{
				const XMVECTOR pt =
					XMVector4Transform(
						XMVectorSet(
							2.f * x - 1.f,
							2.f * y - 1.f,
							2.f * z - 1.f,
							1.f),
						inv);
				
				frustumCorners.push_back(pt / XMVectorGetW(pt));
			}
		}
	}

	return frustumCorners;
}

void ShadowMapInstance::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, uint32_t singleMapSize, ShadowDesc cascadingMapDesc[SHADOW_CASCADE_COUNT])
{
	// Device
	m_deviceContext = deviceContext;

	// Shadow Data
	m_singleMapData.textureSize = singleMapSize;
	initializeTexture(device, m_singleMapData);

	for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; i++)
	{
		m_cascadeMapsData[i].textureSize = cascadingMapDesc[i].textureSize;
		m_cascadeMapsData[i].frustumCoveragePercentage = cascadingMapDesc[i].frustumCoveragePercentage;
		initializeTexture(device, m_cascadeMapsData[i]);
	}

	// Viewport
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;

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

	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);
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
	for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; i++)
	{
		m_lightMatrixCBuffer[i].init(device, deviceContext);
	}
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
	m_lightDirection = lightDirection;
	m_worldBoundingSphere.Center = position;
	XMVECTOR lightDirectionVec = XMLoadFloat3(&m_lightDirection);
	XMVECTOR lookAt = XMLoadFloat3(&position);
	lookAt = XMVectorSetW(lookAt, 1.f);
	XMVECTOR lightPosition = XMVectorAdd(XMVectorScale(lightDirectionVec, m_worldBoundingSphere.Radius), lookAt);

	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	XMVECTOR eyeDirection = XMVectorSubtract(lookAt, lightPosition);
	if (XMVector3Equal(eyeDirection, XMVectorZero()))
	{
		lookAt = XMVectorAdd(lookAt, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	}
	m_singleMapData.matrices.viewMatrix = XMMatrixLookAtLH(lightPosition, lookAt, up);

	// Transform World Bounding Sphere to Light Local View Space
	XMFLOAT3 worldSphereCenterLightSpace;
	XMStoreFloat3(&worldSphereCenterLightSpace, XMVector3TransformCoord(lookAt, m_singleMapData.matrices.viewMatrix));

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
	m_singleMapData.matrices.projMatrix = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Bounding Box
	float distanceMultiplier = 1.1f;
	m_singleMapData.obb.Center = m_worldBoundingSphere.Center;
	float distance = m_worldBoundingSphere.Radius * distanceMultiplier;
	m_singleMapData.obb.Extents = XMFLOAT3(distance * SHADOW_DEPTH_EXTENSION, distance, distance);

	XMFLOAT3 directionF3 = pMath::F3Multiply(m_lightDirection, m_worldBoundingSphere.Radius);
	XMFLOAT3 lookAtF3 = pMath::F3Add(m_singleMapData.obb.Center, directionF3);
	lookAt = XMLoadFloat3(&lookAtF3);

	XMFLOAT3 OBBpositionF3 = pMath::F3Subtract(m_singleMapData.obb.Center, directionF3);
	XMVECTOR OBBposition = XMLoadFloat3(&OBBpositionF3);

	XMMATRIX rotationMatrix = XMMatrixLookAtLH(OBBposition, lookAt, up);
	XMStoreFloat4(&m_singleMapData.obb.Orientation, XMQuaternionRotationMatrix(rotationMatrix));
}

ShadowData* ShadowMapInstance::getShadowData(uint32_t index)
{
	if (index == UINT_MAX)
		return &m_singleMapData;

	assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
	return &m_cascadeMapsData[index];
}

void ShadowMapInstance::buildCascadeLightMatrix(uint32_t index, XMFLOAT3 lightDirection, Camera* camera)
{
	assert(index < SHADOW_CASCADE_COUNT && "Shadow Map Cascade index out of bounds!");

	XMMATRIX cameraProjectionMatrix = camera->getProjectionMatrix();
	XMMATRIX cameraViewMatrix = camera->getViewMatrix();

	XMVECTOR lightPosition = (XMLoadFloat3(&lightDirection)) * 1000.f;
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

	// We loop over the cascades to calculate the orthographic projection for each cascade.
	for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; ++i)
	{
		// Calculate the interval of the View Frustum that this cascade covers. We measure the interval 
		// the cascade covers as a Min and Max distance along the Z Axis.
		/*if (m_eSelectedCascadesFit == FIT_TO_CASCADES)
		{*/
			// Because we want to fit the orthogrpahic projection tightly around the Cascade, we set the Mimiumum cascade 
			// value to the previous Frustum end Interval
			/*if (i == 0)
				frustumIntervalBegin = 0.0f;
			else
				frustumIntervalBegin = m_cascadeMapsData[i - 1].frustumCoveragePercentage;*/
		//}
		//else
		//{
			// In the FIT_TO_SCENE technique the Cascades overlap eachother.  In other words, interval 1 is coverd by
			// cascades 1 to 8, interval 2 is covered by cascades 2 to 8 and so forth.
			frustumIntervalBegin = 0.0f;
		//}

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

		// This code removes the shimmering effect along the edges of shadows due to
		// the light changing to fit the camera.
		//if (m_eSelectedCascadesFit == FIT_TO_SCENE)
		//{
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
			FLOAT fWorldUnitsPerTexel = fCascadeBound / (float)m_cascadeMapsData[i].textureSize;
			worldUnitsPerTexel = XMVectorSet(fWorldUnitsPerTexel, fWorldUnitsPerTexel, 0.0f, 0.0f);


		//}
		//else if (m_eSelectedCascadesFit == FIT_TO_CASCADES)
		//{

			//// We calculate a looser bound based on the size of the PCF blur.  This ensures us that we're 
			//// sampling within the correct map.
			//float m_iPCFBlurSize = 1.f;
			//float fScaleDuetoBlureAMT = ((m_iPCFBlurSize * 2 + 1)
			//	/ m_cascadeMapsData[i].textureSize);
			//XMVECTORF32 vScaleDuetoBlureAMT = { fScaleDuetoBlureAMT, fScaleDuetoBlureAMT, 0.0f, 0.0f };


			//float fNormalizeByBufferSize = (1.0f / m_cascadeMapsData[i].textureSize);
			//XMVECTOR vNormalizeByBufferSize = XMVectorSet(fNormalizeByBufferSize, fNormalizeByBufferSize, 0.0f, 0.0f);
			//
			//// We calculate the offsets as a percentage of the bound.
			//XMVECTOR vBoarderOffset = lightCameraOrthographicMax - lightCameraOrthographicMin;
			//vBoarderOffset *= { 0.5f, 0.5f, 0.5f, 0.5f };
			//vBoarderOffset *= vScaleDuetoBlureAMT;
			//lightCameraOrthographicMax += vBoarderOffset;
			//lightCameraOrthographicMin -= vBoarderOffset;

			//// The world units per texel are used to snap  the orthographic projection
			//// to texel sized increments.  
			//// Because we're fitting tighly to the cascades, the shimmering shadow edges will still be present when the 
			//// camera rotates.  However, when zooming in or strafing the shadow edge will not shimmer.
			//worldUnitsPerTexel = lightCameraOrthographicMax - lightCameraOrthographicMin;
			//worldUnitsPerTexel *= vNormalizeByBufferSize;

		//}
		float fLightCameraOrthographicMinZ = XMVectorGetZ(lightCameraOrthographicMin);


		/*if (m_bMoveLightTexelSize)
		{*/
			// We snape the camera to 1 pixel increments so that moving the camera does not cause the shadows to jitter.
			// This is a matter of integer dividing by the world space size of a texel
			lightCameraOrthographicMin /= worldUnitsPerTexel;
			lightCameraOrthographicMin = XMVectorFloor(lightCameraOrthographicMin);
			lightCameraOrthographicMin *= worldUnitsPerTexel;

			lightCameraOrthographicMax /= worldUnitsPerTexel;
			lightCameraOrthographicMax = XMVectorFloor(lightCameraOrthographicMax);
			lightCameraOrthographicMax *= worldUnitsPerTexel;
		//}

		//These are the unconfigured near and far plane values.  They are purposly awful to show 
		// how important calculating accurate near and far planes is.
		FLOAT fNearPlane = 0.0f;
		FLOAT fFarPlane = 10000.0f;

		/*if (m_eSelectedNearFarFit == FIT_NEARFAR_AABB)
		{*/
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
		//}
		//else if (m_eSelectedNearFarFit == FIT_NEARFAR_SCENE_AABB || m_eSelectedNearFarFit == FIT_NEARFAR_PANCAKING)
		//{
		//	// By intersecting the light frustum with the scene AABB we can get a tighter bound on the near and far plane.
		//	ComputeNearAndFar(fNearPlane, fFarPlane, lightCameraOrthographicMin, lightCameraOrthographicMax, sceneAABBPointsLightSpace);
		//	if (m_eSelectedNearFarFit == FIT_NEARFAR_PANCAKING)
		//	{
		//		if (fLightCameraOrthographicMinZ > fNearPlane)
		//		{
		//			fNearPlane = fLightCameraOrthographicMinZ;
		//		}
		//	}
		//}
		// Create the orthographic projection for this cascade.
		m_cascadeMapsData[i].matrices.projMatrix = XMMatrixOrthographicOffCenterLH(
			XMVectorGetX(lightCameraOrthographicMin), XMVectorGetX(lightCameraOrthographicMax),
			XMVectorGetY(lightCameraOrthographicMin), XMVectorGetY(lightCameraOrthographicMax),
			fNearPlane, fFarPlane);
		m_cascadeMapsData[i].matrices.viewMatrix = lightViewMatrix;

		m_cascadeMapsData[i].frustumCoverage = frustumIntervalEnd;
	}

	/*
	const XMMATRIX proj = XMMatrixPerspectiveFovLH(camera->getFov(), camera->getAspectRatio(), m_cascadeMapsData[index].nearZ, m_cascadeMapsData[index].farZ);

	Vector3 frustomCorners[8] =
	{
		Vector3(-1.f, 1.f, 0.f),
		Vector3( 1.f, 1.f, 0.f),
		Vector3( 1.f,-1.f, 0.f),
		Vector3(-1.f,-1.f, 0.f),
		Vector3(-1.f, 1.f, 1.f),
		Vector3( 1.f, 1.f, 1.f),
		Vector3( 1.f,-1.f, 1.f),
		Vector3(-1.f,-1.f, 1.f)
	};
	
	XMMATRIX cameraViewProjMatrix = camera->getViewMatrix() * proj;

	XMMATRIX invViewProj = XMMatrixInverse(nullptr, cameraViewProjMatrix);
	Vector3 frustumCenter = Vector3::Zero;

	for (uint32_t i = 0; i < 8; i++)
	{
		frustomCorners[i] = pMath::Transform(frustomCorners[i], invViewProj);
		frustumCenter += frustomCorners[i];
	}
	frustumCenter *= (1.f / 8.f);

	float radius = (frustomCorners[0] - frustomCorners[6]).Length() / 2.f;
	float texelSize = (float)m_cascadeMapsData[index].textureSize / (radius * 2.f);
	XMMATRIX scalar = XMMatrixScaling(texelSize, texelSize, texelSize);
	
	XMVECTOR upDirection = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR baseLookAt = XMVectorNegate(XMLoadFloat3(&lightDirection));

	XMMATRIX lookAt = XMMatrixLookAtLH(Vector3::Zero, baseLookAt, upDirection) * scalar;
	XMMATRIX lookAtInv = XMMatrixInverse(nullptr, lookAt);

	frustumCenter = XMVector3Transform(frustumCenter, lookAt);
	frustumCenter.x = floor(frustumCenter.x);
	frustumCenter.y = floor(frustumCenter.y);
	frustumCenter = XMVector3Transform(frustumCenter, lookAtInv);

	XMFLOAT3 extends = XMFLOAT3(radius * SHADOW_DEPTH_EXTENSION, radius, radius);
	m_cascadeMapsData[index].obb.Center = frustumCenter;
	m_cascadeMapsData[index].obb.Extents = extends;

	Vector3 eye = frustumCenter - (lightDirection * radius * 2.f);
	m_cascadeMapsData[index].matrices.viewMatrix = XMMatrixLookAtLH(eye, frustumCenter, upDirection);
	m_cascadeMapsData[index].matrices.projMatrix = XMMatrixOrthographicOffCenterLH(-extends.x, extends.x, -extends.y, extends.y, -extends.z, extends.z);

	XMMATRIX rotationMatrix = m_cascadeMapsData[index].matrices.viewMatrix;
	XMStoreFloat4(&m_cascadeMapsData[index].obb.Orientation, XMQuaternionRotationMatrix(rotationMatrix));
	*/
}

void ShadowMapInstance::update()
{

}

void ShadowMapInstance::reloadShaders() {
	m_shader.reloadShaders();
}

ID3D11ShaderResourceView* ShadowMapInstance::getShadowMapSRV(uint32_t index)
{
	if (index == UINT_MAX)
		return m_singleMapData.shaderResourceView.Get();
		
	assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
	return m_cascadeMapsData[index].shaderResourceView.Get();
}

ID3D11ShaderResourceView* const* ShadowMapInstance::getShadowMapSRVConstPtr(uint32_t index)
{
	if (index == UINT_MAX)
		return m_singleMapData.shaderResourceView.GetAddressOf();
	
	assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
	return m_cascadeMapsData[index].shaderResourceView.GetAddressOf();
}

ID3D11DepthStencilView* ShadowMapInstance::getShadowMapDSV(uint32_t index)
{
	if (index == UINT_MAX)
		return m_singleMapData.depthStencilView.Get();

	assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
	return m_cascadeMapsData[index].depthStencilView.Get();
}

void ShadowMapInstance::clearShadowmap()
{
	if (cascadedShadowMapsToggle)
	{
		for (uint32_t i = 0; i < SHADOW_CASCADE_COUNT; i++)
		{
			m_deviceContext->ClearDepthStencilView(m_cascadeMapsData[i].depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
	else
		m_deviceContext->ClearDepthStencilView(m_singleMapData.depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ShadowMapInstance::setComparisonSampler()
{
	m_deviceContext->PSSetSamplers(1, 1, m_comparisonSampler.GetAddressOf());
}

void ShadowMapInstance::bindLightMatrixBufferVS(uint32_t index, uint32_t slot)
{
	if (index == UINT_MAX)
	{
		m_lightMatrixCBuffer[0].upd(&m_singleMapData.matrices);
		m_deviceContext->VSSetConstantBuffers(slot, 1, m_lightMatrixCBuffer[0].GetAddressOf());
	}
	else
	{
		assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
		m_lightMatrixCBuffer[index].upd(&m_cascadeMapsData[index].matrices);
		m_deviceContext->VSSetConstantBuffers(slot, 1, m_lightMatrixCBuffer[index].GetAddressOf());
	}
}

void ShadowMapInstance::bindViewport(uint32_t index)
{
	if (index == UINT_MAX)
	{
		m_viewport.Width = (float)m_singleMapData.textureSize;
		m_viewport.Height = (float)m_singleMapData.textureSize;
	}
	else
	{
		assert(index < SHADOW_CASCADE_COUNT && "Error, index out of reach!");
		m_viewport.Width = (float)m_cascadeMapsData[index].textureSize;
		m_viewport.Height = (float)m_cascadeMapsData[index].textureSize;
	}
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void ShadowMapInstance::bindStatesAndShader()
{
	m_shader.setShaders();
	m_deviceContext->RSSetState(m_rasterizerState.Get());
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
}