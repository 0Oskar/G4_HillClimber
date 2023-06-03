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
		m_stride = 0;
		m_nrOf = 0;
	}

	~VertexBuffer() {}

	void initialize(ID3D11Device* device, T* data, const int nrOfVertices)
	{
		m_nrOf = nrOfVertices;
		m_stride = UINT(sizeof(T));

		// Buffer Description
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.ByteWidth = m_stride * m_nrOf;
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

		HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_buffer);
		assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");
	}

	// Getters
	ID3D11Buffer* Get() const { return m_buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const { return m_buffer.GetAddressOf(); }

	const UINT getStride() const { return *m_stride; }
	const UINT* getStridePointer() const { return &m_stride; }

	UINT getSize() const { return m_nrOf; }
};