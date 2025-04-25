#ifndef FILE_HANDLING
#define FILE_HANDLING

#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem; 

void checkAvailableFiles(std::vector<std::string>& _filenames, std::string _filePaths); 
void writeToFile(std::string _filename, float* _data, int _numSamps); 

#endif