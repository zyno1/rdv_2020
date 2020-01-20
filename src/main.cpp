#include<iostream>

#include "vector.h"

int main() {
    Vector a(10, 11, 12);
    Vector b = a + a;

    std::cout << a << std::endl;
    std::cout << b << " == " << (a + a) << " == " << 2 * a << " == " << a * 2 << std::endl;
    std::cout << a << std::endl;
    a += a;
    std::cout << a << std::endl;
    std::cout << -a << std::endl;
    std::cout << a / a.length() << " == " << a.normalize() << " == " << a * (1.f / a.length()) << std::endl;

    for(int i = 0; i < 4; i++) {
        std::cout << "a[" << i << "] == " << a[i] << std::endl;
    }

    return 0;
}