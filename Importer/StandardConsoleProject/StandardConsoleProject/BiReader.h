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
	void printFromMainToConsole(Vertex* theVertex);
	unsigned int getNrOfVertex();
	
	// Getters
	//std::vector<Vertex> *getVertexPositions(std::vector<Vertex> *someVerticies);
	void getVertexPositions(Vertex* someVerticies);
};
