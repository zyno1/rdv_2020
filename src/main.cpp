#include<iostream>
#include<fstream>

#include "pixmap.h"
#include "vector.h"
#include "matrix.h"
#include "model.h"

int main() {
    Matrix m(4, 4);

    std::cout << m << std::endl;

    m = Matrix::identity(4, 4);
    std::cout << m << std::endl;

    m.set(0, 1, 14);
    std::cout << m << std::endl;

    Matrix r(3, 2);

    r.set(0, 0, 1);
    r.set(0, 2, 1);
    r.set(1, 1, 1);

    Matrix s(2, 3);

    s.set(0, 0, 1);
    s.set(1, 0, 1);
    s.set(2, 1, 1);

    std::cout << "r: " << r << std::endl;
    std::cout << "s: " << s << std::endl;
    std::cout << "r*s: " << r*s << std::endl;
    std::cout << "s*r: " << s*r << std::endl;

    Model model("data/duck.obj");

    std::cout << model << std::endl;

    return 0;
}