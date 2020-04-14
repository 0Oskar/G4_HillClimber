#pragma once
#include <string>
#include <iostream>
#include <fstream>

<<<<<<< HEAD
struct Vertex
=======
struct VertexStruct
>>>>>>> c1617e2b5d486fb9146c658f18f9294c2030a821
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

<<<<<<< HEAD
	void writeToFile(const char* input, size_t size);
	void EmptyFile();


=======
	void writeToFile(char* data);
	void EmptyFile();



	//VertexStruct myVertexStruct;

>>>>>>> c1617e2b5d486fb9146c658f18f9294c2030a821
};
