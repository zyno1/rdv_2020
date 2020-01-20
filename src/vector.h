#ifndef VECTOR_H
#define VECTOR_H

#include<cstdlib>
#include<iostream>

class Vector {
    public:
    float x, y, z, w;

    Vector(float X, float Y, float Z);
    Vector(Vector const& v);

    float length() const;

    Vector normalize() const;

    Vector& operator/=(float l);
    Vector& operator*=(float l);

    Vector& operator+=(Vector const& v);
    Vector& operator-=(Vector const& v);

    float& operator[](const std::size_t i);
    const float& operator[](const std::size_t i) const;
};

Vector operator+(Vector const& a, Vector const& b);
Vector operator-(Vector const& a, Vector const& b);
Vector operator-(Vector const& a);
Vector operator*(Vector const& a, float b);
Vector operator*(float b, Vector const& a);

float operator*(Vector const& a, Vector const& b);

Vector cross(Vector const& a, Vector const& b);

std::ostream& operator<<(std::ostream& out, Vector const& v);

#endif