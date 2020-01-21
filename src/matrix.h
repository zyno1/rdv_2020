#ifndef MATRIX_H
#define MATRIX_H

#include<cstdlib>
#include<vector>
#include<iostream>

class Matrix {
    public:
    size_t w;
    size_t h;
    std::vector<float> m;

    Matrix(size_t W, size_t H);
    Matrix(Matrix const& b);

    void set(size_t l, size_t c, float v);
    const float& get(size_t l, size_t c) const;
    float& get(size_t l, size_t c);

    size_t getW() const;
    size_t getH() const;

    size_t size() const;
    float& operator[](const size_t i);
    const float& operator[](const size_t i) const;

    static Matrix identity(size_t w, size_t h);

    Matrix& operator=(Matrix const& b);
};

std::ostream& operator<<(std::ostream& out, Matrix const& matrix);

#endif