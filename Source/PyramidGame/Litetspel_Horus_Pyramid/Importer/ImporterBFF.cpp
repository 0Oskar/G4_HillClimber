#include "pch.h"
#include "ImporterBFF.h"

namespace ImporterBFF
{
	//Hidden
	ModelBFF LoadModelFromFile(const char* filePath)
	{
		ModelBFF model;
		std::ifstream MeshFile(filePath, std::ifstream::binary);
		

	/*
	Scene
	Mesh
	Material
	Texture
	ControlPoints
	Lights
	Camera
	BlendShape
	Joints
	*/

		MeshFile.read((char*)&model.scene, sizeof(SceneBFF));

		MeshFile.read((char*)&model.mesh, sizeof(MeshBFF));

		for (int m = 0; m < model.mesh.nrOfMaterials; m++)
		{
			MaterialBFF mat;
			//MeshFile.read((char*)&mat, sizeof(MaterialBFF));
			MeshFile.read((char*)&mat.Diffuse[0], sizeof(float));
			MeshFile.read((char*)&mat.Diffuse[1], sizeof(float));
			MeshFile.read((char*)&mat.Diffuse[2], sizeof(float));
			MeshFile.read((char*)&mat.DiffuseFactor, sizeof(float));

			MeshFile.read((char*)&mat.Ambient[0], sizeof(float));
			MeshFile.read((char*)&mat.Ambient[1], sizeof(float));
			MeshFile.read((char*)&mat.Ambient[2], sizeof(float));
			MeshFile.read((char*)&mat.AmbientFactor, sizeof(float));

			MeshFile.read((char*)&mat.TransparencyColor[0], sizeof(float));
			MeshFile.read((char*)&mat.TransparencyColor[1], sizeof(float));
			MeshFile.read((char*)&mat.TransparencyColor[2], sizeof(float));
			MeshFile.read((char*)&mat.Transparency, sizeof(float));

			MeshFile.read((char*)&mat.Emissive[0], sizeof(float));
			MeshFile.read((char*)&mat.Emissive[1], sizeof(float));
			MeshFile.read((char*)&mat.Emissive[2], sizeof(float));
			MeshFile.read((char*)&mat.EmissiveFactor, sizeof(float));

			MeshFile.read((char*)&mat.NormalMap[0], sizeof(float));
			MeshFile.read((char*)&mat.NormalMap[1], sizeof(float));
			MeshFile.read((char*)&mat.NormalMap[2], sizeof(float));

			MeshFile.read((char*)&mat.Bump, sizeof(float));
			MeshFile.read((char*)&mat.BumpFactor, sizeof(float));

			MeshFile.read((char*)&mat.DisplacementColor[0], sizeof(float));
			MeshFile.read((char*)&mat.DisplacementColor[1], sizeof(float));
			MeshFile.read((char*)&mat.DisplacementColor[2], sizeof(float));
			MeshFile.read((char*)&mat.DisplacementFactor, sizeof(float));

			MeshFile.read((char*)&mat.VectorDisplacementColor[0], sizeof(float));
			MeshFile.read((char*)&mat.VectorDisplacementColor[1], sizeof(float));
			MeshFile.read((char*)&mat.VectorDisplacementColor[2], sizeof(float));
			MeshFile.read((char*)&mat.VectorDisplacementFactor, sizeof(float));

			MeshFile.read((char*)&mat.nrOfTexturesConnected, sizeof(int));
			mat.connectedToTextureIndex.resize(mat.nrOfTexturesConnected);
			for (int c = 0; c < mat.nrOfTexturesConnected; c++)
			{
				MeshFile.read((char*)&mat.connectedToTextureIndex[c], sizeof(int));
			}

			model.material.push_back(mat);
		}
		
		for (int t = 0; t < model.scene.nrOfTextures; t++)
		{
			TextureBFF tex;
			MeshFile.read((char*)&tex, sizeof(TextureBFF));
			model.texture.push_back(tex);
		}



		//model.controllPointsArr.resize(model.mesh.nrOfControlPoints);
		for (int c = 0; c < model.mesh.nrOfControlPoints; c++)
		{
			ControlPointBFF ctrl;


			MeshFile.read((char*)&ctrl, sizeof(ControlPointBFF));
			model.controllPointsArr.push_back(ctrl);
			//Flip
			model.controllPointsArr.back().pos[2] *= -1;
			model.controllPointsArr.back().norm[2] *= -1;
			model.controllPointsArr.back().uv[1] *= -1;
		}






		for (int i = 0; i < model.mesh.nrOfVertex; i++)
		{
			IndexListBFF index;
			MeshFile.read((char*)&index, sizeof(IndexListBFF));
			model.indexList.push_back(index);
		}

		for (int l = 0; l < model.scene.nrOfLights; l++)
		{
			LightBFF light;
			MeshFile.read((char*)&light, sizeof(LightBFF));
			model.light.push_back(light);
		}

		for (int c = 0; c < model.scene.nrOfCameras; c++)
		{
			CameraBFF cam;
			MeshFile.read((char*)&cam, sizeof(CameraBFF));
			model.camera.push_back(cam);
		}

		for (int b = 0; b < model.scene.nrOfBlendShapes; b++)
		{
			BlendshapeBFF blend;
			MeshFile.read((char*)&blend, sizeof(BlendshapeBFF));
			model.blendShapes.push_back(blend);
		}

		for (int j = 0; j < model.mesh.nrJoints; j++)
		{
			JointBFF jnt;
			MeshFile.read((char*)&jnt.jointIndex, sizeof(int));
			MeshFile.read((char*)&jnt.parentIndex, sizeof(int));
			MeshFile.read((char*)&jnt.bindPoseMatrix[0], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[1], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[2], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[3], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[4], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[5], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[6], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[7], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[8], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[9], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[10], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[11], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[12], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[13], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[14], sizeof(float));
			MeshFile.read((char*)&jnt.bindPoseMatrix[15], sizeof(float));
			MeshFile.read((char*)&jnt.nrOfKeyFrames, sizeof(int));
			
			jnt.animationFrames.resize(jnt.nrOfKeyFrames);
			for (int k = 0; k < jnt.nrOfKeyFrames; k++)
			{
				MeshFile.read((char*)&jnt.animationFrames[k].timestamp, sizeof(int));
				MeshFile.read((char*)&jnt.animationFrames[k].pos[0], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].pos[1], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].pos[2], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].rot[0], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].rot[1], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].rot[2], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].scale[0], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].scale[1], sizeof(float));
				MeshFile.read((char*)&jnt.animationFrames[k].scale[2], sizeof(float));
			}
			model.joints.push_back(jnt);
		}
		int stop = 0;
		
		
		
		MeshFile.close();

		return model;
	}


	//std::string Model::MeshName(Model model)
	//{
	//	return std::to_string(this->material.Diffuse[0]);
	//}

	Manager::Manager()
	{

	}

	Manager& Manager::GetInstance()
	{
		static Manager instance;
		return instance;
	}

	const ModelBFF& Manager::LoadModel(const char* fileName)
	{
		// Check if insertion is successful or not
		if (map.find(fileName) == map.end())
		{
			std::string fullFilePath = "../../../../Models/" + (std::string)fileName;

			ModelBFF someModel;
			someModel = LoadModelFromFile(fullFilePath.c_str());

			//Ladda in
			map[fileName] = someModel; //add to list
		}

		return map[fileName];
	}

	Manager::~Manager()
	{
	}



}

