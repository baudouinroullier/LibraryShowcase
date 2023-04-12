#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"


namespace bla
{
template<int N> class Vector : public Matrix<N,1>
{
protected:
    using Base = Matrix<N,1>;
public:
    constexpr Vector(const std::initializer_list<double>& list) : Base(list) {}
    constexpr Vector(const Base& m) : Base(m) {}
    constexpr Matrix<1,N> toCovector() const { return this->transpose(); }
    constexpr double operator()(int n) const
    {
        return Base::operator[](n);
    }
    constexpr double& operator()(int n)
    {
        return Base::operator[](n);
    }
};

class Vec2 : public Vector<2>
{
public:
    Vec2(const std::initializer_list<double>& list) : Vector<2>(list) {}
    Vec2(const Vector<2>& v) : Vector<2>(v) {}
    double& x = m_values[0];
    double& y = m_values[1];
};

class Vec3 : public Vector<3>
{
public:
    Vec3(const std::initializer_list<double>& list) : Vector<3>(list) {}
    Vec3(const Vector<3>& v) : Vector<3>(v) {}
    double& x = m_values[0];
    double& y = m_values[1];
    double& z = m_values[2];
};

template<int N, int M> constexpr inline Vector<N> operator*(const Matrix<N, M>& a, const Vector<M>& b) { return operator*<N, M, 1>(a, b); }
template<int N> constexpr inline double dot(const Vector<N>& a, const Vector<N>& b) { return (a.toCovector()*b)[0]; }
template<int N> constexpr inline double norm(const Vector<N>& a) { return std::sqrt(dot(a,a)); }
}

#ifdef ADD_FMT_FORMATTERS
#include <fmt/format.h>

namespace fmt
{
template<int N>
struct formatter<bla::Vector<N>> : public fmt::formatter<double>
{
    template <typename FormatContext>
    auto format(const bla::Vector<N>& vec, FormatContext& ctx) -> decltype (ctx.out())
    {
        ctx.advance_to(fmt::format_to(ctx.out(), "Vector of size {}", N));
        for (int i=0; i<N; ++i)
        {
            ctx.advance_to(fmt::format_to(ctx.out(), "\n  | "));
            ctx.advance_to(fmt::formatter<double>::format(vec(i), ctx));
            ctx.advance_to(fmt::format_to(ctx.out(), " |"));
        }
        return ctx.out();
    }
};


template<> struct formatter<bla::Vec2> : public fmt::formatter<bla::Vector<2>>
{
    template <typename FormatContext>
    auto format(const bla::Vec2& vec, FormatContext& ctx) -> decltype (ctx.out())
    {
        return fmt::formatter<bla::Vector<2>>::format(static_cast<bla::Vector<2>>(vec), ctx);
    }
};
template<> struct formatter<bla::Vec3> : public fmt::formatter<bla::Vector<3>>
{
    template <typename FormatContext>
    auto format(const bla::Vec3& vec, FormatContext& ctx) -> decltype (ctx.out())
    {
        return fmt::formatter<bla::Vector<3>>::format(static_cast<bla::Vector<3>>(vec), ctx);
    }
};

}

#endif

#endif // VECTOR_H
