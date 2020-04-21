#include "BiReader.h"
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <stdio.h>
//#include "../../FbxDemoBase/FbxDemo/ImporterStructure.h"

int main() {


	BiReader myMesh("../../FbxDemoBase/biFile.bff");
	myMesh.printToConsole();
	



	//for (int i = 0; i < mesh2.nrOfVertex; i++){
	//	delete vertexArr[i];
	//}
	return 0;
}