#pragma once

#include "ConstantBuffer.h"
#include "Shaders.h"

class Camera;

#define SHADOW_CASCADE_COUNT 3
#define SHADOW_DEPTH_EXTENSION 6.f

struct ShadowDesc
{
	uint32_t textureSize;
	float frustumCoveragePercentage;
};
struct ShadowData
{
	uint32_t textureSize = 0;
	float frustumCoveragePercentage = 0.f;
	float frustumCoverage = 0.f;
	BoundingOrientedBox obb;
	VS_VP_MATRICES_CBUFFER matrices;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > depthStencilView;
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > shaderResourceView;
};

class ShadowMapInstance
{
private:
	// Device
	ID3D11DeviceContext* m_deviceContext;

	// Shadow Map Data
	ShadowData m_singleMapData;
	ShadowData m_cascadeMapsData[SHADOW_CASCADE_COUNT];

	// Light Data
	ConstBuffer<VS_VP_MATRICES_CBUFFER> m_lightMatrixCBuffer[SHADOW_CASCADE_COUNT];
	VS_VP_MATRICES_CBUFFER m_lightMatrices;
	XMFLOAT3 m_lightDirection;

	// Pipeline States
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;
	Microsoft::WRL::ComPtr< ID3D11RasterizerState > m_rasterizerState;
	Microsoft::WRL::ComPtr< ID3D11SamplerState > m_comparisonSampler;

	// Viewport
	D3D11_VIEWPORT m_viewport;

	// World Bounding Sphere
	BoundingSphere m_worldBoundingSphere;

	// Shader
	Shaders m_shader;

	void initializeTexture(ID3D11Device* device, ShadowData& data);

public:
	ShadowMapInstance();
	~ShadowMapInstance();

	bool cascadedShadowMapsToggle = true;
	DirectX::BoundingOrientedBox getLightBoundingBox(uint32_t index = UINT_MAX) const;

	// Initialize
	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, uint32_t singleMapSize, ShadowDesc cascadingMapDesc[SHADOW_CASCADE_COUNT]);
	
	// Update
	ShadowData* getShadowData(uint32_t index);
	void buildLightMatrix(XMFLOAT3 lightDirection, XMFLOAT3 position = XMFLOAT3(0,0,0));
	void buildCascadeLightMatrix(uint32_t index, XMFLOAT3 lightDirection, Camera* camera);
	void update();
	void reloadShaders();

	// Render
	ID3D11ShaderResourceView* getShadowMapSRV(uint32_t index = UINT_MAX);
	ID3D11ShaderResourceView* const* getShadowMapSRVConstPtr(uint32_t index = UINT_MAX);
	ID3D11DepthStencilView* getShadowMapDSV(uint32_t index = UINT_MAX);
	void clearShadowmap();
	void setComparisonSampler();
	void bindLightMatrixBufferVS(uint32_t index, uint32_t slot);
	void bindViewport(uint32_t index = UINT_MAX);
	void bindStatesAndShader();
};