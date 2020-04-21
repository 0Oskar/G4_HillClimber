#include "BiReader.h"

int main() 
{


	BiReader myMesh("../../FbxDemoBase/biFile.bff");
	//myMesh.printToConsole();
	std::vector<Vertex> *hej = myMesh.getVertexPositions();

	
	return 0;
}