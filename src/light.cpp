#include "light.h"

Light::Light(Vector const& p, float i) : pos(p), intensity(i) {

}

Light::Light() : pos(0, 0, 0), intensity(0) {

}
