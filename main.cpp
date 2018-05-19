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

    arma::Mat <double>  data_matrix( numSamples_1, 2);
    
    for( int x = 0; x < numSamples_1; x++ ){
        data_matrix.col(0).row(x) = audioFile_1.samples[channel][x];
    };

    for( int x = 0; x < numSamples_2; x++ ){
        data_matrix.col(1).row(x) = audioFile_2.samples[channel][x];
    };
    
    arma::mat covariance_matrix = arma::cov( data_matrix );
   
    arma::vec eigvalues;
    arma::mat eigvectors;

    arma::eig_sym(eigvalues, eigvectors, covariance_matrix);

    arma::mat diagonal_eigvalues = arma::zeros<arma::mat>(eigvalues.n_elem,eigvalues.n_elem);
    for( int x = 0; x < eigvalues.n_elem; x++ ){
        diagonal_eigvalues.col(x).row(x) = eigvalues[x];
    };

    arma::mat inverse_diagonal_eigvalues = arma::inv( diagonal_eigvalues );

    arma::mat inverse_root_diagonal_eigvalues = arma::zeros<arma::mat>(eigvalues.n_elem,eigvalues.n_elem);
    for( int x = 0; x < eigvalues.n_elem; x++ ){
        inverse_root_diagonal_eigvalues.col(x).row(x) = arma::sqrt( inverse_diagonal_eigvalues.col(x).row(x) );
    };

    std::cout << inverse_root_diagonal_eigvalues << std::endl;

    return 0;
};