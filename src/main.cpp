#include<iostream>
#include<fstream>

#include "pixmap.h"
#include "vector.h"
#include "matrix.h"

int main() {
    Matrix m(4, 4);

    std::cout << m << std::endl;

    m = Matrix::identity(4, 4);
    std::cout << m << std::endl;

    m.set(0, 1, 14);
    std::cout << m << std::endl;

    return 0;
}