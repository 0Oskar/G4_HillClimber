#pragma once

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureCoord;
	Vertex()
	{
		position = XMFLOAT3(0.f, 0.f, 0.f);
		normal = XMFLOAT3(0.f, 0.f, 0.f);
		textureCoord = XMFLOAT2(0.f, 0.f);
	}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		position = XMFLOAT3(x, y, z);
		normal = XMFLOAT3(nx, ny, nz);
		textureCoord = XMFLOAT2(u, v);
	}
};

const D3D11_INPUT_ELEMENT_DESC PosNorTexDesc[] = {
	{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		12,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		24,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	}
};

struct VertexPosTex
{
	XMFLOAT3 position;
	XMFLOAT2 textureCoord;
	VertexPosTex()
	{
		position = XMFLOAT3(0.f, 0.f, 0.f);
		textureCoord = XMFLOAT2(0.f, 0.f);
	}
	VertexPosTex(float x, float y, float z, float u, float v)
	{
		position = XMFLOAT3(x, y, z);
		textureCoord = XMFLOAT2(u, v);
	}
};

const D3D11_INPUT_ELEMENT_DESC PosTexDesc[] = {
	{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		12,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
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

enum class LayoutType { POS_NOR_TEX, POS_TEX};

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

	// File Names
	ShaderFiles m_files;

	// Layout
	Microsoft::WRL::ComPtr< ID3D11InputLayout > m_layout;
	LayoutType m_layoutType;

	// Topology
	D3D_PRIMITIVE_TOPOLOGY m_topology;

public:
	Shaders();
	virtual ~Shaders();

	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ShaderFiles names, LayoutType layoutType = LayoutType::POS_NOR_TEX, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void reloadShaders();

	void setShaders();
	void unbindShaders();
};