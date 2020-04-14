#include"pch.h"
#include"Model.h"

Model::Model()
{
	this->m_constBufferPtr = nullptr;
	this->m_deviceContextPtr = nullptr;
	this->m_devicePtr = nullptr;
	this->m_movementComponent = new MovementComponent();
	
	this->m_worldMatrix = DirectX::XMMatrixIdentity();
}

Model::Model(DirectX::XMVECTOR pos)
{
	this->m_constBufferPtr = nullptr;
	this->m_deviceContextPtr = nullptr;
	this->m_devicePtr = nullptr;
	this->m_movementComponent = new MovementComponent();

	this->m_movementComponent->position = pos;
	
}

void Model::initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

	this->m_constBufferPtr = &constBufferVS;

	std::vector<Vertex> vertices
	{
		Vertex(
			-0.5f, -0.5f, 0.f,
			1.f, 0.f, 0.f
		),
		Vertex(
			0.f, 0.5f, 0.f,
			0.f, 1.f, 0.f
		),
		Vertex(
			0.5f, -0.5f, 0.f,
			0.f, 0.f, 1.f
		)
	};

	HRESULT hr = this->m_vertexBuffer.initialize(this->m_devicePtr, vertices.data(), (int)vertices.size());
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");

	this->updateWorldMatrix();
}

void Model::loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, std::vector<Vertex> vertexVector)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

	this->m_constBufferPtr = &constBufferVS;

	HRESULT hr = this->m_vertexBuffer.initialize(this->m_devicePtr, vertexVector.data(), (int)vertexVector.size());
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");

}

void Model::draw(DirectX::XMMATRIX& viewProjMtx)
{
	this->m_constBufferPtr->m_data.wvp = this->m_worldMatrix * viewProjMtx;
	this->m_constBufferPtr->upd();

	UINT vertexOffset = 0;
	this->m_deviceContextPtr->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), this->m_vertexBuffer.getStridePointer(), &vertexOffset);
	this->m_deviceContextPtr->Draw(this->m_vertexBuffer.getSize(), 0);
}

void Model::updateWorldMatrix()
{
	this->m_worldMatrix = DirectX::XMMATRIX(
		DirectX::XMMatrixScalingFromVector(this->m_movementComponent->scale) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_movementComponent->rotation) *
		DirectX::XMMatrixTranslationFromVector(this->m_movementComponent->position));
}

void Model::setPosition(DirectX::XMVECTOR pos)
{
	this->m_movementComponent->position = pos;
}