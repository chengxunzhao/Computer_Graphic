#ifndef __MODEL__
#define __MODEL__
#include <fstream>
#include <vector>
#include <cassert>
#include <iostream>

struct vertex
{
    double x = 0, y = 0, z = 0;
    double&       operator[] (int i)       {assert(i >=0 && i < 3); return i == 1 ? y : i ? z : x; };
    const double& operator[] (int i) const {assert(i >=0 && i < 3); return i == 1 ? y : i ? z : x; };
};

struct fragment
{
    int a = 0, b = 0, c = 0;
    int&       operator[] (int i)       {assert(i >= 0 && i < 3); return i == 1 ? b : i ? c : a; };
    const int& operator[] (int i) const {assert(i >= 0 && i < 3); return i == 1 ? b : i ? c : a; };
};



struct Model
{
    Model(std::string filename);

    int fragment_size() const;
    int vertex_size() const;

    vertex  vetx(const int i) const;
    vertex  fragment_vert(const int i_fragment, const int i_index);

private:
    std::vector<vertex>   vertexs         = {vertex{}};
    std::vector<fragment> fragments_index = {};
};

#endif // __MODEL__