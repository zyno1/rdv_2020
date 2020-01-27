#ifndef MATRIX_H
#define MATRIX_H

#include<cstdlib>
#include<vector>
#include<iostream>

#include "vector.h"

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

    Matrix& operator=(Matrix const& b);

    static Matrix identity(size_t w, size_t h);
    static Matrix translate(float x, float y, float z);
    static Matrix zoom(float c);
    static Matrix rotateX(float a);
    static Matrix rotateY(float a);
    static Matrix rotateZ(float a);
    static Matrix projection(float ar, float fov, float zfar, float znear);
};

Matrix operator*(Matrix const& a, Matrix const& b);

Vector operator*(Matrix const& a, Vector const& b);
Vector operator*(Vector const& a, Matrix const& b);

std::ostream& operator<<(std::ostream& out, Matrix const& matrix);

#endif