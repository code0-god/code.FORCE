#include "tensor/tensor.hpp"
#include <iostream>

int main() {
    ace::Tensor<float> A("A", 3, 4);
    printf("[%s] type: %s, Rows: %d, Cols: %d\n", 
        A.name().c_str(), A.typeName().c_str(), A.rows(), A.cols());
}
