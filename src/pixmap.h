#ifndef PIXMAP_H
#define PIXMAP_H

#include<vector>
#include<cstdlib>

#include "vector.h"

class Pixmap {
    private:
    size_t w;
    size_t h;
    std::vector<unsigned char> map;

    public:
    Pixmap(size_t W, size_t H);

    void setPixel(size_t x, size_t y, Vector const& c);
    Vector getPixel(size_t x, size_t y) const;

    size_t getW() const;
    size_t getH() const;

    size_t size() const;
    unsigned char get(size_t i) const;
    void set(size_t i, unsigned char c);
};

std::ostream& operator<<(std::ostream& out, Pixmap const& map);

#endif