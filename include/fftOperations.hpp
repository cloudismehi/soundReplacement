#ifndef FFT_OPERATIONS
#define FFT_OPERATIONS

#include <iostream>
#include <complex.h>
#include <fstream>
#include <vector>

#include "include/WaveFile.hpp"
#include "include/fileHandling.hpp"

#include "fftw3.h"

//extern global vars
//all these are defined in main.cpp 
extern const int fftSize; 
extern const int numberOfSamps;
extern int numFilesRead;
extern int originalSample;   
extern std::string assetsFilePath; 
extern std::vector<std::string> filenames; 

extern std::vector<float*> analyses; 

extern int iter; //defined in fftOperations.cpp

void setupFFT(); 
bool analyzeFile(std::string _filename); 
bool analyzeFile(float* _file, int _fileSize); 
void cleanupFFT(); 

void execFFTofCandidates(); 
void execFFTofOriginal(); 
void execCorrelation(float* sample_1, float* sample_2, float &correlationResult); 
void execReplacement(); 


void moveAround(float* _src, std::complex<double>* _dest, int _numSamps, int _offset);
void transAmp(std::complex<double>* _srcSignal, float* _destSignal, int _sigSize, bool _normalize = true); 

#endif