fftwInclude = -I ../../third-libs/fftw-3.3.10/lib/include
fftwLib = -L ../../third-libs/fftw-3.3.10/lib/lib -l fftw3

soundReplacement: soundReplacement.o fileHandling.o WaveFile.o fftOperations.o programLogic.o
	@g++ -std=c++20 -o soundReplacement ${fftwLib} src/obj/fileHandling.o \
	 src/obj/fftOperations.o src/obj/WaveFile.o src/obj/programLogic.o src/obj/soundReplacement.o 
	@mv soundReplacement execs 



soundReplacement.o: src/main.cpp
	@g++ -std=c++20  src/main.cpp ${fftwInclude} -I . -c -o soundReplacement.o
	@mv soundReplacement.o src/obj

fileHandling.o: src/fileHandling.cpp
	@g++ -std=c++20 src/fileHandling.cpp -I . -c -o fileHandling.o
	@mv fileHandling.o src/obj

fftOperations.o: src/fftOperations.cpp
	@g++ -std=c++20 src/fftOperations.cpp -I . ${fftwInclude} -c -o fftOperations.o
	@mv fftOperations.o src/obj

WaveFile.o: src/WaveFile.cpp
	@g++ -std=c++20 src/WaveFile.cpp -I . -c -o WaveFile.o
	@mv WaveFile.o src/obj

programLogic.o: src/programLogic.cpp
	@g++ -std=c++20 src/programLogic.cpp -I . -c -o programLogic.o
	@mv programLogic.o src/obj