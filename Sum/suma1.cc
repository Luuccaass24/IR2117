#include <iostream>

int Sum(int num){
    int sum=0;
    for (int i=1;i<=num;i++){
        sum+=i;
    }
    return sum;
}

int main(){
    int num;
    std::cout<<"Enter a number: ";
    std::cin>>num;
    std::cout<<"The sum from 1 to "<<num<<" is "<<Sum(num)<<std::endl;
}
