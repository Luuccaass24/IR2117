#include <iostream>

int main(){
    int a,b,suma;
    do{
        std::cout<<"Introduce a >=1: ";
        std::cin>>a;
    }while (a<1);
    do{
        std::cout<<"Introduce b >=1: ";
        std::cin>>b;
    }while (b<1);
    for (int i=a;i<=b;i++){
        suma+=i;
    }
    std::cout<<"La suma es: "<<suma<<std::endl;
}
