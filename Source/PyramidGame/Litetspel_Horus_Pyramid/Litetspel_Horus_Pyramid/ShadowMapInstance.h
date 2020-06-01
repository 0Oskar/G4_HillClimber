#pragma once

#include "ConstantBuffer.h"
#include "Shaders.h"
#include "ConstantBuffer.h"

struct VS_SHADOW_C_BUFFER
{
	XMMATRIX lightViewMatrix;
	XMMATRIX lightProjectionMatrix;
	XMMATRIX textureTransformMatrix;
	XMVECTOR lightPosition;
};

class ShadowMapInstance
{
private:
	// Device
	ID3D11DeviceContext* m_deviceContext;

	// Dimensions
	UINT m_width;
	UINT m_height;

	// Resources
	Microsoft::WRL::ComPtr< ID3D11Texture2D > m_shadowMapTexture;
	Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > m_shadowMapSRV;
	Microsoft::WRL::ComPtr< ID3D11DepthStencilView > m_shadowMapDSV;

	// Pipeline States
	Microsoft::WRL::ComPtr< ID3D11DepthStencilState > m_depthStencilState;
	Microsoft::WRL::ComPtr< ID3D11RasterizerState > m_rasterizerState;
	Microsoft::WRL::ComPtr< ID3D11SamplerState > m_comparisonSampler;

	// Render Target
	ID3D11RenderTargetView* m_rendertarget[1] = {0};

	// Viewport
	D3D11_VIEWPORT m_viewport;

	// Shaders
	Shaders m_shadowMapShaders;

	// World Bounding Sphere
	BoundingSphere m_worldBoundingSphere;

	// Constant Buffer
	ConstBuffer<VS_SHADOW_C_BUFFER> m_shadowCBuffer;
	XMMATRIX m_lightViewMatrix;
	XMMATRIX m_lightProjectionMatrix;
	XMMATRIX m_shadowTranformMatrix;

public:
	ShadowMapInstance();
	~ShadowMapInstance();

	// Initialize
	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);

	// Update
	void buildLightMatrix(PS_DIR_BUFFER directionalLight);
	void update();

	// Render
	ID3D11ShaderResourceView* const* getShadowMapSRV();
	void bindViewsAndRenderTarget();
	void setLightMatrixConstantBuffer();
};