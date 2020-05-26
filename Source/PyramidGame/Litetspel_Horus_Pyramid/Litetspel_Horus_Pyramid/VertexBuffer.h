#pragma once

#include "pch.h"

template <typename T>
class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr< ID3D11Buffer > m_buffer;
	UINT m_stride;
	UINT m_nrOf;

	ID3D11DeviceContext* m_dContextPtr;
	//ID3D11Device* m_Device;

public:
	VertexBuffer()
	{
		this->m_stride = 0;
		this->m_nrOf = 0;
	}

	~VertexBuffer() {}

	HRESULT initialize(ID3D11Device* device, T* data, int nrOfVertices, bool immutable = true, bool streamOutput = false) // PARTICLE EDIT HERE
	{
		this->m_nrOf = nrOfVertices;
		this->m_stride = UINT(sizeof(T));
		this->m_dContextPtr = nullptr;
		//this->m_Device = nullptr;

		// Buffer Description
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.ByteWidth = this->m_stride * this->m_nrOf;
		if(immutable)
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		else 
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		if(streamOutput)
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		else
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

	// Update
	void update(T* data, int nrOfVertices)
	{

		D3D11_MAPPED_SUBRESOURCE mapSubresource;
		HRESULT hr = this->m_dContextPtr->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		assert(SUCCEEDED(hr) && "Error, failed to map VertexBuffer!");
		CopyMemory(mapSubresource.pData, &data, sizeof(T));

		m_dContextPtr->Unmap(this->m_buffer.Get(), 0);
	}
	

};