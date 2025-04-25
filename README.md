## Sound Replacement
The purpose of this project is to perform fft analysis on a set of signals and then find the closest match to some provided "base case". 
As of right now, the project is set up so that it reads all .wav files in some directory, and analyses them. The correlation function is already written
but i'm pretty sure it's not actually being used. 

### _Dependencies_
- fftw library: for the fft operations
- gnuplot: it's optional, there is a script to visualize the fft analysis of the signals
- _raylib:_ not yet being used but it could be used for GUI and for audio and file i/o handling. 

### _Need to do_
Still need to test that i am where i think i am on the project, code the correlation of our signals, and write the function for the sound replacement
**In the future** it might be nice to have the option to do this live, with some input and output. The i/o could be handled with raylib although i'm not sure
how reliable this would be - i have never tried using raylib for this -. 
