#include <armadillo>
#include <iostream>
#include "AudioFile.h"
#include <string>

std::string resource_forlder = "resources/";


int main(){

    AudioFile<double> audioFile_1;
    AudioFile<double> audioFile_2;
    audioFile_1.load ( resource_forlder + "mix1.wav" );
    audioFile_2.load ( resource_forlder + "mix2.wav" );
    //audioFile_1.printSummary();
    //audioFile_2.printSummary();

    int channel = 0;
    int numSamples = audioFile_1.getNumSamplesPerChannel();

    /*for (int i = 0; i < numSamples; i++)
    {
        double currentSample = audioFile_1.samples[channel][i];
    }*/

    std::cout << audioFile_2.samples[0][0] << " ";
    std::cout << audioFile_2.samples[0][1] << " ";
    std::cout << audioFile_2.samples[0][2] << " ";
    std::cout << audioFile_2.samples[0][3] << " ";
    std::cout << audioFile_2.samples[0][4] << " " << std::endl;

    return 0;
};