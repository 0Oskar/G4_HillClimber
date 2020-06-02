#pragma once
#include"pch.h"
#include"ConstantBuffer.h"
#include"ResourceHandler.h"

struct MaterialData
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	int shininess;
};

class Material
{
private:
	ConstBuffer<MaterialData> m_materialBuffer;
	ID3D11ShaderResourceView* m_texture;
	ResourceHandler* resourceHandler;
	
public:
	Material()
	{
		this->resourceHandler = &ResourceHandler::get();
		this->m_texture = nullptr;
	}
	void init(ID3D11Device* device, ID3D11DeviceContext* dContext, const MaterialData mat, const WCHAR* texturePath = nullptr)
	{
		if (this->resourceHandler->m_device == nullptr)
		{
			this->resourceHandler->m_device = device;
			this->resourceHandler->m_dContext = dContext;
		}
		this->m_materialBuffer.m_data = mat;
		this->m_materialBuffer.init(device, dContext);
		this->m_materialBuffer.upd();

		if (texturePath != nullptr)
		{
			this->m_texture = this->resourceHandler->getTexture(texturePath);
		}

		dContext->PSSetConstantBuffers(0, 1, this->m_materialBuffer.GetAdressOf());
		
	}

	void setTexture(const WCHAR* texturePath)
	{
		this->m_texture = this->resourceHandler->getTexture(texturePath);
	}

	void upd(ID3D11DeviceContext* dContext)
	{
		dContext->PSSetConstantBuffers(0, 1, this->m_materialBuffer.GetAdressOf());
		if(m_texture != nullptr)
			dContext->PSSetShaderResources(0, 1, &m_texture);

	}
	
};
