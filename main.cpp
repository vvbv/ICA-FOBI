#include <armadillo>
#include <iostream>
#include "AudioFile.h"
#include <string>

std::string resource_forlder = "resources/";


int main(){

    AudioFile<double> audioFile;
    audioFile.load ( resource_forlder + "source1.wav" );
    int sampleRate = audioFile.getSampleRate();
    std::cout << std::to_string( sampleRate ) << std::endl;

    return 0;
};