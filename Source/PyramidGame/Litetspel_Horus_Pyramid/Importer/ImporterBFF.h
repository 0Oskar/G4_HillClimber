#pragma once

namespace ImporterBFF
{
	class Importer
	{
	private:
		std::string TestName = "MyMesh";

		struct Model 
		{
			Mesh mesh;
			Vertex *vertexArr;
			Material material;
		};

		Model model;


	public:

		static Model* LoadMesh(std::string MeshFileName, std::string TextureFileName);


		static std::string MeshName();





	};
}
