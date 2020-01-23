#include<iostream>
#include<algorithm>

#include "pixmap.h"
#include "vector.h"
#include "matrix.h"
#include "model.h"

void render(Model const& model, Matrix const& matrix, Pixmap& pixmap) {
    const Vector color(1.0f, 0.5f, 0.5f);

    for(size_t i = 0; i < pixmap.size(); i++) {
        pixmap[i] = 50;
    }

    float posCalc = std::min(pixmap.getW(), pixmap.getH()) / 2;

    for(size_t i = 0; i < model.getNbFaces(); i++) {
        std::vector<Vector> v;
        for(size_t j = 0; j < 3; j++) {
            v.push_back(matrix * model.getVertice(i, j));
        }

        Vector min = v[0];
        Vector max = v[0];

        for(size_t j = 1; j < v.size(); j++) {
            min.x = std::min(min.x, v[j].x);
            min.y = std::min(min.y, v[j].y);

            max.x = std::max(max.x, v[j].x);
            max.y = std::max(max.y, v[j].y);
        }

        min = (min * posCalc) + Vector(posCalc, posCalc, 0);
        max = (max * posCalc) + Vector(posCalc, posCalc, 0);

        size_t minx = (size_t)std::max(0, std::min((int)pixmap.getW(), (int)min.x));
        size_t miny = (size_t)std::max(0, std::min((int)pixmap.getH(), (int)min.y));
        size_t maxx = (size_t)std::max(0, std::min((int)pixmap.getW() - 1, (int)max.x));
        size_t maxy = (size_t)std::max(0, std::min((int)pixmap.getH() - 1, (int)max.y));

        for(size_t y = miny; y < maxy; y++) {
            for(size_t x = minx; x < maxx; x++) {
                pixmap.setPixel(x, y, color);
            }
        }
    }
}

int main() {
    Matrix m = Matrix::identity(4, 4);
    Model model("data/duck.obj");
    Pixmap pixmap(1024, 1024);

    m = Matrix::zoom(0.2f) * m;
    //m = Matrix::rotateZ(-2.f) * m;
    //m = Matrix::translate(-1.0f, 0, 0) * m;

    std::cout << model << std::endl;

    render(model, m, pixmap);

    pixmap.writeToFile("res.ppm");

    return 0;
}