#pragma once
#include"VertexBuffer.h"
#include"ConstantBuffer.h"
#include"MovementComponent.h"


struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
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

	DirectX::XMMATRIX m_worldMatrix;
	void updateWorldMatrix();
	MovementComponent* m_movementComponent;

public:
	Model();
	Model(DirectX::XMVECTOR pos);
	void initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS);
	void draw(DirectX::XMMATRIX &mtrx);

	void setPosition(DirectX::XMVECTOR pos);

	



};