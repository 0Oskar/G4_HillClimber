#pragma once
#include "pch.h"
#include "framework.h"

class ModelBFF
{
	public:
		SceneBFF scene;
		MeshBFF mesh;
		std::vector<ControlPointBFF> controllPointsArr;
		std::vector<IndexListBFF> indexList;
		std::vector<MaterialBFF> material;
		std::vector<TextureBFF> texture;
		std::vector<LightBFF> light;
		std::vector<CameraBFF> camera;
		std::vector<BlendshapeBFF> blendShapes;
		std::vector<KeyFrameBFF> keyframes;
		std::vector<JointBFF> joints;

};