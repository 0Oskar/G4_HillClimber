#pragma once
#include "pch.h"
#include "framework.h"
#include <DirectXCollision.h>

class ModelBFF
{
	public:
		MeshBFF mesh;
		VertexBFF* vertexArr;
		MaterialBFF material;
		CameraBFF camera;
		std::string fileName;
		bool loaded = false;

		bool loadSucceded() {
			return mesh.nrOfVertex != 0;
		}
};