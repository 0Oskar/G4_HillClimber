#pragma once
#include "pch.h"
#include "framework.h"

class ModelBFF
{
	public:
		SceneBFF scene;
		MeshBFF mesh;
		ControlPointBFF* controllPointsArr;
		MaterialBFF material;
		TextureBFF texture;
		LightBFF light;
		CameraBFF camera;
		VertexAnimBFF vertexAnim;
		BlendShapesBFF blendShapes;
};