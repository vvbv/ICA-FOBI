clear
rm build/ica_fobi
g++ -g --std=c++11 *.cpp -o build/ica_fobi -I/usr/include/python2.7 -larmadillo -llapack -lblas -lpython2.7
./build/ica_fobi