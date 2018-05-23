# ICA-FOBI
Separación de audio usando el método FOBI( Fourth-Order Blind Identification )

# Dependencias generales

- liblapack3
- liblapack-dev

# Compilación e instalación de dependencias

 > $ git clone https://github.com/vvbv/armadillo-code.git <br>
 $ cd armadillo-code <br>
 $ git checkout 8.500.1 <br>
 $ ./configure <br>
 $ make <br>
 $ sudo make install <br>

# para instalar matplotlib
 $ sudo apt-get install python-matplotlib python-numpy python2.7-dev

# Compilación 
> $./deploy.sh <br>

> <code> g++ --std=c++11 main.cpp -o ica_fobi </code>
