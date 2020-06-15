#pragma once
#include <vector>

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
	unsigned int nrOfVertex;
	unsigned int nrOfControlPoints;
	unsigned int nrOfMaterials;
	unsigned int nrJoints;
};

struct MaterialBFF
{
	float Diffuse[3];
	float DiffuseFactor;

	float Ambient[3];
	float AmbientFactor;

	float TransparencyColor[3];
	float Transparency;

	float Emissive[3];
	float EmissiveFactor;

	float NormalMap[3];

	float Bump;
	float BumpFactor;


	float DisplacementColor[3];
	float DisplacementFactor;

	float VectorDisplacementColor[3];
	float VectorDisplacementFactor;

	int nrOfTexturesConnected;
	std::vector<int> connectedToTextureIndex;
};

struct TextureBFF
{
	int textureIndex;
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

	unsigned int controlPointIndex;

	int influencedByJoints[4];
	float boneWeight[4];
};

struct IndexListBFF
{
	unsigned int index;
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

struct KeyFrameBFF
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
	std::vector<KeyFrameBFF> animationFrames;
};

