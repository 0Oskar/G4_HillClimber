#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../FbxDemoBase/FbxDemo/ImporterStructure.h"

#include <vector>
#include <DirectXMathVector.inl>
#include <d3d11.h>


class BiReader
{
private:
	std::string fileName = "../../FbxDemoBase/biFile.bff";
	std::ifstream inputFile;

	Mesh mesh;
	Vertex* vertexArr;


public:
	BiReader(std::string path);
	~BiReader();

	// Test
	void printToConsole();

	// Getters
	
	std::vector<Vertex> VertexAsVector;


};
