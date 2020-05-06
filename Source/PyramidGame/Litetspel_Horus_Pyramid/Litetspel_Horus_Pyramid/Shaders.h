// Medo Osman, 961130-3711
#pragma once

#include "pch.h"

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 textureCoord;
	Vertex()
	{
		position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		normal = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		textureCoord = DirectX::XMFLOAT2(0.f, 0.f);
	}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		position = DirectX::XMFLOAT3(x, y, z);
		normal = DirectX::XMFLOAT3(nx, ny, nz);
		textureCoord = DirectX::XMFLOAT2(u, v);
	}
};

struct ShaderFiles
{
	LPCWSTR vs = L""; // Vertex Shader
	LPCWSTR hs = L""; // Hull Shader
	LPCWSTR ds = L""; // Domain Shader
	LPCWSTR gs = L""; // Geometry Shader
	LPCWSTR ps = L""; // Pixel Shader
	LPCWSTR cs = L""; // Compute Shader
};

class Shaders
{
protected:
	// Device
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

private:
	// Shaders
	Microsoft::WRL::ComPtr< ID3D11VertexShader > m_vertexShader;
	Microsoft::WRL::ComPtr < ID3D11HullShader > m_hullShader;
	Microsoft::WRL::ComPtr < ID3D11DomainShader > m_domainShader;
	Microsoft::WRL::ComPtr < ID3D11GeometryShader > m_geometryShader;
	Microsoft::WRL::ComPtr< ID3D11PixelShader > m_pixelShader;
	Microsoft::WRL::ComPtr < ID3D11ComputeShader > m_computeShader;


	// Layout
	Microsoft::WRL::ComPtr< ID3D11InputLayout > m_layout;

	// Topology
	D3D_PRIMITIVE_TOPOLOGY m_topology;

public:
	Shaders();
	virtual ~Shaders();

	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ShaderFiles names, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void setShaders();
	void unbindShaders();
};