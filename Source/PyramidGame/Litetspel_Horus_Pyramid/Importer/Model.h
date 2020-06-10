#pragma once
#include "pch.h"
#include "framework.h"

class ModelBFF
{
	public:
		SceneBFF scene;
		MeshBFF mesh;
		MaterialBFF material;
		LightBFF light;
		CameraBFF camera;
		BlendShapesBFF blendShapes;

		VertexAnimBFF vertexAnim;
		VertexBFF* vertexArr;
};