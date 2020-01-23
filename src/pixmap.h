#ifndef PIXMAP_H
#define PIXMAP_H

#include<vector>
#include<string>
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
    unsigned char& operator[](const size_t i);
    const unsigned char& operator[](const size_t i) const;

    unsigned char* data();
    const unsigned char* data() const;

    void writeToFile(std::string path) const;
};

std::ostream& operator<<(std::ostream& out, Pixmap const& map);

#endif