#pragma once
#include <string>
#include <iostream>
#include <fstream>


class FileWrite
{

private:
	std::string fileName;
	std::ofstream outputFile;

	const int MAX_STRING_LEN = 64;


public:
	FileWrite();
	~FileWrite();

	void writeToFile(std::string input);


};
