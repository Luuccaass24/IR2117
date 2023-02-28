#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

int main() {
    
    int rows, cols;
    std::cout<<"Enter matrix size: "<<std::endl;
    std::cin >> rows;
    cols=rows;
    
    MatrixXd m(rows,cols);
    std::cout<<"Enter elements (row-size): "<<std::endl;
    for(int i=0;i<rows;i++){
        for(int j=0;i<cols;j++){
            std::cin>>m(i,j);
        }

    }
    std::cout<<"The matrix is: "<<std::endl;
    std::cout<<m<<std::endl;
    
    VectorXd s(rows);
    for(int i=0;i<rows;i++){
        s(i) = m.row(i).sum();
        std::cout<<"The sum of row is: "<< s << std::endl;
    }
    double MaxSumRow= s.maxCoeff();
    std::cout<<"The max sum of rows is: "<<std::endl;
    for(int i=0;i<cols;i++){
        s(i) = m.col(i).sum();
        std::cout<<"The sum of rows is: "<< s << std::endl;
    }
    double MaxSumCol = s.maxCoeff();
    std::cout<<"The max sum of columns is: "<<std::endl;
    
    double maxRowColSum = MaxSumRow > MaxSumCol ? MaxSumRow : MaxSumCol;
    std::cout<<"The maximum sum of rows or columns is: "<<std::endl;
    return 0;
    
}
