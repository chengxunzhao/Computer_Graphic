#ifndef __MODEL__
#define __MODEL__
#include <fstream>
#include <vector>
#include <cassert>
#include <iostream>
#include "geometry.h"

struct Model
{
    Model(std::string filename);

    int fragment_size() const;
    int vertex_size() const;

    double_vec3 vetx(const int i) const;
    double_vec3 fragment_vert(const int i_fragment, const int i_index);

private:
    std::vector<double_vec3>   vertexs         = {double_vec3(0, 0, 0)};
    std::vector<int_vec3>      fragments_index = {};
};

#endif // __MODEL__