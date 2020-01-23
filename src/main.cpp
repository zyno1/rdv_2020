#include<iostream>
#include<algorithm>

#include "pixmap.h"
#include "vector.h"
#include "matrix.h"
#include "model.h"

float sign (Vector p1, Vector p2, Vector p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool pointInTriangle (Vector pt, Vector v1, Vector v2, Vector v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

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
        size_t maxx = (size_t)std::max(0, std::min((int)pixmap.getW() - 1, (int)max.x + 1));
        size_t maxy = (size_t)std::max(0, std::min((int)pixmap.getH() - 1, (int)max.y + 1));

        for(size_t y = miny; y < maxy; y++) {
            for(size_t x = minx; x < maxx; x++) {
                if(pointInTriangle(Vector((x - posCalc) / posCalc, (y - posCalc) / posCalc, 0), v[0], v[1], v[2])) {
                    pixmap.setPixel(x, y, color);
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
    //m = Matrix::rotateY(-1.f) * m;
    //m = Matrix::translate(-1.0f, 0, 0) * m;

    std::cout << model << std::endl;

    render(model, m, pixmap);

    pixmap.writeToFile("res.ppm");

    return 0;
}
