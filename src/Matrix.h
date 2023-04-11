#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <cmath>


namespace bla
{
template<int _N, int _M>
class Matrix
{
protected:
    using MatNM = Matrix<_N,_M>;
    std::array<double, _N*_M> m_values{};
    static constexpr int N = _N;
    static constexpr int M = _M;

public:
    constexpr Matrix() {}
    constexpr Matrix(const std::initializer_list<double>& list)
    {
        std::copy(list.begin(), list.end(), m_values.begin());
    }

    constexpr double operator()(int n, int m) const
    {
        return m_values[n + m*_N];
    }
    constexpr double& operator()(int n, int m)
    {
        return m_values[n + m*_N];
    }
    constexpr double operator[](int n) const
    {
        return m_values[n];
    }
    constexpr double& operator[](int n)
    {
        return m_values[n];
    }

    constexpr Matrix<_M,_N> transpose() const
    {
        Matrix<_M,_N> ret;
        for (int j=0; j<_M; ++j)
        {
            for (int i=0; i<_N; ++i)
                ret[j+i*_M] = m_values[i+j*_N];
        }
        return ret;
    }

    constexpr static MatNM zeroes()
    {
        MatNM ret;
        ret.m_values.fill(0);
        return ret;
    }
    constexpr static MatNM ones()
    {
        MatNM ret;
        ret.m_values.fill(1);
        return ret;
    }
    constexpr static MatNM eye()
    {
        MatNM ret;
        for (int i=0; i<std::min(_N,_M); ++i)
            ret(i,i) = 1;
        return ret;
    }

    constexpr MatNM operator-() const
    {
        MatNM ret;
        for (int i=0; i<size(); i++)
            ret[i] = -m_values[i];
        return ret;
    }

    constexpr MatNM& operator+=(double d)
    {
        for (auto& v : m_values) v += d;
        return *this;
    }
    constexpr MatNM& operator-=(double d)
    {
        return *this += -d;
    }

    constexpr MatNM& operator*=(double d)
    {
        for (auto& v : m_values) v *= d;
        return *this;
    }
    constexpr MatNM& operator/=(double d)
    {
        return *this *= (1/d);
    }

    constexpr MatNM& operator+=(const MatNM& rhs)
    {
        for (int i=0; i<size(); ++i) m_values[i] += rhs.m_values[i];
        return *this;
    }
    constexpr MatNM operator+(MatNM rhs) const
    {
        rhs += *this;
        return rhs;
    }
    constexpr MatNM& operator-=(const MatNM& rhs)
    {
        for (int i=0; i<size(); ++i) m_values[i] -= rhs.m_values[i];
        return *this;
    }
    constexpr MatNM operator-(const MatNM& rhs) const
    {
        MatNM ret = *this;
        ret -= rhs;
        return ret;
    }

    constexpr auto begin() const { return m_values.begin(); }
    constexpr auto end() const { return m_values.end(); }
    constexpr auto begin() { return m_values.begin(); }
    constexpr auto end() { return m_values.end(); }

    constexpr int width() const { return N; }
    constexpr int height() const { return M; }
    constexpr int size() const { return N*M; }
};

template<int N, int M> constexpr inline Matrix<N,M> operator+(Matrix<N,M> m, double d) { return m += d; }
template<int N, int M> constexpr inline Matrix<N,M> operator-(Matrix<N,M> m, double d) { return m -= d; }
template<int N, int M> constexpr inline Matrix<N,M> operator*(Matrix<N,M> m, double d) { return m *= d; }
template<int N, int M> constexpr inline Matrix<N,M> operator/(Matrix<N,M> m, double d) { return m /= d; }
template<int N, int M> constexpr inline Matrix<N,M> operator+(double d, Matrix<N,M> m) { return m += d; }
template<int N, int M> constexpr inline Matrix<N,M> operator-(double d, const Matrix<N,M>& m) { return -m + d; }
template<int N, int M> constexpr inline Matrix<N,M> operator*(double d, Matrix<N,M> m) { return m *= d; }


template<int N1, int M, int N2> constexpr
inline Matrix<N1, N2> operator*(const Matrix<N1, M>& a, const Matrix<M, N2>& b)
{
    Matrix<N1, N2> ret;
    for (int j=0; j<N2; ++j)
    {
        for (int k=0; k<M; ++k)
        {
            for (int i=0; i<N1; ++i)
                ret(i, j) += a(i, k) * b(k, j);
        }
    }
    return ret;
}


template<int N, int M>
inline Matrix<N,M> operator+(const Matrix<N, M>& a, const Matrix<N, M>& b)
{
    Matrix<N, M> ret;
    for (int i=0; i<N*M; ++i)
        ret.m_values[i] = a.m_values[i] + b.m_values[i];
    return ret;
}

};


#endif // MATRIX_H
