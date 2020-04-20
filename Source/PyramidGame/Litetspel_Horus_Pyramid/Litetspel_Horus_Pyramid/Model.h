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
	ConstBuffer<VS_CONSTANT_BUFFER>* m_constBufferPtr;
	VertexBuffer<Vertex> m_vertexBuffer;
	std::vector<Vertex> m_vertices;

	IndexBuffer indexBuffer;
	Material m_material;

	DirectX::XMMATRIX m_worldMatrix;
	void updateWorldMatrix();
	MovementComponent* m_movementComponent;

	bool drawWithIndex;

public:
	Model();
	Model(DirectX::XMVECTOR pos);

	void initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, MaterialData material);

	void initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, ID3D11ShaderResourceView* texture);

	void draw(DirectX::XMMATRIX &mtrx);
	void setPosition(DirectX::XMVECTOR pos);
	void setScale(DirectX::XMVECTOR scale);
	void loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, std::vector<Vertex> vertexVector);

	void loadVertexFromOBJ(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, std::wstring objFilePath, DirectX::XMFLOAT3 color, MaterialData material);
};

