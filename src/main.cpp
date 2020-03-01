#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<string>

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

void renderAnaglyph(float z, float znear, Model const& model, Matrix const& projection, Matrix const& matrix, Pixmap& pixmap, std::vector<Light> const& lights, Material const& material) {
    z = abs(abs(z) - 0);
    const float eyeSep = 0.1f; //a ajuster
    //const float diff = z * eyeSep / 10.f;
    const float diff = eyeSep * (z - znear) / z;
    float angle = atan(diff / z);

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

            float gl = cl[0] * 0.21 + cl[1] * 0.72 + cl[2] * 0.07;
            float gr = cr[0] * 0.21 + cr[1] * 0.72 + cr[2] * 0.07;

            //float gl = (cl[0] + cl[1] + cl[2]) / 3;
            //float gr = (cr[0] + cr[1] + cr[2]) / 3;

            pixmap.setPixel(i, j, Vector(gl, 0, gr));
        }
    }
}

void processArgv(int argc, char** argv, std::string & modelPath, Matrix & m, bool & anaglyph, float& tz, Vector& color) {
    float zoom = 0.8f;
    float tx = 0;
    float ty = 0;
    tz = -5.f;

    anaglyph = true;
    const float brightness = 0.5f;
    color = Vector(brightness, brightness, brightness);

    Matrix r = Matrix::identity(4, 4);

    for(int i = 1; i < argc; i++) {
        if(0 == std::strcmp("--help", argv[i]) || 0 == std::strcmp("-h", argv[i])) {
            std::cout << "help screen" << std::endl;
            std::cout << "-i [str]        specifies the path to the model" << std::endl;
            std::cout << "--zoom [float]  specifies a zoom factor" << std::endl;
            std::cout << "-x [float]      specifies the position for the x coordinate" << std::endl;
            std::cout << "-y [float]      specifies the position for the y coordinate" << std::endl;
            std::cout << "-z [float]      specifies the position for the z coordinate" << std::endl;
            std::cout << "--rx [float]    specifies the rotation in radians around the x axis" << std::endl;
            std::cout << "--ry [float]    specifies the rotation in radians around the y axis" << std::endl;
            std::cout << "--rz [float]    specifies the rotation in radians around the z axis" << std::endl;
            std::cout << "                Note: rotations are done before translations" << std::endl;
            std::cout << "--anaglyph      Use this option to render an anaglyph (default)" << std::endl;
            std::cout << "--normal        Use this for a normal render" << std::endl;
            exit(0);
        }
        else if(0 == std::strcmp("-i", argv[i])) {
            modelPath = argv[++i];
        }
        else if(0 == std::strcmp("--zoom", argv[i])) {
            zoom = std::stof(argv[++i]);
        }
        else if(0 == std::strcmp("-x", argv[i])) {
            tx = std::stof(argv[++i]);
        }
        else if(0 == std::strcmp("-y", argv[i])) {
            ty = std::stof(argv[++i]);
        }
        else if(0 == std::strcmp("-z", argv[i])) {
            tz = std::stof(argv[++i]);
        }
        else if(0 == std::strcmp("--rx", argv[i])) {
            float rx = std::stof(argv[++i]);
            r = Matrix::rotateX(rx) * r;
        }
        else if(0 == std::strcmp("--ry", argv[i])) {
            float ry = std::stof(argv[++i]);
            r = Matrix::rotateY(ry) * r;
        }
        else if(0 == std::strcmp("--rz", argv[i])) {
            float rz = std::stof(argv[++i]);
            r = Matrix::rotateZ(rz) * r;
        }
        else if(0 == std::strcmp("--anaglyph", argv[i])) {
            anaglyph = true;
            color = Vector(brightness, brightness, brightness);
        }
        else if(0 == std::strcmp("--normal", argv[i])) {
            anaglyph = false;
            color = Vector(brightness, 0, 0);
        }
    }

    m = Matrix::translate(tx, ty, tz) * Matrix::zoom(zoom) * r;
}

int main(int argc, char** argv) {
    std::string modelPath = "";
    float tz = -5.f;
    bool anaglyph = true;
    Vector color(0, 0, 0);
    Matrix m = Matrix::identity(4, 4);

    processArgv(argc, argv, modelPath, m, anaglyph, tz, color);

    if(modelPath == "") {
        std::cerr << "Error: no model specified\nexiting" << std::endl;
        exit(1);
    }

    Model model(modelPath);
    Material material(color, 0.3f, 0.3f, 500.f);
    Pixmap pixmap(1920, 1080);

    std::cout << model << std::endl;

    const float fov = M_PI / 3.0;
    const float zfar = 100;
    const float znear = 0.25;
    const float ar = (float)pixmap.getH() / pixmap.getW();

    Matrix projection = Matrix::projection(ar, fov, zfar, znear);

    std::vector<Light> lights;
    lights.emplace_back(Vector(10, 10, 1), 2.f);
    lights.emplace_back(Vector(0, 10, 1), 2.f);
    lights.emplace_back(Vector(-10, 10, 1), 2.f);

    for(size_t i = 0; i < lights.size(); i++) {
        lights[i].pos.w = 1;
        lights[i].pos = projection * lights[i].pos;
        lights[i].pos = lights[i].pos / -lights[i].pos.w;
    }

    if(anaglyph) {
        renderAnaglyph(tz, znear, model, projection, m, pixmap, lights, material);
    }
    else {
        render(model, projection * m, pixmap, lights, material);
    }

    pixmap.writeToFile("res.ppm");

    return 0;
}
