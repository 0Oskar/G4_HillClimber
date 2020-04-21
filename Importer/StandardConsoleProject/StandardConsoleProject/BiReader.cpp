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
		std::cout << "-------------------------------------- " << std::endl;
	}
}

std::vector<Vertex> *BiReader::getVertexPositions()
{
	int nrOfVertex = this->mesh.nrOfVertex;
	std::vector<Vertex> *returnVec = new std::vector<Vertex>[nrOfVertex];

	for (int i = 0; i < nrOfVertex; i++)
	{
		returnVec->at(i).pos[0] = this->vertexArr[i].pos[0];
		returnVec->at(i).pos[1] = this->vertexArr[i].pos[1];
		returnVec->at(i).pos[2] = this->vertexArr[i].pos[2];
		returnVec->at(i).uv[0] = this->vertexArr[i].uv[0];
		returnVec->at(i).uv[1] = this->vertexArr[i].uv[1];
		returnVec->at(i).norm[0] = this->vertexArr[i].norm[0];
		returnVec->at(i).norm[1] = this->vertexArr[i].norm[1];
		returnVec->at(i).norm[2] = this->vertexArr[i].norm[2];
		returnVec->at(i).biNorm[0] = this->vertexArr[i].biNorm[0];
		returnVec->at(i).biNorm[1] = this->vertexArr[i].biNorm[1];
		returnVec->at(i).biNorm[2] = this->vertexArr[i].biNorm[2];
		returnVec->at(i).tan[0] = this->vertexArr[i].tan[0];
		returnVec->at(i).tan[1] = this->vertexArr[i].tan[1];
		returnVec->at(i).tan[2] = this->vertexArr[i].tan[2];
	}

	return returnVec;
}
