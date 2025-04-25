#include "include/WaveFile.hpp"

bool WaveFile::openFile(std::string filename)
{
    _pWaveFileIn.open(filename, std::ios::binary | std::ios::in); 
    if (!_pWaveFileIn) {
        _pFileOpened = false; 
        return false; 
    }

    getFileSize();
    readMetaData(); 
    findDataChunk(); 
    _pData = new int[_pNumSamps];
    _pNormData = new float[_pNumSamps];
    _pFileOpened = true; 
    return true; 
}

void WaveFile::loadData()
{
    int maxSampVal = std::pow(2, _pBitsPerSample - 1) - 1; 
    _pWaveFileIn.seekg(p_DataChunkStart + 8);

    if (_pBitsPerSample == 24)
    {
        int readSample, decodedSample; 
        for (int i = 0; i < _pNumSamps; i++)
        {
            _pWaveFileIn.read((char*)&readSample, 3);
            _pData[i] = decode24BitSignedInts(readSample);
            _pNormData[i] = _pData[i] / (float)maxSampVal;
            if (_pNormData[i] > _pMaxVal) _pMaxVal = _pNormData[i]; 
            if (_pNormData[i] < _pMinVal) _pMinVal = _pNormData[i];  
        }
    }
    if (_pBitsPerSample == 16)
    {
        short readSample;  
        for (int i = 0; i < _pNumSamps; i++)
        {
            _pWaveFileIn.read((char*)&readSample, 2);
            _pData[i] = readSample; 
            _pNormData[i] = _pData[i] / (float)maxSampVal; 
            if (_pNormData[i] > _pMaxVal) _pMaxVal = _pNormData[i]; 
            if (_pNormData[i] < _pMinVal) _pMinVal = _pNormData[i]; 
        }
    }
    if (_pBitsPerSample == 32)
    {
        //FIXME: 32-bit floating point file reading hasn't been implemented
        printf("WARNING! reading 32 bits hasn't been tested and may result in\n"
        "unverified behavior\n");
        int readSample; 
        for (int i = 0; i < _pNumSamps; i++)
        {
            _pWaveFileIn.read((char*)&readSample, 4);
            _pData[i] = readSample; 
            _pNormData[i] = _pData[i] / (float)maxSampVal;
            if (_pNormData[i] > _pMaxVal) _pMaxVal = _pNormData[i]; 
            if (_pNormData[i] < _pMinVal) _pMinVal = _pNormData[i]; 
        }
    }    
    _pFileRead = true; 
}

void WaveFile::dumpData(float* dataIn)
{
    for(int i = 0; i < _pNumSamps; i++) dataIn[i] = _pNormData[i];
}

void WaveFile::dumpData(float* dataIn, int _numSamps){
    for(int i = 0; i < _numSamps; i++) dataIn[i] = _pNormData[i];
}

void WaveFile::printFileData()
{
    if (_pFileRead){ 
        std::cout << "audio format: " << _pAudioFormat << '\n';
        std::cout << "sample rate: " << _pSampleRate << '\n';
        std::cout << "bit rate: " << _pBitsPerSample << '\n';
        std::cout << "num channels: " << _pNumChannels << '\n';
        std::cout << "block align: " << _pBlockAlign << '\n';
    }
    else {
        std::cout << "no file was ever read\n";
    }
}

void WaveFile::closeFile()
{
    if (_pFileOpened) _pWaveFileIn.close(); 
}

WaveFile::~WaveFile()
{
    if (_pFileOpened){
        delete[] _pData; 
        delete[] _pNormData; 
        _pWaveFileIn.close(); 
    }
}

void WaveFile::getFileSize()
{
    _pWaveFileIn.seekg(0, _pWaveFileIn.end); 
    _pFileSize = _pWaveFileIn.tellg(); 
    _pWaveFileIn.seekg(0, _pWaveFileIn.beg);
}

void WaveFile::readMetaData()
{
    //audio format
    _pWaveFileIn.seekg(20, _pWaveFileIn.beg); 
    _pWaveFileIn.read(reinterpret_cast<char*>(&_pAudioFormat), 2);
    //sample rate
    _pWaveFileIn.seekg(24, _pWaveFileIn.beg);
    _pWaveFileIn.read(reinterpret_cast<char*>(&_pSampleRate), 4);
    //bits per sample
    _pWaveFileIn.seekg(34); 
    _pWaveFileIn.read(reinterpret_cast<char*> (&_pBitsPerSample), 2);
    //num channels
    _pWaveFileIn.seekg(22); 
    _pWaveFileIn.read(reinterpret_cast<char*> (&_pNumChannels), 2);
    //block align 
    _pWaveFileIn.seekg(32, _pWaveFileIn.beg);
    _pWaveFileIn.read(reinterpret_cast<char*>(&_pBlockAlign), 2);
}

void WaveFile::findDataChunk()
{
    for (int i = 0; i < _pFileSize; i++){
        _pWaveFileIn.seekg(i, _pWaveFileIn.beg);
        char readChar[4]; 
        _pWaveFileIn.read(readChar, 4);
        if (readChar[0] == 'd') {
            if (strcmp(readChar, "data")){
                p_DataChunkStart = i; 
                break; 
            }
        }
    }
    _pWaveFileIn.seekg(p_DataChunkStart + 4);
    _pWaveFileIn.read(reinterpret_cast<char*>(&_pNumSamps), 4); 
    _pNumSamps = (_pNumSamps / _pNumChannels) / (_pBitsPerSample / 8); 
}

int WaveFile::decode24BitSignedInts(int sample)
{  
    int decoded; 
    if (sample & (1 << 23))
    {   
        //turns sample into 32 bits
        for (int i = 0; i < 8; i++)
        {
            sample ^= (1 << (24 + i));
        }
        //decode bits into int
        decoded = -((~sample) + 1);
    }
    else 
    {   
        //pass sample as is
        decoded = sample; 
    }
    return decoded; 
}