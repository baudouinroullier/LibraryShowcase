#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"


namespace bla
{
template<int N> class Vector : public Matrix<N,1>
{
protected:
    using Super = Matrix<N,1>;
public:
    constexpr Vector(const std::initializer_list<double>& list) : Super(list) {}
    constexpr Matrix<1,N> toCovector() const { return this->transpose(); }
    constexpr double operator()(int n) const
    {
        return Super::operator[](n);
    }
    constexpr double& operator()(int n)
    {
        return Super::operator[](n);
    }
};

class Vec2 : public Vector<2>
{
public:
    double& x = m_values[0];
    double& y = m_values[1];
};

class Vec3 : public Vector<3>
{
public:
    double& x = m_values[0];
    double& y = m_values[1];
    double& z = m_values[2];
};

template<int N, int M> constexpr inline Vector<N> operator*(const Matrix<N, M>& a, const Vector<M>& b) { return operator*<N, M, 1>(a, b); }
template<int N> constexpr inline double dot(const Vector<N>& a, const Vector<N>& b) { return (a.toCovector()*b)[0]; }
template<int N> constexpr inline double norm(const Vector<N>& a) { return std::sqrt(dot(a,a)); }
}

#endif // VECTOR_H
