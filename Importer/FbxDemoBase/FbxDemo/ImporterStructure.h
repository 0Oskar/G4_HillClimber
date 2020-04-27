#pragma once

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
