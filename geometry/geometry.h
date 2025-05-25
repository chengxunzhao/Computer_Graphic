#ifndef __WAVE_FRONT__
#define __WAVE_FRONT__
#include <istream>
#include <cassert>
#include <iostream>

template<int n> struct vec
{
    double data[n] = {0};
    double&       operator[] (int i)       {assert(i >= 0 && i < n); return data[i];};
    const double& operator[] (int i) const {assert(i >= 0 && i < n); return data[i];};
};

template<int n> double operator* (const vec<n> lvec, const vec<n>rvec)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += lvec[i] * rvec[i];
    }
    return sum;
};

template<> struct vec<2>
{
    double x = 0, y = 0;
    double&       operator[] (int i)       {assert(i >= 0 && i < 2); return i ? y : x;};
    const double& operator[] (int i) const {assert(i >= 0 && i < 2); return i ? y : x;};
};

template<> struct vec<3>
{
    double x = 0, y = 0, z = 0;
    double&       operator[] (int i)       {assert(i >= 0 && i < 3); return i == 1 ? y : i ? z : x;};
    const double& operator[] (int i) const {assert(i >= 0 && i < 3); return i == 1 ? y : i ? z : x;};
    /* data */
};

typedef vec<2> vec2;
typedef vec<3> vec3;

#endif

