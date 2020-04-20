#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

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


class FileWrite
{

private:
	std::string fileName;
	std::ofstream outputFile;

	const int MAX_STRING_LEN = 26;


public:
	FileWrite(std::string path);
	~FileWrite();

	void writeToFile(const char* input, size_t size);
	void writeToStringFile(std::string input);
	void EmptyFile();


};
