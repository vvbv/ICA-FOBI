#include <armadillo>
#include <iostream>
#include "AudioFile.h"
#include <string>

std::string resource_forlder = "resources/";


int main(){

    AudioFile<double> audioFile_1;
    AudioFile<double> audioFile_2;
    audioFile_1.load ( resource_forlder + "source1.wav" );
    audioFile_2.load ( resource_forlder + "source2.wav" );
    audioFile_1.printSummary();
    audioFile_2.printSummary();


    return 0;
};