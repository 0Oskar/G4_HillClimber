#pragma once
#include"VertexBuffer.h"
#include"ConstantBuffer.h"
#include"MovementComponent.h"
#include"IndexBuffer.h"


struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	Vertex()
	{
		position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		color = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	}
	Vertex(float x, float y, float z, float r, float g, float b)
	{
		position = DirectX::XMFLOAT3(x, y, z);
		color = DirectX::XMFLOAT3(r, g, b);
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

	DirectX::XMMATRIX m_worldMatrix;
	void updateWorldMatrix();
	MovementComponent* m_movementComponent;

	bool drawWithIndex;

public:
	Model();
	Model(DirectX::XMVECTOR pos);
	void initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS);
	void draw(DirectX::XMMATRIX &mtrx);
	void setPosition(DirectX::XMVECTOR pos);
	void setScale(DirectX::XMVECTOR scale);
	void loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, std::vector<Vertex> vertexVector);
	void loadVertexFromOBJ(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, std::wstring objFilePath, DirectX::XMFLOAT3 color);
};