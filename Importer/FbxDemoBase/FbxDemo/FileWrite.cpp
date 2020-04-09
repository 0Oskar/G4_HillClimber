#include "FileWrite.h"

FileWrite::FileWrite()
{
	//this->fileName = "C:/Users/vibbo/OneDrive/Skrivbord/LitetSpel-HillClimbers/G4_HillClimber/Importer/FbxDemoBase/testFile.bff";
	this->fileName = "../biFile.bff";
	this->outputFile.open(fileName.c_str(), std::ofstream::binary);
	//std::ios_base::app

	//for (int i = 0; i < 64; i++) {
	//	this->MAX_STRING_LENGTH[i] = 1;
	//}

	/*this->MAX_STRING_LENGTH[64] = { 1 };*/
}

FileWrite::~FileWrite()
{
	this->outputFile.close();
}

void FileWrite::writeToFile(std::string input)
{
	std::string outData = input;
	
	int number = MAX_STRING_LEN - outData.size();
	for (int i = 0; i < number; i++) 
	{
		outData += "Î";
	}

	outputFile.write(outData.c_str(), outData.size());
}

//outData.size() - bara data
//sizeof(outData) - hela längden av en string