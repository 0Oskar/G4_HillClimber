#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "ImporterStructure.h"

class FileWrite
{

private:
	std::string fileName;
	std::ofstream outputFile;

	const int MAX_STRING_LEN = 26;


public:
	FileWrite(std::string path);
	~FileWrite();

	void writeToFile(const char* input, size_t size);
	void writeToStringFile(std::string input);
	void EmptyFile();


};
