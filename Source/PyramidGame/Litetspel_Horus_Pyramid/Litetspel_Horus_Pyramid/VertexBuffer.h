#pragma once

#include "pch.h"

template <typename T>
class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr< ID3D11Buffer > m_buffer;
	UINT m_stride;
	UINT m_nrOf;

public:
	VertexBuffer()
	{
		this->m_buffer = nullptr;
		this->m_stride = 0;
		this->m_nrOf = 0;
	}

	~VertexBuffer()
	{
		if (this->m_buffer)
			this->m_buffer->Release();
	}

	HRESULT initialize(ID3D11Device* device, T* data, int nrOfVertices)
	{
		this->m_nrOf = nrOfVertices;
		this->m_stride = UINT(sizeof(T));

		// Buffer Description
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.ByteWidth = this->m_stride * this->m_nrOf;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		// Subresource data
		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(vertexData));
		vertexData.pSysMem = data;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		return device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_buffer);
	}

	// Getters
	ID3D11Buffer* Get() const { return this->m_buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return this->m_buffer.GetAddressOf(); }

	const UINT getStride() const { return *this->m_stride; }
	const UINT* getStridePointer() const { return &this->m_stride; }

	UINT getSize() const { return this->m_nrOf; }
};