#include "BiReader.h"


int main() 
{
	//Vet ej varf�r vi har denna h�r �n s� l�nge
	//Mesh mesh;

	BiReader myMesh("../../FbxDemoBase/biFile.bff");

	Vertex* tempVertex;

	int nrVerticies = myMesh.getNrOfVertex();
	tempVertex = new Vertex[nrVerticies];


	myMesh.getVertexPositions(tempVertex);
	myMesh.printFromMainToConsole(tempVertex);

	
	return 0;
}