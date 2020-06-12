#pragma once
#include "pch.h"
#include "framework.h"

class ModelBFF
{
	public:
		SceneBFF scene;
		MeshBFF mesh;
		std::vector<ControlPointBFF> controllPointsArr;
		MaterialBFF material;
		TextureBFF texture;
		std::vector<LightBFF> light;
		std::vector<CameraBFF> camera;
		BlendshapeBFF blendShapes;
		KeyFrameBFF keyframes;
		JointBFF joints;

};