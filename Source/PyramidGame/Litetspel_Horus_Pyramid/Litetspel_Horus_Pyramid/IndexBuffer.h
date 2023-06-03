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
		indexBufferPtr = nullptr;
		size = 0;
	}

	void init(ID3D11Device* device, DWORD* data, const UINT nrOf)
	{
		size = nrOf;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = data;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(DWORD) * nrOf;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		HRESULT hr = device->CreateBuffer(&bufferDesc, &bufferData, indexBufferPtr.GetAddressOf());
		assert(SUCCEEDED(hr) && "Error, index buffer could not be created!");
	}
	UINT getSize() const
	{
		return size;
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