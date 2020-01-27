#include "matrix.h"

#include<algorithm>
#include<cmath>

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

Matrix Matrix::translate(float x, float y, float z) {
    Matrix res = Matrix::identity(4, 4);

    res.set(0, 3, x);
    res.set(1, 3, y);
    res.set(2, 3, z);

    return res;
}

Matrix Matrix::zoom(float c) {
    Matrix res = Matrix::identity(4, 4);

    res.set(0, 0, c);
    res.set(1, 1, c);
    res.set(2, 2, c);

    return res;
}

Matrix Matrix::rotateX(float a) {
    Matrix res = Matrix::identity(4, 4);

    res.set(1, 1, cos(a));
    res.set(1, 2, -sin(a));
    res.set(2, 1, sin(a));
    res.set(2, 2, cos(a));

    return res;
}

Matrix Matrix::rotateY(float a) {
    Matrix res = Matrix::identity(4, 4);

    res.set(0, 0, cos(a));
    res.set(0, 2, sin(a));
    res.set(2, 0, -sin(a));
    res.set(2, 2, cos(a));

    return res;
}

Matrix Matrix::rotateZ(float a) {
    Matrix res = Matrix::identity(4, 4);

    res.set(0, 0, cos(a));
    res.set(0, 1, -sin(a));
    res.set(1, 0, sin(a));
    res.set(1, 1, cos(a));

    return res;
}

Matrix Matrix::projection(float ar, float fov, float zfar, float znear) {
    const float rfov = 1 / tan(fov);

    Matrix p = Matrix::identity(4, 4);
    
    p.set(0, 0, ar * rfov);
    p.set(1, 1, rfov);
    p.set(2, 2, zfar / (zfar - znear));
    p.set(2, 3, 1);
    p.set(3, 2, - zfar * znear / (zfar - znear));
    p.set(3, 3, 0);

    return p;
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

Matrix operator*(Matrix const& a, Matrix const& b) {
    Matrix res(b.getW(), a.getH());

    for(size_t j = 0; j < res.getH(); j++) {
        for(size_t i = 0; i < res.getW(); i++) {
            float tmp = 0;
            for(size_t k = 0; k < a.getW(); k++) {
                tmp += a.get(j, k) * b.get(k, i);
            }
            res.set(j, i, tmp);
        }
    }

    return res;
}

Vector operator*(Matrix const& a, Vector const& b) {
    Vector res;
    res.w = 1;

    for(size_t j = 0; j < a.getH(); j++) {
        float tmp = 0;
        for(size_t i = 0; i < a.getW(); i++) {
            tmp += a.get(j, i) * b[i];
        }
        res[j] = tmp;
    }

    return res;
}

Vector operator*(Vector const& a, Matrix const& b) {
    Vector res;

    for(size_t i = 0; i < b.getW(); i++) {
        float tmp = 0;
        for(size_t j = 0; j < b.getH(); j++) {
            tmp += a[j] * b.get(j, i);
        }
        res[i] = tmp;
    }

    return res;
}


std::ostream& operator<<(std::ostream& out, Matrix const& matrix) {
    out << "Matrix(";

    for(size_t i = 0; i < matrix.size() - 1; i++) {
        out << matrix[i];

        if((i + 1) % matrix.getW() == 0) {
            out << "| ";
        }
        else {
            out << ", ";
        }
    }

    if(matrix.size() > 0) {
        out << matrix[matrix.size() - 1];
    }

    out << ")";
    return out;
}