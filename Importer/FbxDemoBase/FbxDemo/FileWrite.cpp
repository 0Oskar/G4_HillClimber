#include "FileWrite.h"

FileWrite::FileWrite(std::string path)
{
	//this->fileName = "C:/Users/vibbo/OneDrive/Skrivbord/LitetSpel-HillClimbers/G4_HillClimber/Importer/FbxDemoBase/testFile.bff";
	this->fileName = path; 
	this->outputFile.open(fileName.c_str(), std::ofstream::binary | std::ios_base::app);
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

void FileWrite::writeToFile(const char* input, size_t size)
{
	outputFile.write(input, size);
}

void FileWrite::writeToStringFile(std::string input)
{
	//int timesBigger = 1;
	//if (outData.size() > MAX_STRING_LEN)
	//{
	//	timesBigger = std::ceil(float(outData.size()) / float(MAX_STRING_LEN));
	//}

	//int number = (MAX_STRING_LEN * timesBigger) - outData.size();
	//for (int i = 0; i < number; i++)
	//{
	//	outData += "Î";
	//}
	outputFile.write(input.c_str(), input.size());
}

void FileWrite::EmptyFile()
{
	this->outputFile.clear();
}

//outData.size() - bara data
//sizeof(outData) - hela längden av en string