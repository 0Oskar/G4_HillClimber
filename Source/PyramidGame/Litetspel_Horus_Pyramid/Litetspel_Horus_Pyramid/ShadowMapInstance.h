#pragma once

#include "ConstantBuffer.h"
#include "Shaders.h"

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

	// Constant Buffers
	ConstBuffer<VS_DIRECTIONAL_CBUFFER> m_lightMatrixCBuffer;

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

	// Initialize
	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);

	// Update
	void buildLightMatrix(PS_DIR_BUFFER directionalLight, XMFLOAT3 position = XMFLOAT3(0,0,0));
	void update();

	// Render
	ID3D11ShaderResourceView* getShadowMapSRV();
	ID3D11ShaderResourceView* const* getShadowMapSRVConstPtr();
	void bindViewsAndRenderTarget();
};