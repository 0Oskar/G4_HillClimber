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

	std::vector<LightBFF> pointLights;
	std::vector<LightBFF> spotLights;
	std::vector<LightBFF> directionalLights;

	bool m_drawWithIndex;

public:
	Model();

	void initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, MaterialData material, std::wstring texturePath = L"");
	void draw(DirectX::XMMATRIX &mtrx);
	void loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Vertex> vertexVector, MaterialData material, std::wstring texturePath = L"");
	void loadVertexFromOBJ(ID3D11Device* device, ID3D11DeviceContext* dContext, std::wstring objFilePath, MaterialData material, std::wstring texturePath = L"");
	void initializeModelBff(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string fileName, MaterialData material, std::wstring texturePath = L"");
	void printBffModel(ModelBFF model);

	LightBFF getPointLight(int index);
	LightBFF getSpotLight(int index);
	LightBFF getDirectionalLight(int index);

	void printLight(const ModelBFF model) const;
	//void printSpotLight(const ModelBFF model) const;
	//void printDirectionalLight(const ModelBFF model) const;

	void printCamera(const ModelBFF model) const;

	Material m_material;

	std::wstring m_originalTexture;

};