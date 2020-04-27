#pragma once
#include "Model.h"

namespace ImporterBFF
{
	class Manager
	{
	private:
		Manager();
			
		std::map<std::string, Model> map; // name on mesh, type of object
			
			
			

	public:
		static Manager& GetInstance();
		Manager(Manager const&) = delete;
		void operator =(Manager const&) = delete;

		bool LoadModel(const char* filePath);

		


		~Manager();
		//Model(const char* MeshFileName); // load
		//std::string MeshName(Model model);
	};
}
