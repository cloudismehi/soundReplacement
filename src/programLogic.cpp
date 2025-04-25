#include "include/programLogic.hpp" 

extern std::vector<std::string> filenames; //defined in main
extern int originalSample; //defined in main
extern int numFilesRead; //defined in main

void sayHi(){
    system("clear"); 
    std::cout << "\033[1mhello! welcome to the \033[4msound replacement software\033[m\n"; 
    std::cout << "\033[1mread \033[4m" << numFilesRead << "\033[m\033[1m samples\033[m\n"; 

    int _index = 0; 
    for (std::string name : filenames){
        std::cout << "\033[1m" << _index++ << ":\033[m " << name << '\n'; 
    }
    std::cout << "\n"; 

    std::cout << "\033[1mpick your base sample by its index\033[m\n"; 

    std::cin >> originalSample; 
    if (originalSample > numFilesRead){
        std::cout << "\033[1myou picked an index out of range, defaulting to index 0\033[m\n"; 
        originalSample = 0; 
    }  
    else {
        std::cout << "\033[1myou picked sample " << originalSample << "\033[m " << filenames[originalSample] << '\n'; 
    }
}