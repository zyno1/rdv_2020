#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {
    public:
    Vector pos;
    float intensity;

    Light(Vector const& p, float i);
    Light();
};

#endif