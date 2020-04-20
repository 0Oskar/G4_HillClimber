#include"pch.h"
#include"Model.h"


Model::Model()
{
	this->m_constBufferPtr = nullptr;
	this->m_deviceContextPtr = nullptr;
	this->m_devicePtr = nullptr;
	this->m_movementComponent = new MovementComponent();
	this->drawWithIndex = false;


	
	this->m_worldMatrix = DirectX::XMMatrixIdentity();
}

Model::Model(DirectX::XMVECTOR pos)
{
	this->m_constBufferPtr = nullptr;
	this->m_deviceContextPtr = nullptr;
	this->m_devicePtr = nullptr;
	this->m_movementComponent = new MovementComponent();

	this->drawWithIndex = false;
	this->m_movementComponent->position = pos;
}


void Model::initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, MaterialData material)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

	this->m_constBufferPtr = &constBufferVS;

	std::vector<Vertex> vertices
	{
		Vertex(
			-0.5f, -0.5f, 0.f,
			0.f, 0.f, -1.f,
			0.0f, 1.0f
		),
		Vertex(
			-0.5f, 0.5f, 0.f,
			0.f, 1.f, -1.f,
			0.0f, 0.0f
		),
		Vertex(
			0.5f, 0.5f, 0.f,
			0.f, 0.f, -1.f,
			1.0f, 0.0f
		),
		Vertex(
			0.5f, -0.5f, 0.f,
			0.f, 0.f, -1.f,
			1.0f, 1.0f
		)
	};
	HRESULT hr = this->m_vertexBuffer.initialize(this->m_devicePtr, vertices.data(), (int)vertices.size());
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");

	DWORD indicies[] =
	{
		0, 1, 2,
		0, 2, 3
	};
	hr = this->indexBuffer.init(this->m_devicePtr, indicies, ARRAYSIZE(indicies));
	assert(SUCCEEDED(hr) && "Error, index buffer could not be created!");

	this->m_material.init(device, dContext, material, L"Textures/pyramidTextur.png");
	this->drawWithIndex = true;
	this->updateWorldMatrix();
}

void Model::loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, std::vector<Vertex> vertexVector)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;
	this->m_constBufferPtr = &constBufferVS;

	HRESULT hr = this->m_vertexBuffer.initialize(this->m_devicePtr, vertexVector.data(), (int)vertexVector.size());
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");

	this->updateWorldMatrix();
}

void Model::loadVertexFromOBJ(ID3D11Device* device, ID3D11DeviceContext* dContext, ConstBuffer<VS_CONSTANT_BUFFER>& constBufferVS, std::wstring objFilePath, DirectX::XMFLOAT3 color, MaterialData materialData)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;
	this->m_constBufferPtr = &constBufferVS;

	// Values Contained
	bool hasNormals = false;
	bool hasTexcoords = false;

	// Vertices
	std::vector<DirectX::XMFLOAT3> vertexPositions;
	std::vector<DirectX::XMFLOAT3> vertexNormals;
	std::vector<DirectX::XMFLOAT2> vertexTexcoords;

	// Faces
	std::vector<UINT> vertexPosIndices;
	std::vector<UINT> vertexNormIndices;
	std::vector<UINT> vertexTexIndices;

	// Material
	std::string materialFilePath = "";

	// File Stream
	std::stringstream sStream;
	std::ifstream inFileStream(objFilePath);
	std::string line = "";
	std::string prefix = "";
	DirectX::XMFLOAT3 tempF3;
	DirectX::XMFLOAT2 tempF2;
	UINT tempUINT;

	if (!inFileStream.is_open())
		assert(false && "Error, could not open OBJ file!");

	while (std::getline(inFileStream, line))
	{
		sStream.clear();
		sStream.str(line);
		sStream >> prefix;

		if (prefix == "mtllib")
			sStream >> materialFilePath;
		else if (prefix == "v")
		{
			sStream >> tempF3.x >> tempF3.y >> tempF3.z;
			vertexPositions.push_back(tempF3);
		}
		else if (prefix == "vt")
		{
			hasTexcoords = true;
			sStream >> tempF2.x >> tempF2.y;
			vertexTexcoords.push_back(tempF2);
		}
		else if (prefix == "vn")
		{
			hasNormals = true;
			sStream >> tempF3.x >> tempF3.y >> tempF3.z;
			vertexNormals.push_back(tempF3);
		}
		else if (prefix == "f")
		{
			int counter = 0;
			int counterCap = 2;
			if (!hasNormals)
				counterCap--;

			if (!hasTexcoords)
				counterCap--;

			while (sStream >> tempUINT)
			{
				if (counter == 0)
					vertexPosIndices.push_back(tempUINT);
				else if (counter == 1)
				{
					if (hasTexcoords)
						vertexTexIndices.push_back(tempUINT);
					else if (hasNormals)
						vertexNormIndices.push_back(tempUINT);
				}
				else if (counter == 2)
				{
					if (hasNormals)
						vertexNormIndices.push_back(tempUINT);
				}

				if (sStream.peek() == '/')
				{
					++counter;
					sStream.ignore(1, '/');
					if (sStream.peek() == '/')
						sStream.ignore(1, '/');
				}
				else if (sStream.peek() == ' ')
				{
					++counter;
					sStream.ignore(1, ' ');
				}

				if (counter > counterCap)
					counter = 0;
			}
		}
	}
	inFileStream.close();

	this->m_vertices.resize(vertexPosIndices.size(), Vertex());

	//Load in all indices
	for (size_t i = 0; i < this->m_vertices.size(); ++i)
	{
		this->m_vertices[i].position = vertexPositions[vertexPosIndices[i] - 1];
		//this->m_vertices[i].color = vertexPositions[vertexPosIndices[i] - 1];
		if (vertexTexcoords.size())
			this->m_vertices[i].textureCoord = vertexTexcoords[vertexTexIndices[i] - 1];
			
		if (vertexNormals.size())
			this->m_vertices[i].normal = vertexNormals[vertexNormIndices[i] - 1];
	}

	HRESULT hr = this->m_vertexBuffer.initialize(this->m_devicePtr, this->m_vertices.data(), (int)this->m_vertices.size());
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");
	this->m_material.init(device, dContext, materialData);
	this->updateWorldMatrix();
}

void Model::draw(DirectX::XMMATRIX& viewProjMtx)
{
	this->m_constBufferPtr->m_data.wvp = this->m_worldMatrix * viewProjMtx;
	this->m_constBufferPtr->m_data.worldMatrix = this->m_worldMatrix;
	this->m_constBufferPtr->upd();
	this->m_material.upd(this->m_deviceContextPtr);


	UINT vertexOffset = 0;
	this->m_deviceContextPtr->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), this->m_vertexBuffer.getStridePointer(), &vertexOffset);
	this->m_deviceContextPtr->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	if (this->drawWithIndex)
	{
		this->m_deviceContextPtr->DrawIndexed(this->indexBuffer.getSize(), 0, 0);
	}
	else
		this->m_deviceContextPtr->Draw(this->m_vertexBuffer.getSize(), 0);
}

void Model::updateWorldMatrix()
{
	this->m_worldMatrix = DirectX::XMMATRIX(
		DirectX::XMMatrixScalingFromVector(this->m_movementComponent->scale) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_movementComponent->rotation) *
		DirectX::XMMatrixTranslationFromVector(this->m_movementComponent->position));
}

void Model::setPosition(DirectX::XMVECTOR pos)
{
	this->m_movementComponent->position = pos;
}

void Model::setScale(DirectX::XMVECTOR scale)
{
	this->m_movementComponent->scale = scale;
}