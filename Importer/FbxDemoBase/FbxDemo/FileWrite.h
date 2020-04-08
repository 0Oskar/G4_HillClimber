#pragma once
#include <string>
#include <iostream>
#include <fstream>


class FileWrite
{

private:
	std::string fileName;
	std::ofstream outputFile;

	char MAX_STRING_LENGTH[8];


public:
	FileWrite();
	~FileWrite();

	void writeToFile(std::string input);


};
