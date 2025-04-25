#include "include/fftOperations.hpp"

std::complex<double>* complexBucket;
fftw_plan fftPlan;
float* fileData;
float* fftData; 
int iter; 

void setupFFT(){
    complexBucket = (std::complex<double>*) fftw_malloc(sizeof(std::complex<double>) * fftSize); 
    fftPlan = fftw_plan_dft_1d(fftSize, (fftw_complex*) complexBucket, (fftw_complex*) complexBucket, FFTW_FORWARD, FFTW_ESTIMATE);
    fftData = new float[fftSize]; 
}


bool analyzeFile(std::string _filename)
{
    WaveFile _file;
    std::fstream output; 

    if (!_file.openFile(_filename)){
        std::cout << "error opening file " << _filename << '\n'; 
        return false; 
    }

    fileData = new float[_file._pNumSamps]; 
    _file.loadData(); 
    _file.dumpData(fileData, _file._pNumSamps);

    int dec = _filename.find_last_of('/'); 

    std::string foo = "outputs"+_filename.substr(dec); 
    for (int i = 0 ; i < 4; i++) foo.pop_back(); 
    foo.append(".txt");

    output.open(foo, std::ios::out); 
    for (int i = 0; i < _file._pNumSamps; i+= 100){
        output << fileData[i] <<'\n'; 
    }
    output.close(); 

    iter = floor(_file._pNumSamps / fftSize); 
    
    for (int i = 0; i < iter; i++){
        moveAround(fileData, complexBucket, fftSize, (i * fftSize / 2)); 
        fftw_execute(fftPlan); 
        transAmp(complexBucket, fftData, fftSize, true); 
    }

    delete[] fileData;  
    return true; 
}

bool analyzeFile(float* _file , int _fileSize){
    iter = floor(_fileSize / fftSize); 
    for (int i = 0; i < iter; i++){
        moveAround(_file, complexBucket, fftSize, (i * fftSize / 2)); 
        fftw_execute(fftPlan); 
        transAmp(complexBucket, fftData, fftSize, true); 
    }
    return true; 
}

void cleanupFFT(){
    fftw_destroy_plan(fftPlan); 
    fftw_free(complexBucket);
    delete[] fftData;

    for (int i = 0; i < numFilesRead; i++) delete[] analyses[i]; 
}

void execFFTofCandidates(){
    for (int i = 0; i < numFilesRead; i++){
        analyses[i] = new float[fftSize]; 

        if (i == originalSample) continue;
        analyzeFile(assetsFilePath + filenames[i]); 
        writeToFile(filenames[i], fftData, fftSize); 

        for (int q = 0; q < fftSize; q++){
            analyses[i][q] = fftData[q]; 
        }
    }
}

void execFFTofOriginal(){
    WaveFile file; 
    file.openFile(assetsFilePath + filenames[originalSample]); 
    file.loadData(); 
    fileData = new float[file._pNumSamps]; 

    file.dumpData(fileData); 

    analyzeFile(fileData, file._pNumSamps); 
    writeToFile(filenames[originalSample], fftData, fftSize); 

    delete[] fileData; 
}

void execCorrelation(float* sample_1, float* sample_2, float &correlationResult){
    float result = 0; 
    for (int i = 0; i < fftSize; i++){
        result += sample_1[i] * sample_2[i]; 
    }
    correlationResult = result; 
}


void moveAround(float *_src, std::complex<double> *_dest, int _numSamps, int _offset)
{
    float a0 = 0.21557895, a1 = 0.41663158, a2 = 0.277263158, a3 = 0.083578947, a4 = 0.006947368; 
    for (int i = 0; i < _numSamps; i++) {
        if (i + _offset > _numSamps){
            _dest[i] = {0, 0}; 
        } else {
            //using flat-top window
            float wind = a0 - (a1 * std::cos(2 * M_PI * i / _numSamps)) + (a2 * std::cos(4 * M_PI * i / _numSamps))
            - (a3 * std::cos(6 * M_PI * i / _numSamps)) + (a4 * std::cos(8 * M_PI * i / _numSamps)) ; 

            // wind = (0.5 * (1 - std::cos(2 * M_PI * i / _numSamps))); //hann win
            _dest[i] = {_src[i + _offset] * wind, 0};
        }   
    }
}

void transAmp(std::complex<double> *_srcSignal, float *_destSignal, int _sigSize, bool _normalize)
{
    for (int i = 0; i < _sigSize; i++){
        _destSignal[i] += std::sqrt( std::pow( _srcSignal[i].real(), 2 ) + std::pow( _srcSignal[i].imag(), 2) );
        if (_normalize){
            _destSignal[i] *= 2; 
            _destSignal[i] /= 2; 
        }
    }
}