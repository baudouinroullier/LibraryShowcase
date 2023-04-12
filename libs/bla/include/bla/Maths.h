#ifndef MATHS_H
#define MATHS_H

#include <algorithm>
#include <cmath>

namespace maths
{
static constexpr double epsilon = 1.e-6;

template <class T>
constexpr T clamp(T val, T m1, T m2)
{
    const auto mm = std::minmax(m1,m2);
    return std::max(std::min(val, mm.second), mm.first);
}

template <class T, class U, class V>
constexpr T clamp(T val, U m1, V m2)
{
    return clamp(val, static_cast<T>(m1), static_cast<T>(m2));
}

template <class T, class U>
constexpr T clamp(T val, U m)
{
    return clamp(val, -m, m);
}

/// get interpolation between (x1,y1) and (x2,y2). x values outside [x1,x2] are extrapolated. x1 must be different from x2.
template<class T>
constexpr T affine(T x, T x1, T y1, T x2, T y2)
{
    return (y2 - y1) / (x2 - x1) * (x - x1) + y1;
}

/// get interpolation between (x1,y1) and (x2,y2). x values outside [x1,x2] are clamped to [y1,y2]
template<class T>
constexpr T clampedAffine(T x, T x1, T y1, T x2, T y2)
{
    if (x1 == x2)
        return x < x1 ? y1 : y2;
    return clamp(affine(x, x1, y1, x2, y2), y1, y2);
}

constexpr double degrees(double rad);
constexpr double radians(double deg);

};

#endif // MATHS_H
