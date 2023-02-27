#include <iostream>
#include <algorithm>
#include <Eigen/Dense>

using  Eigen::VectorXd;
int main(int argc, char** argv){
    double m=0, s=0;
    int n=0,element;
    VectorXd v;
    std::cin>>element;
    while (not std::cin.eof()){
        v <<element;
        n+=1;
        std::cin>>element;
    }
    int mayor = 0;
    while (s<n){
        int contador = 1;
        for (int i=0;i<n;i++){
            if (v[s] == v[i] && i!=s){
                contador+=1;
            }
        }
        if (contador > mayor){
            mayor = contador;
            m = v[s];
        }
        s++;
    }
    std::cout<<"Mode: "<<m<<"is listed "<< mayor<< "times"<<std::endl;
    return 0;
}
