#include "matrix.h"

#include<algorithm>

Matrix::Matrix(size_t W, size_t H) : w(W), h(H), m(W * H) {
    for(size_t i = 0; i < w * h; i++) {
        m[i] = 0;
    }
}

Matrix::Matrix(Matrix const& b) : w(b.w), h(b.h), m(b.w * b.h) {
    for(size_t i = 0; i < w * h; i++) {
        m[i] = b.m[i];
    }
}

void Matrix::set(size_t l, size_t c, float v) {
    m[l * w + c] = v;
}

const float& Matrix::get(size_t l, size_t c) const {
    return m[l * w + c];
}

float& Matrix::get(size_t l, size_t c) {
    return m[l * w + c];
}

size_t Matrix::getW() const {
    return w;
}

size_t Matrix::getH() const {
    return h;
}

size_t Matrix::size() const {
    return w * h;
}

float& Matrix::operator[](const size_t i) {
    return m[i];
}

const float& Matrix::operator[](const size_t i) const {
    return m[i];
}

Matrix Matrix::identity(size_t w, size_t h) {
    Matrix res(w, h);
    for(size_t i = 0; i < res.size(); i += w + 1) {
        res[i] = 1;
    }
    return res;
}

Matrix& Matrix::operator=(Matrix const& b) {
    w = b.w;
    h = b.h;

    m = std::vector<float>(w * h);

    for(size_t i = 0; i < size(); i++) {
        m[i] = b[i];
    }

    return *this;
}

std::ostream& operator<<(std::ostream& out, Matrix const& matrix) {
    out << "Matrix(";

    for(size_t i = 0; i < matrix.size() - 1; i++) {
        out << matrix[i] << ", ";
    }

    if(matrix.size() > 0) {
        out << matrix[matrix.size() - 1];
    }

    out << ")";
    return out;
}