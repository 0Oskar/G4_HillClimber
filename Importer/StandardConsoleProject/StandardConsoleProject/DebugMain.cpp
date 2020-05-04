#include "BiReader.h"


int main() 
{
	//Vet ej varför vi har denna här än så länge
	//Mesh mesh;

	BiReader myMesh("../../FbxDemoBase/biFile.bff");
	//myMesh.printFromMainToConsole();

	int nrVerticies = myMesh.getNrOfVertex();

	Vertex* tempVertex = new Vertex[nrVerticies];
	myMesh.getVertexPositions(tempVertex);

	Material tempMaterial = myMesh.getMaterial();

	

	std::cout << std::to_string(tempMaterial.Diffuse[0]);
	
	return 0;
}