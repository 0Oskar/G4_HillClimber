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
	unsigned int nrOfVertex;
	unsigned int nrJoints;
};

struct VertexBFF
{
	float pos[3];
	float uv[2];
	float norm[3];
	float biNorm[3];
	float tan[3];
};

struct MaterialBFF
{
	float Diffuse[3];
	float Ambient[3];
	float Opacity;
	char texturePath[128];
};

struct LightBFF
{
	char type[64];
	float color[3];
	float dir;
	float intencity;
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

// Tristans -----------------------------
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
// ------------------------------------------
struct BonesBFF
{
	char name[64];
	float bindRot[3];
};

struct AnimVertexBFF
{
	float pos[3];
	float uv[2];
	float norm[3];
	float biNorm[3];
	float tan[3];
	int boneIDs; // Vi kör med en istället för fyra
	float boneWeights; // 0-1
	JointBFF jointHierarchy;
};

struct JointBFF
{
	//joint children[];
	int id; // index
	float transform[16]; // make into 4x4 Matrix
};

//
struct AnimationBFF
{
	KeyframeBFF frames[];
};

struct KeyframeBFF 
{
	JointTransform transforms[999]; // antal baserat på mängden joints
	float timeStamp;
};

struct JointTransform 
{
	float position[3];
	float rotation[3]; // Quaternion
	float scale[3]; // maybe
};


