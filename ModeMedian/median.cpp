#include <iostream>
#include <algorithm>
#include <Eigen/Dense>

using Eigen::VectorXd;


int main(int argc, char** argv) {
    VectorXd v;
    double m=0, s=0;
    int n=0,element;
    std::cin >> element;
    while (not std::cin.eof()) {
        v.conservativeResize(n+1);
        v(n)=element;
        n +=1;
        std::cin >> element;
    }
    std::sort(v.data(),v.data()+v.size());
    if (v.size()%2==0){
        std::cout<<"even"<<std::endl;
        m = v[v.size()/2-1];
        s = v[v.size()/2];
        std::cout << "Median: "<<m<<" and "<<s<<std::endl;
    }else{
        std::cout<<"odd"<<std::endl;
        m = v[v.size()/2];
        std::cout << "Median: "<<m<<std::endl;
    }
    
    return 0;
}
