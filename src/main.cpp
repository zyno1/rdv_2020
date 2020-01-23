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

    for(size_t i = 0; i < model.getNbVertices(); i++) {
        Vector tmp = matrix * model.getVertice(i);
        tmp *= posCalc;

        int x = tmp.x + posCalc;
        int y = tmp.y + posCalc;

        if(x >= 0 && x < pixmap.getW() && y >= 0 && y < pixmap.getH()) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    int tmpx = std::max(0, std::min((int)pixmap.getW() - 1, x + i));
                    int tmpy = std::max(0, std::min((int)pixmap.getH() - 1, y + j));

                    pixmap.setPixel(tmpx, tmpy, color);
                }
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
    m = Matrix::translate(-1.0f, 0, 0) * m;

    std::cout << model << std::endl;

    render(model, m, pixmap);

    pixmap.writeToFile("res.ppm");

    return 0;
}