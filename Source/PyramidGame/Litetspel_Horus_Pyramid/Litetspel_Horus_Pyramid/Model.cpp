#include"pch.h"
#include"Model.h"


Model::Model()
{
	this->m_deviceContextPtr = nullptr;
	this->m_devicePtr = nullptr;
	this->m_drawWithIndex = false;
	myManager = &ImporterBFF::Manager::GetInstance();
}

void Model::initModel(ID3D11Device* device, ID3D11DeviceContext* dContext, MaterialData material, std::wstring texturePath)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

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
	this->m_drawWithIndex = true;
	hr = this->m_indexBuffer.init(this->m_devicePtr, indicies, ARRAYSIZE(indicies));
	assert(SUCCEEDED(hr) && "Error, index buffer could not be created!");

	if (texturePath != L"")
		this->m_material.init(device, dContext, material, texturePath.c_str());
	else
		this->m_material.init(device, dContext, material);
}

void Model::loadVertexVector(ID3D11Device* device, ID3D11DeviceContext* dContext, std::vector<Vertex> vertexVector, MaterialData material, std::wstring texturePath)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

	HRESULT hr = this->m_vertexBuffer.initialize(this->m_devicePtr, vertexVector.data(), (int)vertexVector.size());
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");

	this->m_vertices = vertexVector;

	if (texturePath != L"")
	{
		this->m_material.init(device, dContext, material, texturePath.c_str());
		this->m_originalTexture = texturePath;
	}
	else
		this->m_material.init(device, dContext, material);
}

void Model::loadVertexFromOBJ(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string modelFilePath, MaterialData material, std::wstring texturePath)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

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
	std::ifstream inFileStream("Models/" + modelFilePath);
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
			tempF3.z *= -1;
			vertexPositions.push_back(tempF3);
		}
		else if (prefix == "vt")
		{
			hasTexcoords = true;
			sStream >> tempF2.x >> tempF2.y;
			tempF2.y *= -1;
			vertexTexcoords.push_back(tempF2);
		}
		else if (prefix == "vn")
		{
			hasNormals = true;
			sStream >> tempF3.x >> tempF3.y >> tempF3.z;
			tempF3.z *= -1;
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

	if (texturePath != L"")
	{
		this->m_material.init(device, dContext, material, texturePath.c_str());
		this->m_originalTexture = texturePath;
	}
	else
		this->m_material.init(device, dContext, material);

	std::string loadedMessage = "OBJ Model loaded: " + modelFilePath + "\n";
	OutputDebugStringA(loadedMessage.c_str());
}

void Model::initializeModelBff(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string modelFilePath, MaterialData material, std::wstring texturePath)
{
	this->m_devicePtr = device;
	this->m_deviceContextPtr = dContext;

	ModelBFF myModel = myManager->LoadModel(modelFilePath.c_str());
	m_vertices.resize(myModel.mesh.nrOfVertex);
	for (unsigned int i = 0; i < myModel.mesh.nrOfVertex; i++)
	{
		m_vertices[i].position = XMFLOAT3(myModel.vertexArr[i].pos);
		m_vertices[i].normal = XMFLOAT3(myModel.vertexArr[i].norm);
		m_vertices[i].textureCoord = XMFLOAT2(myModel.vertexArr[i].uv);
	}

	HRESULT hr = this->m_vertexBuffer.initialize(this->m_devicePtr, m_vertices.data(), (int)m_vertices.size());
	assert(SUCCEEDED(hr) && "Error, vertex buffer could not be created!");

	if (texturePath != L"")
	{
		this->m_material.init(device, dContext, material, texturePath.c_str());
		this->m_originalTexture = texturePath;
	}
	else
		this->m_material.init(device, dContext, material);

	//printBffModel(myModel); //For testing
	std::string loadedMessage = "BFF Model loaded: " + modelFilePath + "\n";
	OutputDebugStringA(loadedMessage.c_str());
}

void Model::printBffModel(ModelBFF model) const
{
	for (unsigned int i = 0; i < model.mesh.nrOfVertex; i++)
	{
		std::string vtxNr = "\nvtx:		" + std::to_string(i) + "\n";

		std::string vtxPos = "Pos:		" + 
			(std::to_string(model.vertexArr[i].pos[0]) + " " +
			(std::to_string(model.vertexArr[i].pos[1])) + " " +
			(std::to_string(model.vertexArr[i].pos[2]))) + "\n";

		std::string uv = "uv:			" + 
			(std::to_string(model.vertexArr[i].uv[0]) + " " +
			(std::to_string(model.vertexArr[i].uv[1]))) + "\n";

		std::string normal = "Normal:		" + 
			(std::to_string(model.vertexArr[i].norm[0]) + " " +
			(std::to_string(model.vertexArr[i].norm[1])) + " " +
			(std::to_string(model.vertexArr[i].norm[2]))) + "\n";

		std::string biNormal = "Binormal:	" + 
			(std::to_string(model.vertexArr[i].biNorm[0]) + " " +
			(std::to_string(model.vertexArr[i].biNorm[1])) + " " +
			(std::to_string(model.vertexArr[i].biNorm[2]))) + "\n";

		std::string tangent = "Tan:		" + 
			(std::to_string(model.vertexArr[i].tan[0]) + " " +
			(std::to_string(model.vertexArr[i].tan[1])) + " " +
			(std::to_string(model.vertexArr[i].tan[2]))) + "\n";



		std::string pos = "Pos:    "+
			(std::to_string(model.camera.pos[0]) + " " +
			(std::to_string(model.camera.pos[1])) + " " +
			(std::to_string(model.camera.pos[2]))) + "\n";

		std::string upVec = "upVec:    " +
			(std::to_string(model.camera.upVec[0]) + " " +
			(std::to_string(model.camera.upVec[1])) + " " +
			(std::to_string(model.camera.upVec[2]))) + "\n";

		std::string forwardVec = "forwardVec:    " +
			(std::to_string(model.camera.forwardVec[0]) + " " +
			(std::to_string(model.camera.forwardVec[1])) + " " +
			(std::to_string(model.camera.forwardVec[2]))) + "\n";

		std::string nearPlane = "nearPlane:    " +
			(std::to_string(model.camera.nearPlane) + "\n"); 


		std::string farPlane = "farPlane:    " +
			(std::to_string(model.camera.farPlane) + "\n");

		std::string FOV = "FOV:    " +
			(std::to_string(model.camera.FOV) + "\n");



		//OutputDebugStringA((vtxNr + vtxPos + uv + normal + biNormal + tangent + pos + upVec + forwardVec + nearPlane + farPlane + FOV).c_str());
	}
	//OutputDebugStringA(std::to_string(model.material.Diffuse[1]).c_str());
}

void Model::draw(DirectX::XMMATRIX& viewProjMtx)
{
	this->m_material.upd(this->m_deviceContextPtr);

	UINT vertexOffset = 0;
	this->m_deviceContextPtr->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), this->m_vertexBuffer.getStridePointer(), &vertexOffset);
	this->m_deviceContextPtr->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	
	if (this->m_drawWithIndex)
		this->m_deviceContextPtr->DrawIndexed(this->m_indexBuffer.getSize(), 0, 0);
	else
		this->m_deviceContextPtr->Draw(this->m_vertexBuffer.getSize(), 0);

}