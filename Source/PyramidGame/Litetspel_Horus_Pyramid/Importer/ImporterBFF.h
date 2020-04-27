#pragma once
#include "Model.h"

namespace ImporterBFF
{
	class Manager
	{
	private:
		Manager();

	public:
		static Manager& GetInstance();
		Model LoadModel(const char* filePath);
		~Manager();
		//Model(const char* MeshFileName); // load
		//std::string MeshName(Model model);
	};
}
