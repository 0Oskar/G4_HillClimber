#pragma once

#include"Shaders.h"
#include"VertexBuffer.h"
#include"ConstantBuffer.h"
#include"MovementComponent.h"
#include"IndexBuffer.h"
#include"Material.h"

class Model
{
private:
	ID3D11Device* m_devicePtr;
	ID3D11DeviceContext* m_deviceContextPtr;
	VertexBuffer<Vertex> m_vertexBuffer;
	std::vector<Vertex> m_vertices;
	IndexBuffer m_indexBuffer;
	ImporterBFF::Manager* myManager;

	bool m_drawWithIndex;

public:
	Model();

	void initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, const MaterialData material, const std::wstring texturePath = L"");
	void draw(DirectX::XMMATRIX &mtrx);
	void loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Vertex> vertexVector, const MaterialData material, const std::wstring texturePath = L"");
	void loadVertexFromOBJ(ID3D11Device* device, ID3D11DeviceContext* dContext, const std::wstring objFilePath, const MaterialData material, const std::wstring texturePath = L"");
	void initializeModelBff(ID3D11Device* device, ID3D11DeviceContext* dContext, const std::string fileName, const MaterialData material, const std::wstring texturePath = L"");
	void printBffModel(const ModelBFF model) const;
	void printLight(const ModelBFF model) const;
	Material m_material;

	std::wstring m_originalTexture;

};