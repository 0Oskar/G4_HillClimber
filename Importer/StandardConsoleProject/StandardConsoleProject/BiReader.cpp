#include "BiReader.h"

BiReader::BiReader(std::string path)
{
	this->fileName = path;
	std::ifstream infile(this->fileName, std::ifstream::binary);

	infile.read((char*)&this->mesh, sizeof(Mesh));

	this->vertexArr = new Vertex[this->mesh.nrOfVertex];

	infile.read((char*)this->vertexArr, this->mesh.nrOfVertex * sizeof(Vertex));
	infile.read((char*)&this->material, sizeof(Material));
	infile.close();
}

BiReader::~BiReader()
{
	this->inputFile.close();
}

void BiReader::printFromMainToConsole()
{
	std::cout << this->mesh.name << std::endl;

	for (int i = 0; i < this->mesh.nrOfVertex; i++) 
	{
		std::cout << "-------------------------------------- Index: " << i << std::endl;
		std::cout << std::to_string(vertexArr[i].pos[0]) << std::endl;
		std::cout << std::to_string(vertexArr[i].pos[1]) << std::endl;
		std::cout << std::to_string(vertexArr[i].pos[2]) << std::endl;
		std::cout << std::endl;			 	 
		std::cout << std::to_string(vertexArr[i].uv[0]) << std::endl;
		std::cout << std::to_string(vertexArr[i].uv[1]) << std::endl;
		std::cout << std::endl;			  	 
		std::cout << std::to_string(vertexArr[i].norm[0]) << std::endl;
		std::cout << std::to_string(vertexArr[i].norm[1]) << std::endl;
		std::cout << std::to_string(vertexArr[i].norm[2]) << std::endl;
		std::cout << std::endl;			  	
		std::cout << std::to_string(vertexArr[i].biNorm[0]) << std::endl;
		std::cout << std::to_string(vertexArr[i].biNorm[1]) << std::endl;
		std::cout << std::to_string(vertexArr[i].biNorm[2]) << std::endl;
		std::cout << std::endl;			  	 
		std::cout << std::to_string(vertexArr[i].tan[0]) << std::endl;
		std::cout << std::to_string(vertexArr[i].tan[1]) << std::endl;
		std::cout << std::to_string(vertexArr[i].tan[2]) << std::endl;
	}
	std::cout << "-------------------------------------- Material: " << std::endl;
	std::cout << std::to_string(material.Diffuse[0]) << std::endl;
	std::cout << std::to_string(material.Diffuse[1]) << std::endl;
	std::cout << std::to_string(material.Diffuse[2]) << std::endl;
	std::cout << std::endl;
	std::cout << std::to_string(material.Ambient[0]) << std::endl;
	std::cout << std::to_string(material.Ambient[1]) << std::endl;
	std::cout << std::to_string(material.Ambient[2]) << std::endl;
	std::cout << std::endl;
	std::cout << std::to_string(material.Opacity) << std::endl;
}

unsigned int BiReader::getNrOfVertex()
{
	return mesh.nrOfVertex;
}



void BiReader::getVertexPositions(Vertex* someVerticies)
{
	int nrOfVertex = this->mesh.nrOfVertex;

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

Material BiReader::getMaterial()
{
	Material tempMaterial;

	tempMaterial.Diffuse[0] = this->material.Diffuse[0];
	tempMaterial.Diffuse[1] = this->material.Diffuse[1];
	tempMaterial.Diffuse[2] = this->material.Diffuse[2];
	tempMaterial.Ambient[0] = this->material.Ambient[0];
	tempMaterial.Ambient[1] = this->material.Ambient[1];
	tempMaterial.Ambient[2] = this->material.Ambient[2];
	tempMaterial.Opacity = this->material.Opacity;

	return tempMaterial;
}
