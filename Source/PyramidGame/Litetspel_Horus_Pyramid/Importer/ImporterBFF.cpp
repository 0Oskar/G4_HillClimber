#include "pch.h"
#include "ImporterBFF.h"

namespace ImporterBFF
{
	Importer::Model* Importer::LoadMesh(std::string MeshFileName, std::string TextureFileName)
	{
		std::ifstream MeshFile(MeshFileName, std::ifstream::binary);

		MeshFile.read((char*)model, sizeof(Model));



		return nullptr;
	}
	std::string ImporterBFF::Importer::MeshName()
	{
		return "hej";
	}

}

