#ifndef __GEOMETRY__
#define __GEOMETRY__
#include <istream>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <cmath>
/* comman template to vec 
   vec , data type T, vec size n*/

template<typename T, int n>
struct vec
{
    T data[n] = {0};
    T&       operator[] (int i)       {assert(i >= 0 && i < n); return data[i];};
    const T& operator[] (int i) const {assert(i >= 0 && i < n); return data[i];};
};

template<typename T1, typename T2, int n>
vec<std::common_type_t<T1, T2>, n> 
operator+ (const vec<T1, n>& lhs, const vec<T2, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    vec<R, n> vect;
    for (int i = 0; i < n; i++)
    {
        vect[i] = lhs[i] + rhs[i];
    }
    return vect;
}

template<typename T1, typename T2, int n>
vec<std::common_type_t<T1, T2>, n>
operator- (const vec<T1, n>& lhs, const vec<T2, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    vec<R, n> vect;
    for (int i = 0; i < n; i++)
    {
        vect[i] = lhs[i] - rhs[i];
    }
    return vect;
}


template<typename T1, typename T2, int n>
vec<std::common_type_t<T1, T2>, n>
operator* (const vec<T1, n>& lhs, const T2& rhs)
{
    using R = std::common_type_t<T1, T2>;
    vec<R, n> vect;
    for (int i = 0; i < n; i++)
    {
        vect[i] = lhs[i] * rhs;
    }
    return vect;
}

template<typename T1, typename T2, int n>
vec<std::common_type_t<T1, T2>, n>
operator* (const T1& lhs, const vec<T2, n>& rhs)
{
    return rhs * lhs;
}

template<typename T1, typename T2, int n>
std::common_type_t<T1, T2>
operator* (const vec<T1, n>& lhs, const vec<T2, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    R sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += static_cast<R>(lhs[i]) * static_cast<R>(rhs[i]);
    }
    return sum;
}

template<typename T, int n>
std::common_type_t<T, double>
length(const vec<T, n>& vect)
{
    using R = std::common_type_t<T, double>;
    R sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += static_cast<R>(vect[i]) * static_cast<R>(vect[i]);
    }
    return std::sqrt(sum);
}

template<typename T, int n>
vec<std::common_type_t<T, double>, n>
normalize(const vec<T, n>& vect)
{
    using R = std::common_type_t<T, double>;
    R len = length(vect);
    vec<R, n> rvec;
    if (len != 0)
    {
        for (int i = 0; i < n; i++)
        {
            rvec[i] = static_cast<R>(vect[i]) / len;
        }
    }
    return rvec;
}


template<typename T> struct vec<T, 2>
{
    T x = 0, y = 0;
    T&       operator[] (int i)       {assert(i >= 0 && i < 2); return i ? y : x;};
    const T& operator[] (int i) const {assert(i >= 0 && i < 2); return i ? y : x;};

    vec<T, 2>() {};
    vec<T, 2>(T x, T y) : x(x), y(y) {};
};

template<typename T1, typename T2>
std::common_type_t<T1, T2>
operator^ (const vec<T1, 2>& lhs, const vec<T2, 2>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    return static_cast<R>(lhs.x) * static_cast<R>(rhs.y) - static_cast<R>(lhs.y) * static_cast<R>(rhs.x);
}

template<typename T> struct vec<T, 3>
{
    T x = 0, y = 0, z = 0;
    T&       operator[] (int i)       {assert(i >= 0 && i < 3); return i == 1 ? y : i ? z : x;};
    const T& operator[] (int i) const {assert(i >= 0 && i < 3); return i == 1 ? y : i ? z : x;};
    
    vec<T, 3>() {};
    vec<T, 3>(T x, T y, T z) : x(x) , y(y), z(z) {};
};

template<typename T1, typename T2>
vec<std::common_type_t<T1, T2>, 3>
operator^ (const vec<T1, 3>& lhs, const vec<T2, 3>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    vec<R, 3> vect;
    vect.x = static_cast<R>(lhs.y) * static_cast<R>(rhs.z) - static_cast<R>(lhs.z) * static_cast<R>(rhs.y);
    vect.y = static_cast<R>(lhs.x) * static_cast<R>(rhs.z) - static_cast<R>(lhs.z) * static_cast<R>(rhs.x);
    vect.z = static_cast<R>(lhs.x) * static_cast<R>(rhs.y) - static_cast<R>(lhs.y) * static_cast<R>(rhs.x);
    return vect;
}

typedef vec<double, 2> double_vec2;
typedef vec<double, 3> double_vec3;
typedef vec<int, 2>    int_vec2;
typedef vec<int, 3>    int_vec3;
typedef vec<float, 2>  float_vec2;
typedef vec<float, 3>  float_vec3;


template<typename T, int m, int n>
struct mat
{
    vec<T, n> data[m] = {};

    vec<T, n>& operator[](int i) {assert(i >= 0 && i <m); return data[i];}
    const vec<T, n>& operator[](int i) const {assert(i >= 0 && i <m); return data[i];}
};

template<typename T, int m, int n>
vec<T, n> row(const mat<T, m, n>& rmat, int j)
{
    assert(j >= 0 && j < m);
    return rmat[j];
}

template<typename T, int m, int n>
vec<T, m> col(const mat<T, m, n>& rmat, int j)
{
    assert(j >= 0 && j < n);
    vec<T, m> rvec;
    for (int i = 0; i < m; i++)
    {
        rvec[i] = rmat[i][j];
    }
    return rvec;
}

template<typename T1, typename T2, int m, int n>
mat<std::common_type_t<T1, T2>, m, n>
operator+ (const mat<T1, m, n>& lhs, const mat<T2, m, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    mat<R, m, n> ret;
    for (int i = 0; i < m; i++)
    {
        ret[i] = lhs[i] + rhs[i];
    }
    return ret;
}

template<typename T1, typename T2, int m, int n>
mat<std::common_type_t<T1, T2>, m, n>
operator- (const mat<T1, m, n>& lhs, const mat<T2, m, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    mat<R, m, n> ret;
    for (int i = 0; i < m; i++)
    {
        ret[i] = lhs[i] - rhs[i];
    }
    return ret;
}

template<typename T1, typename T2, int m, int n>
mat<std::common_type_t<T1, T2>, m, n>
operator* (const mat<T1, m, n>& lhs, const T2& rhs)
{
    using R = std::common_type_t<T1, T2>;
    mat<R, m, n> ret;
    for (int i = 0; i < m; i++)
    {
        ret[i] = lhs[i] * rhs;
    }
    return ret;
}

template<typename T1, typename T2, int m, int n>
mat<std::common_type_t<T1, T2>, m, n>
operator* (const T1& lhs, const mat<T2, m, n>& rhs)
{
    return rhs * lhs;
}

template<typename T1, typename T2, int m, int n>
vec<std::common_type_t<T1, T2>, m>
operator* (const mat<T1, m, n>& lhs, const vec<T2, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    vec<R, m> ret;
    for (int i = 0; i < m; i++)
    {
        ret[i] = lhs[i] * rhs;
    }
    return ret;
}

template<typename T1, typename T2, int m, int n>
vec<std::common_type_t<T1, T2>, n>
operator* (const vec<T1, m>& lhs, const mat<T2, m, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    vec<R, n> ret;
    for (int i = 0; i < m; i++)
    {
        ret[i] = lhs * col(rhs, i);
    }
    return ret;
}

template<typename T1, typename T2, int m, int k, int n>
mat<std::common_type_t<T1, T2>, m, n>
operator*(const mat<T1, m, k>& lhs, const mat<T2, k, n>& rhs)
{
    using R = std::common_type_t<T1, T2>;
    mat<R, m, n> ret;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            ret[i][j] = lhs[i] * col(rhs, j);
    return ret;
}


template<typename T, int m, int n>
mat<T, n, m> transpose(const mat<T, m, n>& src)
{
    mat<T, n, m> ret;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            ret[j][i] = src[i][j];
    return ret;
}

template<typename T>
mat<T, 2, 2> inverse(const mat<T, 2, 2>& m)
{
    T det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    assert(det != T(0));
    T invDet = T(1) / det;

    mat<T, 2, 2> ret;
    ret[0][0] =  m[1][1] * invDet;
    ret[0][1] = -m[0][1] * invDet;
    ret[1][0] = -m[1][0] * invDet;
    ret[1][1] =  m[0][0] * invDet;
    return ret;
}

template<typename T>
mat<T, 3, 3> inverse(const mat<T, 3, 3>& m)
{
    vec<T, 3> a = m[0];
    vec<T, 3> b = m[1];
    vec<T, 3> c = m[2];

    vec<T, 3> r0 = cross(b, c);
    vec<T, 3> r1 = cross(c, a);
    vec<T, 3> r2 = cross(a, b);

    T det = a * r0;  // a . (b x c)
    assert(det != T(0));
    T invDet = T(1) / det;

    mat<T, 3, 3> ret;
    ret[0] = r0 * invDet;
    ret[1] = r1 * invDet;
    ret[2] = r2 * invDet;
    return transpose(ret);  // 注意转置
}

template<typename T>
mat<T, 4, 4> inverse(const mat<T, 4, 4>& m)
{
    const auto& a = m[0];
    const auto& b = m[1];
    const auto& c = m[2];
    const auto& d = m[3];

    // 前两行组成矩阵
    T A = a[0]*b[1] - a[1]*b[0];
    T B = a[0]*b[2] - a[2]*b[0];
    T C = a[0]*b[3] - a[3]*b[0];
    T D = a[1]*b[2] - a[2]*b[1];
    T E = a[1]*b[3] - a[3]*b[1];
    T F = a[2]*b[3] - a[3]*b[2];

    // 后两行组成矩阵
    T G = c[0]*d[1] - c[1]*d[0];
    T H = c[0]*d[2] - c[2]*d[0];
    T I = c[0]*d[3] - c[3]*d[0];
    T J = c[1]*d[2] - c[2]*d[1];
    T K = c[1]*d[3] - c[3]*d[1];
    T L = c[2]*d[3] - c[3]*d[2];

    // 行列式
    T det = A * L - B * K + C * J + D * I - E * H + F * G;
    assert(det != T(0));
    T invDet = T(1) / det;

    mat<T, 4, 4> ret;

    ret[0][0] = ( b[1]*L - b[2]*K + b[3]*J) * invDet;
    ret[0][1] = (-a[1]*L + a[2]*K - a[3]*J) * invDet;
    ret[0][2] = ( d[1]*F - d[2]*E + d[3]*D) * invDet;
    ret[0][3] = (-c[1]*F + c[2]*E - c[3]*D) * invDet;

    ret[1][0] = (-b[0]*L + b[2]*I - b[3]*H) * invDet;
    ret[1][1] = ( a[0]*L - a[2]*I + a[3]*H) * invDet;
    ret[1][2] = (-d[0]*F + d[2]*C - d[3]*B) * invDet;
    ret[1][3] = ( c[0]*F - c[2]*C + c[3]*B) * invDet;

    ret[2][0] = ( b[0]*K - b[1]*I + b[3]*G) * invDet;
    ret[2][1] = (-a[0]*K + a[1]*I - a[3]*G) * invDet;
    ret[2][2] = ( d[0]*E - d[1]*C + d[3]*A) * invDet;
    ret[2][3] = (-c[0]*E + c[1]*C - c[3]*A) * invDet;

    ret[3][0] = (-b[0]*J + b[1]*H - b[2]*G) * invDet;
    ret[3][1] = ( a[0]*J - a[1]*H + a[2]*G) * invDet;
    ret[3][2] = (-d[0]*D + d[1]*B - d[2]*A) * invDet;
    ret[3][3] = ( c[0]*D - c[1]*B + c[2]*A) * invDet;

    return ret;
}


typedef mat<double, 2, 2> double_mat2;
typedef mat<double, 3, 3> double_mat3;
typedef mat<double, 4, 4> double_mat4;

typedef mat<int, 2, 2> int_mat2;
typedef mat<int, 3, 3> int_mat3;
typedef mat<int, 4, 4> int_mat4;

#endif // __GEOMETRY__

