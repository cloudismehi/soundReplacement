#include "include/fileHandling.hpp"

extern int numFilesRead; //defined in main 

void checkAvailableFiles(std::vector<std::string>& _filenames, std::string _filePaths)
{
    fs::path assetsPath = _filePaths; 

    for (const auto& entry : fs::directory_iterator(assetsPath)){
        std::string filename = entry.path().filename().string(); 

        if (filename.substr(filename.length() - 4) == ".wav"){
            _filenames.push_back(filename); 
            numFilesRead++; 
        }
    }
}

void writeToFile(std::string _filename, float *_data, int _numSamps)
{
    std::fstream file; 
    std::string name = _filename; 
    if (name.substr(name.length() - 4) == ".wav"){
        for (int i = 0 ; i < 4; i++) name.pop_back(); 
    } 

    file.open("assets/analyses/" + name + ".txt", std::ios::out); 
    for (int i = 0 ; i < _numSamps / 2; i++){
        file << _data[i] << '\n'; 
    }
	file.close(); 
}