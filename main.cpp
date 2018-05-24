/**
Implementacion del metodo FOBI, metodo de ICA
Integrantes:
	Jeison Cardona Gomez 1325562
	Juan Camilo Perez Muñoz 1630779
	Emily Esmeralda Carvajal Camelo 1630436
**/


//Libreria de Algebra lineal
#include <armadillo>
//Libreria para leer y escribir archivos de audio
#include "AudioFile.h"
#include <iostream>
#include <string>
#include <vector>
#include "matplotlibcpp.h"

std::string resource_forlder = "resources/";
namespace plt = matplotlibcpp;

int main(){


	//lee los archivos de audio
    AudioFile<double> audioFile_1;
    AudioFile<double> audioFile_2;
    audioFile_1.load ( resource_forlder + "mix1.wav" );
    audioFile_2.load ( resource_forlder + "mix2.wav" );

    int channel = 0;
    int numSamples_1 = audioFile_1.getNumSamplesPerChannel();
    int numSamples_2 = audioFile_2.getNumSamplesPerChannel();
    //crea una matriz para colocar los datos correspondientes a los archivos de audio recibidos
    arma::Mat <double>  data_matrix( numSamples_1, 2);
    
    
    for( int x = 0; x < numSamples_1; x++ ){
        data_matrix.col(0).row(x) = audioFile_1.samples[channel][x];
    };

    for( int x = 0; x < numSamples_2; x++ ){
        data_matrix.col(1).row(x) = audioFile_2.samples[channel][x];
    };

    //graficar la matriz de datos 
    std::vector < double > sample1 = arma::conv_to< std::vector<double> >::from(data_matrix.col(0));
    std::vector < double > sample2 = arma::conv_to< std::vector<double> >::from(data_matrix.col(1));
	plt::plot(sample1,sample2,"y*");
	plt::title("Original Data Matrix");
	plt::save("DataMatrix.png");
    plt::clf();

    //calcula la matriz de covarianza de la matriz de datos dada
    arma::mat covariance_matrix = arma::cov( data_matrix );
    arma::vec eigvalues;
    arma::mat eigvectors;
    //calcula los eigenvalores y eigenvectores de la matriz dada y los almacena en las variables dadas
    arma::eig_sym(eigvalues, eigvectors, covariance_matrix);
    //crea una matriz cuadrada llena de ceros con tantas filas y columnas como eigenvalores
    arma::mat diagonal_eigvalues = arma::zeros<arma::mat>(eigvalues.n_elem,eigvalues.n_elem);
    //llena la diagonal de la matriz anterior con los eigenvalores
    for( int x = 0; x < eigvalues.n_elem; x++ ){
        diagonal_eigvalues.col(x).row(x) = eigvalues[x];
    };
    //calcula la inversa de la matriz diagonal
    //esta matriz en el caso para la implementacion es 2x2 por lo tanto su calculo no es excesivamente costo
    arma::mat inverse_diagonal_eigvalues = arma::inv( diagonal_eigvalues );
    //crea una matriz de cuadrada llena de ceros con tantas filas y columnas como eigenvalores
    arma::mat inverse_root_diagonal_eigvalues = arma::zeros<arma::mat>(eigvalues.n_elem,eigvalues.n_elem);
    //llena la diagonal de la matriz anterior con la raiz de cada elemento de la matriz diagonal inversa
    for( int x = 0; x < eigvalues.n_elem; x++ ){
        inverse_root_diagonal_eigvalues.col(x).row(x) = arma::sqrt( inverse_diagonal_eigvalues.col(x).row(x) );
    };
    //calcula la matriz transpuesta de la matriz de eigenvectores
    arma::mat transpose_eigvectors = eigvectors.t();
    //multiplica la matriz de datos con la matriz tranpuesta de eigenvectores
    arma::mat transpose_eigvectors_dot_data_matrix = data_matrix*transpose_eigvectors;
    //finaliza la primera etapa calculando la matriz blanqueada como un producto de matrices
    arma::mat whitened_data = transpose_eigvectors_dot_data_matrix*inverse_root_diagonal_eigvalues;

    //graficar la matriz blanqueada
    std::vector < double > wsample1 = arma::conv_to< std::vector<double> >::from(whitened_data.col(0));
    std::vector < double > wsample2 = arma::conv_to< std::vector<double> >::from(whitened_data.col(1));
    plt::plot(wsample1,wsample2,"y*");//#6B0D0D
    plt::title("Whitened Data Matrix");
    plt::save("WhitenedMatrix.png");
    plt::clf();


    //SEGUNDA PARTE
    arma::vec arma_whitened_data_norm_2_axis_0;
    std::vector < double > whitened_data_norm_2_axis_0;
    //se crea una matriz donde se guardaran por filas la norma dos de la matriz blanqueada
    for( int x = 0; x < whitened_data.col(0).n_elem; x++ ){
        whitened_data_norm_2_axis_0.push_back( arma::norm(whitened_data.row(x), 2) );
    };

    arma_whitened_data_norm_2_axis_0 = whitened_data_norm_2_axis_0;
    arma::mat matrix_norm( arma_whitened_data_norm_2_axis_0.n_elem, 2 );
    //en matrix_norm guarda en todas sus columnas las filas de la matriz con la norma 2 de la matriz blanqueada
    for( int x = 0; x < arma_whitened_data_norm_2_axis_0.n_elem; x++ ){
        matrix_norm.row(x).col(0) = arma_whitened_data_norm_2_axis_0[x];
        matrix_norm.row(x).col(1) = arma_whitened_data_norm_2_axis_0[x];
    };
	
    arma::mat m_nomr_multiply_wd_norm( matrix_norm.col(0).n_elem, 2 );
    //multiplica cada norma de la matriz blanqueada con su correspondiente posicion en la matriz blanqueada
    for( int x = 0; x < matrix_norm.col(0).n_elem; x++ ){
        m_nomr_multiply_wd_norm.row(x).col(0) = matrix_norm.row(x).col(0) * whitened_data.row(x).col(0);
        m_nomr_multiply_wd_norm.row(x).col(1) = matrix_norm.row(x).col(1) * whitened_data.row(x).col(1);
    };
    //se calculo la matriz de covarianza de matriz blanqueada por su norma
    arma::mat covariance_m_nomr_multiply_wd_norm = arma::cov( m_nomr_multiply_wd_norm );//(*)

    arma::vec eigvalues_m_nomr_multiply_wd_norm;
    arma::mat eigvectors_m_nomr_multiply_wd_norm;
    //se calcularon los eigenvalores y eigenvectores de la matriz de covarianza anterior(*)
    arma::eig_sym(eigvalues_m_nomr_multiply_wd_norm, eigvectors_m_nomr_multiply_wd_norm, covariance_m_nomr_multiply_wd_norm);
    //calcula la matriz transpuesta de la matriz de eigenvectores de la matriz de covarianza anterior (*)
    arma::mat transposed_eigvectors_m_nomr_multiply_wd_norm = eigvectors_m_nomr_multiply_wd_norm.t();
    //se hace el producto de matriz de eigenvectores con la matriz blanqueada transpuesta
    arma::mat source = eigvectors_m_nomr_multiply_wd_norm*whitened_data.t();

    //graficar las fuentes obtenidas
    std::vector < double > ssample1 = arma::conv_to< std::vector<double> >::from(source.row(0));
    std::vector < double > ssample2 = arma::conv_to< std::vector<double> >::from(source.row(1));

    std::vector<double> time;
    double sampleRate = audioFile_1.getSampleRate();
    
    for(int i=0;i<numSamples_1;i++){
        time.push_back((i/sampleRate)*1000);
    }

    //grafica fuente uno
    plt::plot(time,ssample1);
    plt::title("FOBI Source 1");
    plt::save("FOBISource1.png");
    plt::clf();

    //grafica fuente dos
    plt::plot(time,ssample2);
    plt::title("FOBI Source 2");
    plt::save("FOBISource2.png");
    plt::clf();

    //COMPARACION
    //lee los archivos de audio
    AudioFile<double> original1;
    AudioFile<double> original2;
    original1.load ( resource_forlder + "source1.wav" );
    original2.load ( resource_forlder + "source2.wav" );

    int oSamples_1 = original1.getNumSamplesPerChannel();

    std::vector < double > originalSource1;
    std::vector < double > originalSource2;

    for( int x = 0; x < oSamples_1; x++ ){
        originalSource1.push_back(original2.samples[channel][x]);
        originalSource2.push_back(original1.samples[channel][x]);
        //ssample2[x] = ssample2[x] / 255.0 - 0.5;
    };
    //std::cout << ssample2.size() << std::endl;
    //plt::plot(time,ssample2,"k");
    plt::plot(time,originalSource2,"m");
    plt::title(" original 2");
    plt::save("Original2.png");
    plt::clf();

    //plt::plot(time,ssample1,"k");
    plt::plot(time,originalSource1,"m");
    plt::title(" original  1");
    plt::save("Original1.png");
    plt::clf();


    return 0;
};
