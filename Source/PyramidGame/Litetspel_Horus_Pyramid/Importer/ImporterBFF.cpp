#include "pch.h"
#include "ImporterBFF.h"

namespace ImporterBFF
{
	//Model::Model(const char* MeshFileName)
	//{
	//	addToListOfModels();
	//	myHiddenFunc();
	//	std::ifstream MeshFile(MeshFileName, std::ifstream::binary);

	//	MeshFile.read((char*)&this->mesh, sizeof(MeshBFF));

	//	this->vertexArr = new VertexBFF[this->mesh.nrOfVertex];

	//	MeshFile.read((char*)this->vertexArr, this->mesh.nrOfVertex * sizeof(VertexBFF));
	//	MeshFile.read((char*)&this->material, sizeof(MaterialBFF));
	//	MeshFile.close();
	//}

	//std::string Model::MeshName(Model model)
	//{
	//	return std::to_string(this->material.Diffuse[0]);
	//}

	//Hidden
	void addToListOfModels()
	{

	}

	void myHiddenFunc()
	{
		
	}

	Manager::Manager()
	{

	}

	Manager& Manager::GetInstance()
	{
		static Manager instance = Manager();
		return instance;
	}

	Model Manager::LoadModel(const char* filePath)
	{
		return Model();
	}

	Manager::~Manager()
	{
	}
}

