#include <iostream>
#include <complex>
#include <vector>
#include <filesystem>

#include "include/fileHandling.hpp"
#include "include/fftOperations.hpp"
#include "include/programLogic.hpp"

#include "fftw3.h"

//TODO: DOESN'T	WORK FOR LONGER SIGNALS

int MAX_NUM_SAMPS = 20; 
int numFilesRead = 0; 
int originalSample = 0; 
const int fftSize = 8192; 

const int numberOfSamps = 44100; //how many samples are we replacing at a time

extern float *fileData; //declared in fftOperations.cpp
extern float *fftData; //declared in fftOperations.cpp

std::vector<std::string> filenames; 
std::string assetsFilePath = "assets/audioFiles/"; 

std::vector<float*> analyses; 
std::vector<float> correlations; 

int main(){
	analyses.reserve(MAX_NUM_SAMPS); 
	checkAvailableFiles(filenames, assetsFilePath);  //checks for wav files in the assets folder
	setupFFT(); 

	sayHi(); //prints files to console and asks to pick the base file
	execFFTofCandidates(); //executes fft of every file but the base file
	execFFTofOriginal(); //executes fft of original sample
	
	

	cleanupFFT(); 
	return 0; 
}
