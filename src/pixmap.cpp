#include "pixmap.h"

#include<fstream>

Pixmap::Pixmap(size_t W, size_t H) : w(W), h(H), map(W * H * 3) {

}

void Pixmap::setPixel(size_t x, size_t y, Vector const& c) {
    y = h - y - 1;
    size_t i = (y * w + x) * 3;

    Vector t = c;
    t *= 255;

    map[i] = t[0];
    map[i + 1] = t[1];
    map[i + 2] = t[2];
}

Vector Pixmap::getPixel(size_t x, size_t y) const {
    y = h - y - 1;
    size_t i = (y * w + x) * 3;
    Vector t(map[i], map[i + 1], map[i + 2]);
    return t / 255;
}

size_t Pixmap::size() const {
    return w * h * 3;
}

unsigned char& Pixmap::operator[](const size_t i) {
    return map[i];
}

const unsigned char& Pixmap::operator[](const size_t i) const {
    return map[i];
}

size_t Pixmap::getW() const {
    return w;
}

size_t Pixmap::getH() const {
    return h;
}

unsigned char* Pixmap::data() {
    return map.data();
}

const unsigned char* Pixmap::data() const {
    return map.data();
}

void Pixmap::writeToFile(std::string path) const {
    std::ofstream res(path, std::ios::binary);

    res << *this;

    res.close();
}

std::ostream& operator<<(std::ostream& out, Pixmap const& map) {
    out << "P6\n" << map.getW() << " " << map.getH() << "\n255\n";

    out.write((const char*)map.data(), map.size());

    return out;
}