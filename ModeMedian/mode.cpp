#include <iostream>
#include <vector>
int main(int argc, char** argv){
    double m=0, s=0;
    int n=0,element;
    std::vector<int> elements;
    std::cin>>element;
    while (not std::cin.eof()){
        elements.push_back(element);
        n+=1;
        std::cin>>element;
    }
    mayor = 0
    while (s<n){
        contador = 1;
        for (int i=0;i<n;i++){
            if (elements[s] == elements[i] && i!=s){
                contador+=1;
            }
        }
        if (contador > mayor){
            mayor = contador;
            m = elements[s];
        }
        s++;
    }
    std::cout<<"Mode: "<<m<<std::endl;
    return 0;
}
