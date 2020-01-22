#include<iostream>
#include<fstream>

#include "pixmap.h"
#include "vector.h"
#include "matrix.h"
#include "model.h"

int main() {
    Matrix m = Matrix::identity(4, 4);
    Matrix r = Matrix::identity(4, 4);
    Matrix t = Matrix::identity(4, 4);

    t = t * Matrix::translate(2, 1.5f, -4);
    r = Matrix::rotateZ(3.1415f / 6);

    m = t * r;

    Vector i1(0, 0, 0);
    Vector i2(1, 0, 0);
    Vector i3(0, 1, 0);
    Vector i4(0, 0, 1);

    std::cout << "m: " << m << std::endl;
    std::cout << "r: " << r << std::endl;
    std::cout << "t: " << t << std::endl;

    std::cout << "i1: " << i1 << std::endl;
    std::cout << "i2: " << i2 << std::endl;
    std::cout << "i3: " << i3 << std::endl;
    std::cout << "i4: " << i4 << std::endl;

    std::cout << i2 << std::endl;
    std::cout << r * i2 << std::endl;
    std::cout << m * i2 << std::endl;

    return 0;
}