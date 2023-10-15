#include "pch.h"
#include "Model.h"
#include "PyramidMathTools.h"

void Model::loadOBJModel(uint32_t asyncThreadIndex)
{	
	// Values Contained
	bool hasNormals = false;
	bool hasTexcoords = false;

	// Vertices
	vector<DirectX::XMFLOAT3> vertexPositions;
	vector<DirectX::XMFLOAT3> vertexNormals;
	vector<DirectX::XMFLOAT2> vertexTexcoords;

	// Faces
	vector<UINT> vertexPosIndices;
	vector<UINT> vertexNormIndices;
	vector<UINT> vertexTexIndices;

	// Material
	string materialFilePath = "";

	// Bounding Box
	XMFLOAT3 minExtends;
	XMFLOAT3 maxExtends;
	DirectX::XMStoreFloat3(&minExtends, g_XMFltMax);
	DirectX::XMStoreFloat3(&maxExtends, g_XMFltMin);

	// File Stream
	stringstream sStream;
	ifstream inFileStream(MODEL_PATH + m_modelPath, ios::in);
	string line = "";
	string prefix = "";
	DirectX::XMFLOAT3 tempF3;
	DirectX::XMFLOAT2 tempF2;
	UINT tempUINT;
	bool fileFound = true;

	if (!inFileStream.is_open())
	{
		fileFound = false;
		MaterialData mat;
		mat.ambient = { .2f, .2f, .2f};
		mat.globalAmbientContribution = 1.f;
		mat.diffuse = { .9f, .9f, .9f, 1.f };
		mat.specular = { 1.f, 1.f, 1.f, 1.f };
		mat.shininess = 32;
		
		// Load error model and material
		inFileStream.open(string(MODEL_PATH) + ERROR_MODEL, ios::in);
		//m_material.setMaterial(mat);
		m_material.setTexture((wstring(TEXTURE_PATH) + ERROR_TEXTURE).c_str());
	}

	while (getline(inFileStream, line))
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
			minExtends = pMath::F3Min(tempF3, minExtends);
			maxExtends = pMath::F3Max(tempF3, maxExtends);
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

	m_vertices.resize(vertexPosIndices.size(), Vertex());

	//Load in all indices
	for (size_t i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i].position = vertexPositions[vertexPosIndices[i] - 1];
		if (vertexTexcoords.size())
			m_vertices[i].textureCoord = vertexTexcoords[vertexTexIndices[i] - 1];

		if (vertexNormals.size())
			m_vertices[i].normal = vertexNormals[vertexNormIndices[i] - 1];
	}

	m_vertexBuffer.initialize(m_devicePtr, m_vertices.data(), (int)m_vertices.size());
	
	// Setup Bounding Box
	m_aabb.Extents = pMath::F3Multiply(pMath::F3Subtract(maxExtends, minExtends), 0.5f);
	m_aabb.Center = pMath::F3Subtract(maxExtends, m_aabb.Extents);

	string loadedMessage;
	if (asyncThreadIndex != UINT_MAX)
	{
		loadedMessage = "	|";
		for (uint32_t i = 0; i < asyncThreadIndex; i++)
			loadedMessage += INDENT_CHAR + "|";
		std::string content = "";
		if (fileFound)
			content += " D:" + m_modelPath;
		else
			content += " F:" + m_modelPath;

		loadedMessage += content;
		loadedMessage += std::string(INDENT_CHAR_LENGTH - content.length(), ' ') + "|";

		for (uint32_t i = asyncThreadIndex; i < MODEL_IMPORT_THREAD_COUNT - 1; i++)
			loadedMessage += INDENT_CHAR + "|";
		loadedMessage += "\n";
	}
	else
	{
		if (fileFound)
			loadedMessage += "OBJ model loaded: " + m_modelPath + "\n";
		else
			loadedMessage += "OBJ model could not be loaded: " + m_modelPath + "\n";
	}
	
	OutputDebugStringA(loadedMessage.c_str());
}

void Model::loadBffModel(uint32_t asyncThreadIndex)
{
	ModelBFF myModel;
	string loadedMessage;

	// Bounding Box
	XMFLOAT3 minExtends;
	XMFLOAT3 maxExtends;
	DirectX::XMStoreFloat3(&minExtends, g_XMFltMax);
	DirectX::XMStoreFloat3(&maxExtends, g_XMFltMin);

	if (asyncThreadIndex != UINT_MAX)
	{
		loadedMessage = "	|";
		for (uint32_t i = 0; i < asyncThreadIndex; i++)
			loadedMessage += INDENT_CHAR + "|";
		
		std::string content = " D: " + m_modelPath;
		loadedMessage += content;
		loadedMessage += std::string(INDENT_CHAR_LENGTH - content.length(), ' ') + "|";

		myModel = m_myManager->LoadModelAsync(m_modelPath.c_str());

		for (uint32_t i = asyncThreadIndex; i < MODEL_IMPORT_THREAD_COUNT - 1; i++)
			loadedMessage += INDENT_CHAR + "|";
		loadedMessage += "\n";
	}
	else {
		loadedMessage = "BFF Model loaded: " + m_modelPath + "\n";
		myModel = m_myManager->LoadModel(m_modelPath.c_str());
	}
	if (!myModel.loadSucceded())
	{
		loadOBJModel(); // Will trigger error model load
		return;
	}
	m_vertices.resize(myModel.mesh.nrOfVertex);
	for (unsigned int i = 0; i < myModel.mesh.nrOfVertex; i++)
	{
		m_vertices[i].position = XMFLOAT3(myModel.vertexArr[i].pos);
		m_vertices[i].normal = XMFLOAT3(myModel.vertexArr[i].norm);
		m_vertices[i].textureCoord = XMFLOAT2(myModel.vertexArr[i].uv);

		minExtends = pMath::F3Min(m_vertices[i].position, minExtends);
		maxExtends = pMath::F3Max(m_vertices[i].position, maxExtends);
	}

	m_vertexBuffer.initialize(m_devicePtr, m_vertices.data(), (int)m_vertices.size());

	// Setup Bounding Box
	m_aabb.Extents = pMath::F3Multiply(pMath::F3Subtract(maxExtends, minExtends), 0.5f);
	m_aabb.Center = pMath::F3Subtract(maxExtends, m_aabb.Extents);

	//printBffModel(myModel); //For testing
	OutputDebugStringA(loadedMessage.c_str());
}

Model::Model()
{
	m_deviceContextPtr = nullptr;
	m_devicePtr = nullptr;
	m_drawWithIndex = false;
	m_loaded = false;
	m_myManager = &ImporterBFF::Manager::GetInstance();
}

void Model::initForAsyncLoad(ID3D11Device* device, ID3D11DeviceContext* dContext, const char* modelFilePath, const MaterialData material, const wchar_t* texturePath)
{
	// Device
	m_devicePtr = device;
	m_deviceContextPtr = dContext;

	// Mesh
	m_modelPath = modelFilePath;
	string fileFormat = m_modelPath.substr(m_modelPath.find(".") + 1);
	if (fileFormat == "obj")
		m_format = ModelFormat::ModelFormat_obj;
	else if (fileFormat == "bff")
	{
		m_format = ModelFormat::ModelFormat_bff;
		m_myManager->InitModelAsync(m_modelPath.c_str());
	}
	else
		m_format = ModelFormat::ModelFormat_custom;

	// Material
	if (texturePath != L"")
	{
		wstring path = (wstring(TEXTURE_PATH) + texturePath);
		m_material.init(device, dContext, material, path.c_str());
		m_texturePath = path;
	}
	else
	{
		m_material.init(device, dContext, material);
	}
}

void Model::loadModelAsync(uint32_t asyncThreadIndex)
{
	string loadedMessage = "	|";
	for (uint32_t i = 0; i < asyncThreadIndex; i++)
		loadedMessage += INDENT_CHAR + "|";

	std::string content = " S: " + m_modelPath;
	loadedMessage += content;
	loadedMessage += std::string(INDENT_CHAR_LENGTH - content.length(), ' ') + "|";

	for (uint32_t i = asyncThreadIndex; i < MODEL_IMPORT_THREAD_COUNT - 1; i++)
		loadedMessage += INDENT_CHAR + "|";
	loadedMessage += "\n";

	OutputDebugStringA(loadedMessage.c_str());
	switch (m_format)
	{
		case ModelFormat::ModelFormat_obj: loadOBJModel(asyncThreadIndex); break;
		case ModelFormat::ModelFormat_bff: loadBffModel(asyncThreadIndex); break;
	}

	m_loaded = true;
}

void Model::loadModel(ID3D11Device* device, ID3D11DeviceContext* dContext, const char* modelFilePath, const MaterialData material, const wchar_t* texturePath)
{
	// Device
	m_devicePtr = device;
	m_deviceContextPtr = dContext;

	// Material
	if (texturePath != L"")
	{
		wstring path = (wstring(TEXTURE_PATH) + texturePath);
		m_material.init(device, dContext, material, path.c_str());
		m_texturePath = path;
	}
	else
	{
		m_material.init(device, dContext, material);
	}

	// Mesh
	m_modelPath = modelFilePath;
	string fileFormat = m_modelPath.substr(m_modelPath.find(".") + 1);
	if (fileFormat == "obj") {
		m_format = ModelFormat::ModelFormat_obj;
		loadOBJModel();
	}
	else if (fileFormat == "bff")
	{
		m_format = ModelFormat::ModelFormat_obj;
		loadBffModel();
	}
	else
	{
		m_format = ModelFormat::ModelFormat_custom;
	}
	m_loaded = true;
}

void Model::initQuadModel(ID3D11Device* device, ID3D11DeviceContext* dContext, MaterialData material, wstring texturePath, bool flipped)
{
	m_devicePtr = device;
	m_deviceContextPtr = dContext;
	vector<Vertex> vertices
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
	if (flipped)
	{
		for (uint32_t i = 0; i < 4; i++)
		{
			XMVECTOR position = XMLoadFloat3(&vertices[i].position);
			position = XMVectorScale(position, -1.f);
			XMStoreFloat3(&vertices[i].position, position);

			XMVECTOR normal = XMLoadFloat3(&vertices[i].normal);
			normal = XMVectorScale(normal, -1.f);
			XMStoreFloat3(&vertices[i].normal, normal);

			vertices[i].textureCoord.x = 1.f - vertices[i].textureCoord.x;
			vertices[i].textureCoord.y = 1.f - vertices[i].textureCoord.y;
		}
	}
	m_vertexBuffer.initialize(m_devicePtr, vertices.data(), (int)vertices.size());

	DWORD indicies[] =
	{
		0, 1, 2,
		0, 2, 3
	};
	m_drawWithIndex = true;
	m_indexBuffer.init(m_devicePtr, indicies, ARRAYSIZE(indicies));

	if (texturePath != L"")
	{
		wstring path = (TEXTURE_PATH + texturePath);
		m_material.init(device, dContext, material, path.c_str());
		m_texturePath = path;
	}
	else
		m_material.init(device, dContext, material);

	m_format = ModelFormat::ModelFormat_custom;
	m_loaded = true;
}

void Model::initModelFromData(ID3D11Device* device, ID3D11DeviceContext* dContext, vector<Vertex> vertexVector, MaterialData material, wstring texturePath)
{
	m_devicePtr = device;
	m_deviceContextPtr = dContext;

	m_vertexBuffer.initialize(m_devicePtr, vertexVector.data(), (int)vertexVector.size());
	
	m_vertices = vertexVector;

	if (texturePath != L"")
	{
		wstring path = (TEXTURE_PATH + texturePath);
		m_material.init(device, dContext, material, path.c_str());
		m_texturePath = path;
	}
	else
	{
		m_material.init(device, dContext, material);
	}

	m_format = ModelFormat::ModelFormat_custom;
	m_loaded = true;
}

void Model::printBffModel(ModelBFF model) const
{
	for (unsigned int i = 0; i < model.mesh.nrOfVertex; i++)
	{
		string vtxNr = "\nvtx:		" + to_string(i) + "\n";

		string vtxPos = "Pos:		" + 
			(to_string(model.vertexArr[i].pos[0]) + " " +
			(to_string(model.vertexArr[i].pos[1])) + " " +
			(to_string(model.vertexArr[i].pos[2]))) + "\n";

		string uv = "uv:			" + 
			(to_string(model.vertexArr[i].uv[0]) + " " +
			(to_string(model.vertexArr[i].uv[1]))) + "\n";

		string normal = "Normal:		" + 
			(to_string(model.vertexArr[i].norm[0]) + " " +
			(to_string(model.vertexArr[i].norm[1])) + " " +
			(to_string(model.vertexArr[i].norm[2]))) + "\n";

		string biNormal = "Binormal:	" + 
			(to_string(model.vertexArr[i].biNorm[0]) + " " +
			(to_string(model.vertexArr[i].biNorm[1])) + " " +
			(to_string(model.vertexArr[i].biNorm[2]))) + "\n";

		string tangent = "Tan:		" + 
			(to_string(model.vertexArr[i].tan[0]) + " " +
			(to_string(model.vertexArr[i].tan[1])) + " " +
			(to_string(model.vertexArr[i].tan[2]))) + "\n";



		string pos = "Pos:    "+
			(to_string(model.camera.pos[0]) + " " +
			(to_string(model.camera.pos[1])) + " " +
			(to_string(model.camera.pos[2]))) + "\n";

		string upVec = "upVec:    " +
			(to_string(model.camera.upVec[0]) + " " +
			(to_string(model.camera.upVec[1])) + " " +
			(to_string(model.camera.upVec[2]))) + "\n";

		string forwardVec = "forwardVec:    " +
			(to_string(model.camera.forwardVec[0]) + " " +
			(to_string(model.camera.forwardVec[1])) + " " +
			(to_string(model.camera.forwardVec[2]))) + "\n";

		string nearPlane = "nearPlane:    " +
			(to_string(model.camera.nearPlane) + "\n"); 


		string farPlane = "farPlane:    " +
			(to_string(model.camera.farPlane) + "\n");

		string FOV = "FOV:    " +
			(to_string(model.camera.FOV) + "\n");



		//OutputDebugStringA((vtxNr + vtxPos + uv + normal + biNormal + tangent + pos + upVec + forwardVec + nearPlane + farPlane + FOV).c_str());
	}
	//OutputDebugStringA(to_string(model.material.Diffuse[1]).c_str());
}

bool Model::is_loaded() const
{
	return m_loaded;
}

void Model::draw()
{
	if (!m_loaded)
		return;

	m_material.upd(m_deviceContextPtr);

	UINT vertexOffset = 0;
	m_deviceContextPtr->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.getStridePointer(), &vertexOffset);
	m_deviceContextPtr->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	
	if (m_drawWithIndex)
		m_deviceContextPtr->DrawIndexed(m_indexBuffer.getSize(), 0, 0);
	else
		m_deviceContextPtr->Draw(m_vertexBuffer.getSize(), 0);

}