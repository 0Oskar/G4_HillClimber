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
