#ifndef __GEOMETRY__
#define __GEOMETRY__
#include <istream>
#include <cassert>
#include <iostream>
template<typename T, int n> struct vec
{
    T data[n] = {0};
    T&       operator[] (int i)       {assert(i >= 0 && i < n); return data[i];};
    const T& operator[] (int i) const {assert(i >= 0 && i < n); return data[i];};
};

template<typename T, int n> T operator* (const vec<T, n> lvec, const vec<T, n>rvec)
{
    T sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += lvec[i] * rvec[i];
    }
    return sum;
};

template<typename T> struct vec<T, 2>
{
    T x = 0, y = 0;
    T&       operator[] (int i)       {assert(i >= 0 && i < 2); return i ? y : x;};
    const T& operator[] (int i) const {assert(i >= 0 && i < 2); return i ? y : x;};
};

template<typename T> struct vec<T, 3>
{
    T x = 0, y = 0, z = 0;
    T&       operator[] (int i)       {assert(i >= 0 && i < 3); return i == 1 ? y : i ? z : x;};
    const T& operator[] (int i) const {assert(i >= 0 && i < 3); return i == 1 ? y : i ? z : x;};
    /* data */
};

typedef vec<double, 2> double_vec2;
typedef vec<double, 3> double_vec3;

#endif // __GEOMETRY__

