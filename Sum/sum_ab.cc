#include <iostream>

int main(){
    std::cout<<"Introduce un número >=1: ";
    int a,b,suma;
    std::cin>>a;
    std::cout<<"Introduce un número >=1: ";
    std::cin>>b;
    for(int i=a;i<=b;i++){
        suma+=i;
    }
    std::cout<<"La suma es: "<<suma<<std::endl;
}
