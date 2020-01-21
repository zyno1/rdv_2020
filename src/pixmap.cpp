#include "pixmap.h"

Pixmap::Pixmap(size_t W, size_t H) : w(W), h(H), map(W * H * 3) {

}

void Pixmap::setPixel(size_t x, size_t y, Vector const& c) {
    size_t i = (y * w + x) * 3;

    Vector t = c;
    t *= 255;

    map[i] = t[0];
    map[i + 1] = t[1];
    map[i + 2] = t[2];
}

Vector Pixmap::getPixel(size_t x, size_t y) const {
    size_t i = (y * w + x) * 3;
    Vector t(map[i], map[i + 1], map[i + 2]);
    return t / 255;
}

size_t Pixmap::size() const {
    return w * h * 3;
}

unsigned char Pixmap::get(size_t i) const {
    return map[i];
}

void Pixmap::set(size_t i, unsigned char c) {
    map[i] = c;
}

size_t Pixmap::getW() const {
    return w;
}

size_t Pixmap::getH() const {
    return h;
}

std::ostream& operator<<(std::ostream& out, Pixmap const& map) {
    out << "P6\n" << map.getW() << " " << map.getH() << "\n255\n";

    for(size_t i = 0; i < map.size(); i++) {
        out << (char)map.get(i);
    }

    return out;
}