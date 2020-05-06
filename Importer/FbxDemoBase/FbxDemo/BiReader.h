#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>


struct ReaderMesh
{
	char name[64];
	unsigned int nrOfVertex;

	float pos[3];
	float uv[2];
	float norm[3];
	float biNorm[3];
	float tan[3];
};

struct Temp
{
	char name[64];
	unsigned int nrOfVertex;
};


class BiReader
{
private:
	std::string fileName = "Meshes/biFile.bff";
	std::ifstream inputFile;

	int sizeOfFile;

	ReaderMesh mesh;



	FILE* in;
	Temp a;

	char buffer[100];



public:
	BiReader(std::string path);
	~BiReader();

	//std::string getMeshName();

	void printToConsole();


};
