#pragma once
#include"pch.h"
#include"ConstantBuffer.h"
#include"Texture.h"

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
	TextureHandler* textureHandler;
public:
	Material()
	{
		this->textureHandler = &TextureHandler::get();
		this->m_texture = nullptr;
	}
	void init(ID3D11Device* device, ID3D11DeviceContext* dContext, MaterialData mat, const WCHAR* texturePath = nullptr)
	{
		this->m_materialBuffer.m_data = mat;
		this->m_materialBuffer.init(device, dContext);
		this->m_materialBuffer.upd();

		if(texturePath != nullptr)
			this->m_texture = this->textureHandler->getTexture(texturePath, device);


		dContext->PSSetConstantBuffers(0, 1, this->m_materialBuffer.GetAdressOf());
		
	}

	void upd(ID3D11DeviceContext* dContext)
	{
		dContext->PSSetConstantBuffers(0, 1, this->m_materialBuffer.GetAdressOf());
		if(m_texture != nullptr)
			dContext->PSSetShaderResources(0, 1, &m_texture);

	}

	
};
