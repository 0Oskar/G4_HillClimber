#pragma once
#include"VertexBuffer.h"
#include"ConstantBuffer.h"
#include"MovementComponent.h"
#include"IndexBuffer.h"
#include"Material.h"


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



class Model
{
private:
	ID3D11Device* m_devicePtr;
	ID3D11DeviceContext* m_deviceContextPtr;
	VertexBuffer<Vertex> m_vertexBuffer;
	std::vector<Vertex> m_vertices;
	IndexBuffer m_indexBuffer;
	Material m_material;

	bool m_drawWithIndex;

public:
	Model();

	void initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, MaterialData material);
	void draw(DirectX::XMMATRIX &mtrx);
	void loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Vertex> vertexVector, MaterialData material);
	void loadVertexFromOBJ(ID3D11Device* device, ID3D11DeviceContext* dContext, std::wstring objFilePath, MaterialData material);
};

