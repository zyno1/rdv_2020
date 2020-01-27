#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstring>

#include "pixmap.h"
#include "vector.h"
#include "matrix.h"
#include "model.h"
#include "material.h"
#include "light.h"

float sign (Vector p1, Vector p2, Vector p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool pointInTriangle (Vector pt, Vector v1, Vector v2, Vector v3) {
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

Vector reflect(Vector const& I, Vector const& N) {
    return I - N * 2.f * (I * N);
}

void calcMaxMinVector(std::vector<Vector> const& v, float posCalcX, float posCalcY, Vector & min, Vector & max, float & z) {
    min = v[0];
    max = v[0];

    z = v[0].z;

    for(size_t j = 1; j < v.size(); j++) {
        min.x = std::min(min.x, v[j].x);
        min.y = std::min(min.y, v[j].y);

        max.x = std::max(max.x, v[j].x);
        max.y = std::max(max.y, v[j].y);

        z += v[j].z;
    }
    z /= 3;

    min.x *= posCalcX;
    min.y *= posCalcY;

    max.x = posCalcX;
    max.y *= posCalcY;

    min += Vector(posCalcX, posCalcY, 0);
    max += Vector(posCalcX, posCalcY, 0);
}

void render_triangle(std::vector<Vector> const& v, std::vector<float> & zbuffer, std::vector<Light> const& lights, Material material, Pixmap & pixmap) {
    const float posCalcX = pixmap.getW() / 2;
    const float posCalcY = pixmap.getH() / 2;

    Vector min;
    Vector max;
    float z;

    calcMaxMinVector(v, posCalcX, posCalcY, min, max, z);

    Vector N = cross(v[1] - v[0], v[2] - v[0]).normalize();

    size_t minx = (size_t)std::max(0, std::min((int)pixmap.getW(), (int)min.x));
    size_t miny = (size_t)std::max(0, std::min((int)pixmap.getH(), (int)min.y));
    size_t maxx = (size_t)std::max(0, std::min((int)pixmap.getW() - 1, (int)max.x + 1));
    size_t maxy = (size_t)std::max(0, std::min((int)pixmap.getH() - 1, (int)max.y + 1));

    for(size_t y = miny; y < maxy; y++) {
        for(size_t x = minx; x < maxx; x++) {
            Vector point((x - posCalcX) / posCalcX, (y - posCalcY) / posCalcY, z);
            Vector dir = point.normalize();

            size_t j = y * pixmap.getW() + x;
            
            if(zbuffer[j] < z && pointInTriangle(point, v[0], v[1], v[2])) {
                float diffuse_light_intensity = 0;
                float specular_light_intensity = 0;

                for(auto& light : lights) {
                    Vector light_dir = (light.pos - point).normalize();
                    diffuse_light_intensity  += light.intensity * std::max(0.f, light_dir*N);
                    specular_light_intensity += powf(std::max(0.f, -reflect(-light_dir, N)*dir), material.specular_light_exponent) * light.intensity;
                }

                pixmap.setPixel(x, y, diffuse_light_intensity * material.color * material.diffuse_light_coeff + Vector(1, 1, 1) * specular_light_intensity * material.specular_light_coeff);
                zbuffer[j] = z;
            }
        }
    }
}

void render(Model const& model, Matrix const& matrix, Pixmap& pixmap, std::vector<Light> const& lights, Material const& material) {
    std::vector<float> zbuffer;
    for(size_t i = 0; i < pixmap.getW() * pixmap.getH(); i++) {
        zbuffer.push_back(-(1 << 30));
    }

    for(size_t i = 0; i < pixmap.size(); i++) {
        pixmap[i] = 50;
    }

    for(size_t i = 0; i < model.getNbFaces(); i++) {
        std::vector<Vector> v;
        for(size_t j = 0; j < 3; j++) {
            Vector tmp = model.getVertice(i, j);
            tmp.w = 1;
            tmp = matrix * tmp;
            v.push_back(tmp / tmp.w);
        }

        render_triangle(v, zbuffer, lights, material, pixmap);
    }
}

void renderAnaglyph(Model const& model, Matrix const& projection, Matrix const& matrix, Pixmap& pixmap, std::vector<Light> const& lights, Material const& material) {
    const float eyeSep = 0.2f;
    const float diff = 5.f * eyeSep / 10.f;
    float angle = atan(diff / 5.f);

    Matrix t = Matrix::translate(-diff / 2.f, 0, 0);
    Matrix r = Matrix::rotateY(-angle);

    Matrix m = t * matrix * r;

    Pixmap right(pixmap.getW(), pixmap.getH());

    render(model, projection * m, right, lights, material);

    t = Matrix::translate(diff / 2.f, 0, 0);
    r = Matrix::rotateY(angle);

    m = t * matrix * r;

    Pixmap left(pixmap.getW(), pixmap.getH());

    render(model, projection * m, left, lights, material);

    for(size_t j = 0; j < pixmap.getH(); j++) {
        for(size_t i = 0; i < pixmap.getW(); i++) {
            Vector cl = left.getPixel(i, j);
            Vector cr = right.getPixel(i, j);

            cr[2] = cr[0];
            cr[0] = 0;
            cr[1] = 0;

            cl[1] = 0;
            cl[2] = 0;

            pixmap.setPixel(i, j, cl + cr);
        }
    }
}

int main(int argc, char** argv) {
    Matrix m = Matrix::identity(4, 4);
    Model model("data/duck.obj");
    Pixmap pixmap(1920, 1080);

    for(int i = 1; i < argc; i++) {
        if(0 == std::strcmp("--help", argv[i]) || 0 == std::strcmp("-h", argv[i])) {
            std::cout << "help screen" << std::endl;
            exit(0);
        }
        else if(0 == std::strcmp("-i", argv[i])) {
            //load model from path argv[i+1]
            model = Model(argv[++i]);
        }
        else if(0 == std::strcmp("--zoom", argv[i])) {
            //zoom factor argv[i+1]
        }
        else if(0 == std::strcmp("-x", argv[i])) {
            //x  argv[i+1]
        }
        else if(0 == std::strcmp("-y", argv[i])) {
            //y  argv[i+1]
        }
        else if(0 == std::strcmp("-z", argv[i])) {
            //z  argv[i+1]
        }
    }

    const float fov = M_PI / 3.0;
    const float zfar = 100;
    const float znear = 0.1;
    const float ar = (float)pixmap.getH() / pixmap.getW();

    Matrix p = Matrix::projection(ar, fov, zfar, znear);

    std::vector<Light> lights;
    lights.emplace_back(Vector(10, 10, 1), 2.f);
    lights.emplace_back(Vector(0, 10, 1), 2.f);
    lights.emplace_back(Vector(-10, 10, 1), 2.f);

    for(size_t i = 0; i < lights.size(); i++) {
        lights[i].pos.w = 1;
        lights[i].pos = p * lights[i].pos;
        lights[i].pos = lights[i].pos / -lights[i].pos.w;
    }

    Material material(Vector(0.4f, 0, 0), 0.3f, 0.3f, 500.f);

    m = Matrix::zoom(0.2f) * m;
    //m = Matrix::rotateY(-0.5f) * m;
    m = Matrix::translate(0, 0, -5.f) * m;

    std::cout << model << std::endl;

    render(model, p * m, pixmap, lights, material);
    //renderAnaglyph(model, p, m, pixmap, lights, material);

    pixmap.writeToFile("res.ppm");

    return 0;
}
