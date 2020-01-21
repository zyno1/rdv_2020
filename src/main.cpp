#include<iostream>
#include<fstream>

#include "pixmap.h"
#include "vector.h"

int main() {
    auto out = std::ofstream("file.ppm", std::ios::binary);

    Pixmap p(100, 90);

    Vector c(0, 0.4f, 0);

    for(size_t j = 0; j < p.getH(); j++) {
        for(size_t i = 0; i < p.getW(); i++) {
            p.setPixel(i, j, c);
        }   
    }

    for(size_t i = 325; i < 375; i++) {
        p[i*3] = 255;
    }

    out << p;

    out.close();

    return 0;
}