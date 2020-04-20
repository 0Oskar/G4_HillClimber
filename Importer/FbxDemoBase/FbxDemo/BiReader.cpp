#include "BiReader.h"

BiReader::BiReader(std::string path)
{
	this->fileName = path;

	

	fopen_s(&this->in, (const char*)&this->fileName, "rb");
	if (this->in != NULL) {
		fread_s(&this->a, sizeof(Temp), sizeof(Temp), 1, this->in);
		fclose(this->in);

	}

	
}

BiReader::~BiReader()
{
	this->inputFile.close();
}

void BiReader::printToConsole()
{
	printf("Print: ", (this->a.name), std::to_string(this->a.nrOfVertex));
}
