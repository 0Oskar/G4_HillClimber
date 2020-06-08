#pragma once

struct SceneBFF
{
	unsigned int nrOfLights;
	unsigned int nrOfCameras;
	unsigned int nrOfVertexAnimFrames;
	unsigned int nrOfBlendShapes;
};

struct MeshBFF
{
	char name[64];
	unsigned int nrOfControlPoints;
	unsigned int nrJoints;
	
};

struct ControlPointBFF
{
	float pos[3];
	float uv[2];

	float norm[3];
	float biNorm[3];
	float tan[3];

	//int controlPointIndex;
	int boneID[4];
	float boneWeight[4];
};

struct JointBFF
{
	int index;
	float matrix[16]; //4x4 matrix
};

struct BlendshapeBFF
{
	float pos[3];
	float norm[3];
};























struct MaterialBFF
{
	float Diffuse[3];
	float Ambient[3];
	float Opacity;
	char texturePath[128];
	char textureType[128];
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

// Tristans
struct VertexAnimBFF
{
	int time;
	float pos[3];
	float rot[3];
	float scale[3];
};

struct BlendShapesBFF
{
	float pos[3];
	float norm[3];
};

struct BonesBFF
{
	char name[64];
	float bindRot[3];
};

