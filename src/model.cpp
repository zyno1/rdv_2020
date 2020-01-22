#include "model.h"

#include<fstream>
#include<iostream>
#include<sstream>

Model::Model(std::string const& file) {
    std::fstream in(file, std::ifstream::in);

    if(!in) {
        std::cerr << "model load fail: " << file << std::endl;
        return;
    }

    std::string line;
    while(in) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vector v;
            for (int i=0;i<3;i++) {
                iss >> v[i];
            }
            vertices.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<float> f;
            int idx, cnt=0;
            iss >> trash;
            while (iss >> idx) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            if (3==cnt) faces.push_back(f);
        }
    }

    in.close();
}

size_t Model::getNbVertices() const {
    return vertices.size();
}

size_t Model::getNbFaces() const {
    return faces.size();
}

Vector& Model::getVertice(size_t i) {
    return vertices[i];
}

const Vector& Model::getVertice(size_t i) const {
    return vertices[i];
}

Vector& Model::getVertice(size_t fi, size_t j) {
    return vertices[faces[fi][j]];
}

const Vector& Model::getVertice(size_t fi, size_t j) const {
    return vertices[faces[fi][j]];
}
