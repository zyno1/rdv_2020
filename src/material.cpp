#include "material.h"

Material::Material() : color(0, 0, 0), diffuse_light_coeff(0), specular_light_coeff(0), specular_light_exponent(0) {

}

Material::Material(Vector const& c, float dc, float sc, float se) : color(c), diffuse_light_coeff(dc), specular_light_coeff(sc), specular_light_exponent(se) {

}