#include "vector.h"

#include<cmath>

Vector::Vector(float X, float Y, float Z) : x(X), y(Y), z(Z), w(1) {
}

Vector::Vector(Vector const& v) : x(v.x), y(v.y), z(v.z), w(v.w) {
}

float Vector::length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector& Vector::operator/=(float l) {
    x /= l;
    y /= l;
    z /= l;
    return *this;
}

Vector& Vector::operator*=(float l) {
    x *= l;
    y *= l;
    z *= l;
    return *this;
}

Vector& Vector::operator+=(Vector const& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector& Vector::operator-=(Vector const& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

float& Vector::operator[](const std::size_t i) {
    return i == 0 ? x : (1 == i ? y : (2 == i ? z : w));
}

const float& Vector::operator[](const std::size_t i) const {
    return i == 0 ? x : (1 == i ? y : (2 == i ? z : w));
}

Vector Vector::normalize() const {
    Vector res(*this);
    res /= res.length();
    return res;
}

Vector operator+(Vector const& a, Vector const& b) {
    Vector res(a);
    res += b;
    return res;
}

Vector operator-(Vector const& a, Vector const& b) {
    Vector res(a);
    res -= b;
    return res;
}

Vector operator-(Vector const& a) {
    return -1 * a;
}

Vector operator*(Vector const& a, float b) {
    Vector res(a);
    res *= b;
    return res;
}

Vector operator*(float b, Vector const& a) {
    return a * b;
}

float operator*(Vector const& a, Vector const& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector operator/(Vector const& a, float b) {
    Vector res(a);
    res /= b;
    return res;
}

Vector cross(Vector const& a, Vector const& b) {
    return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

std::ostream& operator<<(std::ostream& out, Vector const& v) {
    out << "Vector(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out;
}