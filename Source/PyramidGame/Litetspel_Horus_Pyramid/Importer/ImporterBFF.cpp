#include "pch.h"
#include "ImporterBFF.h"

namespace ImporterBFF
{
	//Hidden
	ModelBFF LoadModelFromFile(const char* filePath)
	{
		ModelBFF model;

		std::ifstream MeshFile(filePath, std::ifstream::binary);

		MeshFile.read((char*)&model.mesh, sizeof(MeshBFF));

		model.vertexArr = new VertexBFF[model.mesh.nrOfVertex];

		MeshFile.read((char*)model.vertexArr, model.mesh.nrOfVertex * sizeof(VertexBFF));
		// Flippa X axeln och V flr uv
		for (size_t i = 0; i < model.mesh.nrOfVertex; i++)
		{
			model.vertexArr[i].pos[2] *= -1;
			model.vertexArr[i].norm[2] *= -1;
			model.vertexArr[i].uv[1] *= -1;
		}
		MeshFile.read((char*)&model.material, sizeof(MaterialBFF));
		MeshFile.read((char*)&model.camera, sizeof(CameraBFF));

		MeshFile.close();

		return model;
	}


	//std::string Model::MeshName(Model model)
	//{
	//	return std::to_string(this->material.Diffuse[0]);
	//}

	Manager::Manager()
	{

	}

	Manager& Manager::GetInstance()
	{
		static Manager instance;
		return instance;
	}

	const ModelBFF& Manager::LoadModel(const char* fileName)
	{
		// Check if insertion is successful or not
		if (map.find(fileName) == map.end())
		{
			std::string fullFilePath = "../../../../Models/" + (std::string)fileName;

			ModelBFF someModel;
			someModel = LoadModelFromFile(fullFilePath.c_str());

			//Ladda in
			map[fileName] = someModel; //add to list
		}

		return map[fileName];
	}

	Manager::~Manager()
	{
	}



}

