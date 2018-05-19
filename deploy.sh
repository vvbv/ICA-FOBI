clear
rm build/ica_fobi
g++ -g --std=c++11 *.cpp -o build/ica_fobi -larmadillo -llapack -lblas
./build/ica_fobi