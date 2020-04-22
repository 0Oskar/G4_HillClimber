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

//void BiReader::printToConsole()
//{
//	std::cout << this->mesh.name << std::endl;
//
//	for (int i = 0; i < this->mesh.nrOfVertex; i++) {
//		std::cout << std::to_string(this->vertexArr[i].pos[0]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].pos[1]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].pos[2]) << std::endl;
//		std::cout << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].uv[0]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].uv[1]) << std::endl;
//		std::cout << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].norm[0]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].norm[1]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].norm[2]) << std::endl;
//		std::cout << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].biNorm[0]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].biNorm[1]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].biNorm[2]) << std::endl;
//		std::cout << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].tan[0]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].tan[1]) << std::endl;
//		std::cout << std::to_string(this->vertexArr[i].tan[2]) << std::endl;
//		std::cout << "-------------------------------------- " << std::endl;
//	}
//}

void BiReader::printFromMainToConsole(Vertex* theVertex)
{
	std::cout << this->mesh.name << std::endl;

	for (int i = 0; i < this->mesh.nrOfVertex; i++) 
	{
		std::cout << std::to_string(theVertex[i].pos[0]) << std::endl;
		std::cout << std::to_string(theVertex[i].pos[1]) << std::endl;
		std::cout << std::to_string(theVertex[i].pos[2]) << std::endl;
		std::cout << std::endl;			 	 
		std::cout << std::to_string(theVertex[i].uv[0]) << std::endl;
		std::cout << std::to_string(theVertex[i].uv[1]) << std::endl;
		std::cout << std::endl;			  	 
		std::cout << std::to_string(theVertex[i].norm[0]) << std::endl;
		std::cout << std::to_string(theVertex[i].norm[1]) << std::endl;
		std::cout << std::to_string(theVertex[i].norm[2]) << std::endl;
		std::cout << std::endl;			  	
		std::cout << std::to_string(theVertex[i].biNorm[0]) << std::endl;
		std::cout << std::to_string(theVertex[i].biNorm[1]) << std::endl;
		std::cout << std::to_string(theVertex[i].biNorm[2]) << std::endl;
		std::cout << std::endl;			  	 
		std::cout << std::to_string(theVertex[i].tan[0]) << std::endl;
		std::cout << std::to_string(theVertex[i].tan[1]) << std::endl;
		std::cout << std::to_string(theVertex[i].tan[2]) << std::endl;
		std::cout << "-------------------------------------- " << std::endl;
	}
}

unsigned int BiReader::getNrOfVertex()
{
	return mesh.nrOfVertex;
}



void BiReader::getVertexPositions(Vertex* someVerticies)
{
	int nrOfVertex = this->mesh.nrOfVertex;
	
	//std::vector<Vertex> *returnVec = new std::vector<Vertex>[nrOfVertex];

	for (int i = 0; i < nrOfVertex; i++)
	{
		someVerticies[i].pos[0] = this->vertexArr[i].pos[0];
		someVerticies[i].pos[1] = this->vertexArr[i].pos[1];
		someVerticies[i].pos[2] = this->vertexArr[i].pos[2];
		someVerticies[i].uv[0] = this->vertexArr[i].uv[0];
		someVerticies[i].uv[1] = this->vertexArr[i].uv[1];
		someVerticies[i].norm[0] = this->vertexArr[i].norm[0];
		someVerticies[i].norm[1] = this->vertexArr[i].norm[1];
		someVerticies[i].norm[2] = this->vertexArr[i].norm[2];
		someVerticies[i].biNorm[0] = this->vertexArr[i].biNorm[0];
		someVerticies[i].biNorm[1] = this->vertexArr[i].biNorm[1];
		someVerticies[i].biNorm[2] = this->vertexArr[i].biNorm[2];
		someVerticies[i].tan[0] = this->vertexArr[i].tan[0];
		someVerticies[i].tan[1] = this->vertexArr[i].tan[1];
		someVerticies[i].tan[2] = this->vertexArr[i].tan[2];
	}	
}
