#pragma once
#include "Model.h"

namespace ImporterBFF
{
	class Manager
	{
	private:
		Manager();
			
		std::map<std::string, ModelBFF> map; // name on mesh, type of object
			
			

	public:
		static Manager& GetInstance();
		Manager(Manager const&) = delete;
		void operator =(Manager const&) = delete;
		/*
		The first call adds mesh to the manager, call again for a pointer to mesh. This is the only function needed.
		*/
		const ModelBFF& LoadModel(const char* fileName);

		


		~Manager();
		//Model(const char* MeshFileName); // load
		//std::string MeshName(Model model);
	};
}
