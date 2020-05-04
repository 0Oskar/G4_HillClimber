#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "../../FbxDemoBase/FbxDemo/ImporterStructure.h"
#include <vector>




class BiReader
{
private:
	std::string fileName = "../../FbxDemoBase/biFile.bff";
	std::ifstream inputFile;

	Mesh mesh;
	Vertex *vertexArr;
	Material material;

public:
	BiReader(std::string path);
	~BiReader();

	// Test
	void printFromMainToConsole();
	
	// Getters
	//std::vector<Vertex> *getVertexPositions(std::vector<Vertex> *someVerticies);
	unsigned int getNrOfVertex();
	void getVertexPositions(Vertex* someVerticies);
	Material getMaterial();
};
