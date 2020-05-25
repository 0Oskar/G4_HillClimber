#pragma once
#include "pch.h"
#include "framework.h"

class ModelBFF
{
	public:
		SceneBFF scene;
		MeshBFF mesh;
		VertexBFF* vertexArr;
		MaterialBFF material;
		LightBFF light;
		CameraBFF camera;
		VertexAnimBFF vertexAnim;
		BlendShapesBFF blendShapes;
};