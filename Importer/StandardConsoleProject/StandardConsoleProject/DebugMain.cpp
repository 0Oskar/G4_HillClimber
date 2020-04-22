#include "BiReader.h"


int main() 
{
	//Vet ej varför vi har denna här än så länge
	//Mesh mesh;

	BiReader myMesh("../../FbxDemoBase/biFile.bff");

	Vertex* tempVertex;

	int nrVerticies = myMesh.getNrOfVertex();
	tempVertex = new Vertex[nrVerticies];


	myMesh.getVertexPositions(tempVertex);
	myMesh.printFromMainToConsole(tempVertex);

	
	return 0;
}