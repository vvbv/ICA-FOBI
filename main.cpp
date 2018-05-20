#include <armadillo>
#include <iostream>
#include "AudioFile.h"
#include <string>
#include <vector>

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

    arma::mat transpose_eigvectors = eigvectors.t();

    arma::mat transpose_eigvectors_dot_data_matrix = data_matrix*transpose_eigvectors;
    arma::mat whitened_data = transpose_eigvectors_dot_data_matrix*inverse_root_diagonal_eigvalues;

    arma::vec arma_whitened_data_norm_2_axis_0;
    std::vector < double > whitened_data_norm_2_axis_0;
    for( int x = 0; x < whitened_data.col(0).n_elem; x++ ){
        whitened_data_norm_2_axis_0.push_back( arma::norm(whitened_data.row(x), 2) );
    };

    arma_whitened_data_norm_2_axis_0 = whitened_data_norm_2_axis_0;

    arma::mat matrix_norm( arma_whitened_data_norm_2_axis_0.n_elem, 2 );

    for( int x = 0; x < arma_whitened_data_norm_2_axis_0.n_elem; x++ ){
        matrix_norm.row(x).col(0) = arma_whitened_data_norm_2_axis_0[x];
        matrix_norm.row(x).col(1) = arma_whitened_data_norm_2_axis_0[x];
    };

    arma::mat m_nomr_multiply_wd_norm( matrix_norm.col(0).n_elem, 2 );
    for( int x = 0; x < matrix_norm.col(0).n_elem; x++ ){
        m_nomr_multiply_wd_norm.row(x).col(0) = matrix_norm.row(x).col(0) * whitened_data.row(x).col(0);
        m_nomr_multiply_wd_norm.row(x).col(1) = matrix_norm.row(x).col(1) * whitened_data.row(x).col(1);
    };

    arma::mat covariance_m_nomr_multiply_wd_norm = arma::cov( m_nomr_multiply_wd_norm );

    arma::vec eigvalues_m_nomr_multiply_wd_norm;
    arma::mat eigvectors_m_nomr_multiply_wd_norm;

    arma::eig_sym(eigvalues_m_nomr_multiply_wd_norm, eigvectors_m_nomr_multiply_wd_norm, covariance_m_nomr_multiply_wd_norm);

    arma::mat transposed_eigvectors_m_nomr_multiply_wd_norm = eigvectors_m_nomr_multiply_wd_norm.t();

    arma::mat source = eigvectors_m_nomr_multiply_wd_norm*whitened_data.t();

    std::cout << "\nSeparación terminada: Test \n\n" << source.col( 0 ) << std::endl << std::endl;

    return 0;
};