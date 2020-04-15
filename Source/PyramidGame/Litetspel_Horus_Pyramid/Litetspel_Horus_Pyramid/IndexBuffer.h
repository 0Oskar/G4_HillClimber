#pragma once
#include "pch.h"

class IndexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBufferPtr;
	UINT size;
public:
	IndexBuffer() 
	{
		this->indexBufferPtr = nullptr;
		this->size = 0;
	}

	HRESULT init(ID3D11Device* device, DWORD* data, UINT nrOf)
	{
		this->size = nrOf;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = data;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(DWORD) * nrOf;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		HRESULT hr = device->CreateBuffer(&bufferDesc, &bufferData, this->indexBufferPtr.GetAddressOf());
		return hr;
	}
	UINT getSize()
	{
		return this->size;
	}
	ID3D11Buffer* Get()
	{
		return indexBufferPtr.Get();
	}

	ID3D11Buffer* const* GetAdressOf()
	{
		return indexBufferPtr.GetAddressOf();
	}



};