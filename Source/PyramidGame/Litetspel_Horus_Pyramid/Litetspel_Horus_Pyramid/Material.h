#pragma once
#include"pch.h"
#include"ConstantBuffer.h"
#include"ResourceHandler.h"

struct MaterialData
{
	DirectX::XMFLOAT3 ambient;
	float globalAmbientContribution;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	int shininess;
};

class Material
{
private:
	ConstBuffer<MaterialData> m_materialBuffer;
	ID3D11ShaderResourceView* m_texture;
	ResourceHandler* m_resourceHandlerPtr;
	
public:
	Material()
	{
		m_resourceHandlerPtr = &ResourceHandler::get();
		m_texture = nullptr;
	}
	void init(ID3D11Device* device, ID3D11DeviceContext* dContext, const MaterialData mat, const WCHAR* texturePath = nullptr)
	{
		if (m_resourceHandlerPtr->m_device == nullptr)
		{
			m_resourceHandlerPtr->m_device = device;
			m_resourceHandlerPtr->m_dContext = dContext;
		}
		m_materialBuffer.m_data = mat;
		m_materialBuffer.init(device, dContext);

		if (texturePath != nullptr)
		{
			m_texture = m_resourceHandlerPtr->getTexture(texturePath);
		}

		dContext->PSSetConstantBuffers(0, 1, m_materialBuffer.GetAddressOf());
		
	}

	MaterialData* getMaterial()
	{
		return &m_materialBuffer.m_data;
	}

	void setMaterial(MaterialData mat)
	{
		m_materialBuffer.upd(&mat);
	}

	void setTexture(const WCHAR* texturePath)
	{
		m_texture = m_resourceHandlerPtr->getTexture(texturePath);
	}

	void upd(ID3D11DeviceContext* dContext)
	{
		dContext->PSSetConstantBuffers(0, 1, m_materialBuffer.GetAddressOf());
		if(m_texture != nullptr)
			dContext->PSSetShaderResources(0, 1, &m_texture);

	}
	
};
