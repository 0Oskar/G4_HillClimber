#pragma once

struct SceneBFF
{
	unsigned int nrOfTextures;
	unsigned int nrOfLights;
	unsigned int nrOfCameras;
	unsigned int nrOfVertexAnimFrames;
	unsigned int nrOfBlendShapes;
};

struct MeshBFF
{
	char name[64];
	unsigned int nrOfControlPoints;
	unsigned int nrOfMaterials;
	unsigned int nrJoints;
};

struct MaterialBFF
{
	float Diffuse[3];
	float Ambient[3];
	float Opacity;
};

struct TextureBFF
{
	int parentMaterialIndex;
	static const int PATH_SIZE = 128;
	char texturePath[PATH_SIZE];
	char textureType[PATH_SIZE];
};

struct ControlPointBFF
{
	float pos[3];
	float uv[2];

	float norm[3];
	float biNorm[3];
	float tan[3];

	//int controlPointIndex;
	int influencedByJoints[4];
	float boneWeight[4];
};

struct LightBFF
{
	char type[64];
	float color[3];
	float dir;
	float intencity;
	float translation[4];
	float rotation[4];
	float scale[4];
};

struct CameraBFF
{
	float pos[3];
	float upVec[3];
	float forwardVec[3];
	float nearPlane;
	float farPlane;
	float FOV;
};

struct BlendshapeBFF
{
	float pos[3];
	float norm[3];
};

struct keyFrameBFF
{
	int timestamp;
	float pose[9];
};

struct JointBFF
{
	int jointIndex;
	int parentIndex;
	float bindPoseMatrix[16]; //4x4 matrix

	int nrOfKeyFrames;
	
	std::vector<keyFrameBFF> animationFrames;
};


