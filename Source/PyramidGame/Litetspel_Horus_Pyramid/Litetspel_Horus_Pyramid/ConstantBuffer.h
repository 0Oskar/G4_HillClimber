#pragma once

#include "pch.h"

struct VS_CONSTANT_BUFFER
{
	DirectX::XMMATRIX wvp;
};

template<class T>
class ConstBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* dContextPtr = nullptr;
public:
	T m_data;

	ConstBuffer()
	{

	}

	ID3D11Buffer* Get(){ return buffer.Get(); }
	ID3D11Buffer* const* GetAdressOf() const { return buffer.GetAddressOf(); }
	HRESULT init(ID3D11Device* device, ID3D11DeviceContext* dContext)
	{
		this->dContextPtr = dContext;

		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&constantBufferDesc, 0, buffer.GetAddressOf());
		assert(SUCCEEDED(hr) && "Error, failed to create constant buffer!");
		return hr;
		

	}
	void upd(T* data = nullptr)
	{
		if (data)
			this->m_data = *data;

		D3D11_MAPPED_SUBRESOURCE mapSubresource;
		HRESULT hr = this->dContextPtr->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		assert(SUCCEEDED(hr) && "Error, failed to map CBuffer!");
		CopyMemory(mapSubresource.pData, &m_data, sizeof(T));
		
		dContextPtr->Unmap(buffer.Get(), 0);
	}

};