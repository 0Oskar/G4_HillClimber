#pragma once

#include "ConstantBuffer.h"
#include "Shaders.h"

class Camera;

class ShadowMapInstance
{
private:
	// Device
	ID3D11DeviceContext* m_deviceContext;

	// Dimensions
	UINT m_width;
	UINT m_height;

	// Resources
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > m_shadowMapSRV;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > m_shadowMapDSV;

	// Light Data
	ConstBuffer<VS_VP_MATRICES_CBUFFER> m_lightMatrixCBuffer;
	VS_VP_MATRICES_CBUFFER m_lightMatrices;
	XMFLOAT3 m_lightDirection;

	// Pipeline States
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;
	Microsoft::WRL::ComPtr< ID3D11RasterizerState > m_rasterizerState;
	Microsoft::WRL::ComPtr< ID3D11SamplerState > m_comparisonSampler;

	// Render Target
	ID3D11RenderTargetView* m_rendertarget[1] = {0};

	// Viewport
	D3D11_VIEWPORT m_viewport;

	// World Bounding Sphere
	BoundingSphere m_worldBoundingSphere;

	// Shader
	Shaders m_shader;

public:
	ShadowMapInstance();
	~ShadowMapInstance();

	VS_VP_MATRICES_CBUFFER getLightMatrices() const;
	DirectX::BoundingOrientedBox getLightBoundingBox(float distanceMultipler = 1.1f) const;

	// Initialize
	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);

	// Update
	void buildLightMatrix(XMFLOAT3 lightDirection, XMFLOAT3 position = XMFLOAT3(0,0,0));
	void buildCascadeLightMatrix(XMFLOAT3 lightDirection, Camera* camera);
	void update();

	// Render
	ID3D11ShaderResourceView* getShadowMapSRV();
	ID3D11ShaderResourceView* const* getShadowMapSRVConstPtr();
	void bindViewsAndRenderTarget();
};