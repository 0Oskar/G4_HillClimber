#pragma once

#include "ConstantBuffer.h"
#include "Shaders.h"

class Camera;

#define SHADOW_CASCADE_COUNT 3
#define SHADOW_CASCADE_OFFSET (1.f / SHADOW_CASCADE_COUNT)
#define SHADOW_DEPTH_EXTENSION 6.f

struct SingleShadowDesc
{
	uint32_t textureSize;
	float radius;
};
struct CascadingShadowDesc
{
	uint32_t textureSize;
	float frustumCoveragePercentage[SHADOW_CASCADE_COUNT];
};
struct ShadowTextureData
{
	XMFLOAT2 textureDimensions;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > depthStencilView;
	Microsoft::WRL::ComPtr< ID3D11RenderTargetView > renderTargetView;
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > shaderResourceView;
};

struct ShadowFrustumData
{
	float frustumCoveragePercentage = 0.f;
	float frustumCoverage = 0.f;
	BoundingOrientedBox obb;
	SHADOW_MATRICES_CBUFFER matrixData;
};

class ShadowMapInstance
{
private:
	// Device
	ID3D11DeviceContext* m_deviceContext;

	// Shadow Map Data
	bool m_cascadedShadowMapsEnabled = true;

	ShadowFrustumData m_singleMapData;
	ShadowTextureData m_singleMapTextureData;

	ShadowFrustumData m_cascadeMapsData[SHADOW_CASCADE_COUNT];
	ShadowTextureData m_cascadeMapsTextureData;

	// Light Data
	ConstBuffer<SHADOW_MATRICES_CBUFFER> m_lightMatrixCBuffer[SHADOW_CASCADE_COUNT];

	// Pipeline States
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;
	Microsoft::WRL::ComPtr< ID3D11RasterizerState > m_rasterizerState;
	Microsoft::WRL::ComPtr< ID3D11SamplerState > m_comparisonSampler;

	// Viewport
	D3D11_VIEWPORT m_viewport[SHADOW_CASCADE_COUNT];

	// World Bounding Sphere
	BoundingSphere m_worldBoundingSphere;

	// Shader
	Shaders m_shader;

	void initializeDepthTexture(ID3D11Device* device, ShadowTextureData& data);
	void initializeRendertargetTexture(ID3D11Device* device, ShadowTextureData& data);

public:
	ShadowMapInstance();
	~ShadowMapInstance();

	bool translucentShadowMapsToggle = false;
	DirectX::BoundingOrientedBox getLightBoundingBox(uint32_t index = UINT_MAX) const;

	// Initialize
	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, SingleShadowDesc singleMapDesc, CascadingShadowDesc cascadingMapDesc, bool cascadedShadowMapsEnabled, bool translucent = false);
	
	// Update
	ShadowFrustumData* getShadowData(uint32_t index = UINT_MAX);
	XMFLOAT2 getShadowTextureSize();
	void buildLightMatrix(XMFLOAT3 lightDirection, XMFLOAT3 position = XMFLOAT3(0,0,0));
	void buildCascadeLightMatrix(uint32_t index, XMFLOAT3 lightDirection, Camera* camera);
	void reloadShaders();

	// Render
	ID3D11ShaderResourceView* getShadowMapSRV();
	ID3D11ShaderResourceView* const* getShadowMapSRVConstPtr();
	ID3D11DepthStencilView* getShadowMapDSV();
	ID3D11RenderTargetView* const* getShadowMapRTVConstPtr();
	void clearShadowmap();
	void setComparisonSampler();
	void bindLightMatrixBufferVS(uint32_t index, uint32_t slot);
	void bindStatesAndShader(uint32_t index = 0);
};