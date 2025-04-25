#pragma once
#include <iostream> 
#include <fstream>
#include <string.h>
#include <cmath>

#ifndef WAVEFILE
#define WAVEFILE
class WaveFile
{
    public:
    std::fstream _pWaveFileIn;
    int _pSampleRate, _pFileSize, _pNumSamps; 
    short _pBitsPerSample, _pNumChannels, _pBlockAlign, _pAudioFormat; 
    int* _pData; 
    float* _pNormData; 
    float _pMaxVal, _pMinVal; 
    
    bool _pFileRead = false; 
    bool _pFileOpened = false; 

    bool openFile(std::string filename);
    void loadData(); 
    void dumpData(float* dataIn); 
    void dumpData(float* dataIn, int _numSamps); 
    void printFileData(); 

    void closeFile(); 

    ~WaveFile(); 
    
    private:
    int p_DataChunkStart; 

    void findDataChunk(); //also sets _pNumSamps
    void getFileSize(); 
    void readMetaData(); 
    int decode24BitSignedInts(int sample);
};

#endif