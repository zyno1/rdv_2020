#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include<string>
#include<cstdlib>
#include "vector.h"

class Model {
    private:
    std::vector<Vector> vertices;
    std::vector<std::vector<float> > faces;

    public:
    Model(std::string const& file);
    Model();

    size_t getNbVertices() const;
    size_t getNbFaces() const;

    Vector& getVertice(size_t i);
    const Vector& getVertice(size_t i) const;

    Vector& getVertice(size_t fi, size_t j);
    const Vector& getVertice(size_t fi, size_t j) const;

    Model& operator=(Model const& b);
};

std::ostream& operator<<(std::ostream& out, Model const& m);

#endif