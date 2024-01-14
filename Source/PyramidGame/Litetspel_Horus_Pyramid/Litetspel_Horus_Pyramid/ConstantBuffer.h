#pragma once

#define MAX_POINT_LIGHTS 20

struct PointLight
{
	DirectX::XMFLOAT3 position;
	float range;
	DirectX::XMFLOAT3 diffuse;
	float pad;
};
struct VS_WVPW_CONSTANT_BUFFER
{
	XMMATRIX wvp;
	XMMATRIX worldMatrix;
};
struct VP_MATRICES_CBUFFER
{
	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;
};
struct SHADOW_MATRICES_CBUFFER
{
	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;
	XMFLOAT2 textureOffset;
	float pad[2];
};

struct CAMERA_BUFFER
{
	XMMATRIX invViewMatrix;
	XMMATRIX invProjMatrix;
	DirectX::XMFLOAT3 cameraPosition;
	float pad;
};

#define CASCADING_LIGHT_COUNT 3

struct CASCADING_LIGHT_CAMERA_BUFFER
{
	XMMATRIX invViewMatrix[CASCADING_LIGHT_COUNT];
	XMMATRIX invProjMatrix[CASCADING_LIGHT_COUNT];
	uint32_t textureSize[CASCADING_LIGHT_COUNT];
	float frustumCoverage[CASCADING_LIGHT_COUNT];
	float pad[2];
};

struct SHADOW_PER_FRAME_BUFFER
{
	BOOL cascadingShadowMapsToggle;
	DirectX::XMFLOAT3 cameraPos;

	float frustumCoverage0;
	float frustumCoverage1;
	float frustumCoverage2;

	BOOL shadowDebug;
};

struct PS_PER_FRAME_BUFFER
{
	PointLight pointLights[MAX_POINT_LIGHTS];
	uint32_t nrOfPointLights;
	DirectX::XMFLOAT3 skyLightDirection;
	DirectX::XMFLOAT3 skyLightColor;
	float skyLightIntensity;
	DirectX::XMFLOAT3 cameraPos;
	float fogStart;
	float fogEnd;
	DirectX::XMFLOAT3 ambientColor;
};

struct DIR_LIGHT_DATA
{
	DirectX::XMFLOAT3 lightDirection;
	uint32_t use_custom_horizon_color;
	DirectX::XMFLOAT3 lightColor;
	float pad;
};

struct VOLUMETRIC_LIGHT_DATA
{
	DirectX::XMFLOAT3 skyLightDirection;
	float skyLightIntensity;
	DirectX::XMFLOAT3 skyLightColor;
	float pad;
	DirectX::XMFLOAT2 textureStep;
	float pad2[2];
};

const int MAX_BLUR_RADIUS = 10;
struct CS_BLUR_CBUFFER
{
	XMMATRIX projectionMatrix;
	int radius;
	BOOL direction;
	XMFLOAT2 pad;
	alignas(16) float weights[(MAX_BLUR_RADIUS * 2) + 1]; // BLUR_RADIUS + 1 * 4 bytes
};

template<class T>
class ConstBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* dContextPtr = nullptr;
public:
	T m_data;

	ConstBuffer()
	{

	}

	ID3D11Buffer* Get(){ return buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }
	HRESULT init(ID3D11Device* device, ID3D11DeviceContext* dContext)
	{
		dContextPtr = dContext;

		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = &m_data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&constantBufferDesc, &subresourceData, buffer.GetAddressOf());
		assert(SUCCEEDED(hr) && "Error, failed to create constant buffer!");
		return hr;
	}
	void upd(T* data = nullptr)
	{
		if (data)
			m_data = *data;

		D3D11_MAPPED_SUBRESOURCE mapSubresource;
		HRESULT hr = dContextPtr->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		assert(SUCCEEDED(hr) && "Error, failed to map CBuffer!");
		CopyMemory(mapSubresource.pData, &m_data, sizeof(T));
		
		dContextPtr->Unmap(buffer.Get(), 0);
	}

};