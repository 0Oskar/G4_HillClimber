#pragma once

struct Mesh
{
	char name[64];
	unsigned int nrOfVertex;
};

struct Vertex
{
	float pos[3];
	float uv[2];
	float norm[3];
	float biNorm[3];
	float tan[3];
};
