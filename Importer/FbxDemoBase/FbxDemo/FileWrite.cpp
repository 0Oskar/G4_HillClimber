#include "FileWrite.h"

FileWrite::FileWrite()
{
	//this->fileName = "C:/Users/vibbo/OneDrive/Skrivbord/LitetSpel-HillClimbers/G4_HillClimber/Importer/FbxDemoBase/testFile.bff";
	this->fileName = "../biFile.bff";
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

<<<<<<< HEAD
void FileWrite::writeToFile(const char* input, size_t size)
{

=======
void FileWrite::writeToFile(char* data)
{
	//std::string outData = input;
>>>>>>> c1617e2b5d486fb9146c658f18f9294c2030a821
	//int timesBigger = 1;
	//if (outData.size() > MAX_STRING_LEN)
	//{
	//	timesBigger = std::ceil(float(outData.size()) / float(MAX_STRING_LEN));
	//}
<<<<<<< HEAD

=======
>>>>>>> c1617e2b5d486fb9146c658f18f9294c2030a821
	//int number = (MAX_STRING_LEN * timesBigger) - outData.size();
	//for (int i = 0; i < number; i++)
	//{
	//	outData += "Î";
	//}

<<<<<<< HEAD
	outputFile.write(input, size);

=======
	//outputFile.write(outData.c_str(), outData.size());
	outputFile.write(data, sizeof(data));
>>>>>>> c1617e2b5d486fb9146c658f18f9294c2030a821
}

void FileWrite::EmptyFile()
{
	this->outputFile.clear();
}

//outData.size() - bara data
//sizeof(outData) - hela längden av en string