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

public:
	BiReader(std::string path);
	~BiReader();

	// Test
	void printToConsole();

	// Getters
	std::vector<Vertex> *getVertexPositions();
};
