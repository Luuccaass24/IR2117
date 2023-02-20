#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {
    double m=0, s=0;
    int n=0,element;
    std::vector<int> elements;
    std::cin >> element;
    while (not std::cin.eof()) {
        elements.push_back(element);
        n +=1;
        std::cin >> element;
    }
    std::sort(elements);
    if (elements.size()%2==0){
        std::cout<<"even"<<std::endl;
    }else{
        std::cout<<"odd"<<std::endl;
        m = elements[elements.size()/2];
    
    std::cout << "Median: "<<m<<std::endl;
    return 0;
}
