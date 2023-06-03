#pragma once

#include"Shaders.h"
#include"VertexBuffer.h"
#include"ConstantBuffer.h"
#include"MovementComponent.h"
#include"IndexBuffer.h"
#include"Material.h"

static const char* MODEL_PATH = "Models/";
static const char* ERROR_MODEL = "cube.obj";

class Model
{
	enum class ModelFormat
	{
		ModelFormat_obj,
		ModelFormat_bff,
		ModelFormat_custom
	};

private:
	ID3D11Device* m_devicePtr;
	ID3D11DeviceContext* m_deviceContextPtr;
	VertexBuffer<Vertex> m_vertexBuffer;
	std::vector<Vertex> m_vertices;
	IndexBuffer m_indexBuffer;
	ImporterBFF::Manager* m_myManager;

	ModelFormat m_format;
	bool m_drawWithIndex;
	bool m_loaded;

	void loadOBJModel();
	void loadBffModel(bool async = false);

public:
	Model();

	void initForAsyncLoad(ID3D11Device* device, ID3D11DeviceContext* dContext, const char* modelFilePath, const MaterialData material, const wchar_t* texturePath = L"");
	void loadModelAsync();
	void loadModel(ID3D11Device* device, ID3D11DeviceContext* dContext, const char* modelFilePath, const MaterialData material, const wchar_t* texturePath = L"");

	void initCubeModel(ID3D11Device* device, ID3D11DeviceContext* dContext, const MaterialData material, const std::wstring texturePath = L"");
	void initModelFromData(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Vertex> vertexVector, const MaterialData material, const std::wstring texturePath = L"");
	void printBffModel(const ModelBFF model) const;
	
	bool is_loaded() const;
	
	void draw();

	Material m_material;

	std::wstring m_texturePath;
	std::string m_modelPath; // Makes it easier to debug
};