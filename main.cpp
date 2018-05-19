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

    int channel = 0;
    int numSamples_1 = audioFile_1.getNumSamplesPerChannel();
    int numSamples_2 = audioFile_2.getNumSamplesPerChannel();

    arma::Mat <double>  data_matrix(2, numSamples_1);
    
    for( int x = 0; x < numSamples_1; x++ ){
        data_matrix.col(0).row(x) = audioFile_1.samples[channel][x];
    };

    for( int x = 0; x < numSamples_2; x++ ){
        data_matrix.col(1).row(x) = audioFile_2.samples[channel][x];
    };
    
    //arma::mat y = {{0,1},{2,3},{2,3},{2,3},{2,3},{2,3},{2,3}};
    //arma::mat x = arma::cov( y );
    arma::mat x = arma::cov( data_matrix );
    std::cout << x << std::endl;

    return 0;
};