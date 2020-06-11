#include "pch.h"
#include "ImporterBFF.h"

namespace ImporterBFF
{
	//Hidden
	ModelBFF LoadModelFromFile(const char* filePath)
	{
		ModelBFF model;
		std::ifstream MeshFile(filePath, std::ifstream::binary);


		MeshFile.read((char*)&model.scene, sizeof(SceneBFF));
		MeshFile.read((char*)&model.mesh, sizeof(MeshBFF));
		MeshFile.read((char*)&model.material, model.mesh.nrOfMaterials * sizeof(MaterialBFF));
		MeshFile.read((char*)&model.texture, model.scene.nrOfTextures * sizeof(TextureBFF));
	
		model.controllPointsArr = new ControlPointBFF[model.mesh.nrOfControlPoints];
		MeshFile.read((char*)model.controllPointsArr, model.mesh.nrOfControlPoints * sizeof(ControlPointBFF));
		// Flippa X axeln och V flr uv
		for (size_t i = 0; i < model.mesh.nrOfControlPoints; i++)
		{
			model.controllPointsArr[i].pos[2] *= -1;
			model.controllPointsArr[i].norm[2] *= -1;
			model.controllPointsArr[i].uv[1] *= -1;
		}
		//MeshFile.read((char*)&model.vertexAnim, model.scene.nrOfVertexAnimFrames * sizeof(VertexAnimBFF));
		
		MeshFile.read((char*)&model.light, model.scene.nrOfLights * sizeof(LightBFF));
		MeshFile.read((char*)&model.camera, model.scene.nrOfCameras * sizeof(CameraBFF));
		MeshFile.read((char*)&model.blendShapes, model.scene.nrOfBlendShapes * sizeof(BlendShapesBFF));
		
		
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

