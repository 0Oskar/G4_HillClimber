#pragma once
#include"pch.h"
#include"ConstantBuffer.h"

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
public:
	Material(){}
	void init(ID3D11Device* device, ID3D11DeviceContext* dContext, MaterialData mat)
	{
		this->m_materialBuffer.m_data = mat;
		this->m_materialBuffer.init(device, dContext);
		this->m_materialBuffer.upd();
		dContext->PSSetConstantBuffers(0, 1, this->m_materialBuffer.GetAdressOf());
	}

	void upd(ID3D11DeviceContext* dContext)
	{
		dContext->PSSetConstantBuffers(0, 1, this->m_materialBuffer.GetAdressOf());
	}
};
