#include "BiReader.h"

BiReader::BiReader(std::string path)
{
	this->fileName = path;

	std::ifstream infile(this->fileName, std::ifstream::binary);

	

	infile.read((char*)&this->mesh, sizeof(Mesh));

	this->vertexArr = new Vertex[this->mesh.nrOfVertex];

	infile.read((char*)this->vertexArr, this->mesh.nrOfVertex * sizeof(Vertex));
	infile.close();


}

BiReader::~BiReader()
{
	this->inputFile.close();
}

void BiReader::printToConsole()
{
	std::cout << this->mesh.name << std::endl;

	for (int i = 0; i < this->mesh.nrOfVertex; i++) {
		std::cout << std::to_string(this->vertexArr[i].pos[0]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].pos[1]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].pos[2]) << std::endl;
		std::cout << std::endl;
		std::cout << std::to_string(this->vertexArr[i].uv[0]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].uv[1]) << std::endl;
		std::cout << std::endl;
		std::cout << std::to_string(this->vertexArr[i].norm[0]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].norm[1]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].norm[2]) << std::endl;
		std::cout << std::endl;
		std::cout << std::to_string(this->vertexArr[i].biNorm[0]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].biNorm[1]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].biNorm[2]) << std::endl;
		std::cout << std::endl;
		std::cout << std::to_string(this->vertexArr[i].tan[0]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].tan[1]) << std::endl;
		std::cout << std::to_string(this->vertexArr[i].tan[2]) << std::endl;
		std::cout << "--------------------------------------- " << std::endl;
	}
}
