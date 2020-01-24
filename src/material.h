#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"

class Material {
    public:
    Vector color;
    float diffuse_light_coeff;
    float specular_light_coeff;
    float specular_light_exponent;

    Material();
    Material(Vector const& c, float dc, float sc, float se);
};

#endif