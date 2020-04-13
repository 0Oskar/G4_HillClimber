#pragma once
#include <string>
#include <iostream>
#include <fstream>

struct VertexStruct
{
	float pos[3];
	float uv[2];
	float norm[3];
};

class FileWrite
{

private:
	std::string fileName;
	std::ofstream outputFile;

	const int MAX_STRING_LEN = 26;

	

	

public:
	FileWrite();
	~FileWrite();

	void writeToFile(char* data);
	void EmptyFile();



	//VertexStruct myVertexStruct;

};
