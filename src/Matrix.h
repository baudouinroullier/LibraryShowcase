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
    std::array<double, _N*_M> m_values{};

public:
    Matrix() { m_values.fill(0); }
    Matrix(const std::initializer_list<double>& list)
    {
        std::copy(list.begin(), list.end(), m_values.begin());
    }
    double operator()(int n, int m=0) const
    {
        return m_values[n + m*_N];
    }
    double& operator()(int n, int m=0)
    {
        return m_values[n + m*_N];
    }
    double operator[](int n) const
    {
        return m_values[n];
    }
    double& operator[](int n)
    {
        return m_values[n];
    }

    Matrix<_M,_N> transpose() const
    {
        Matrix<_M,_N> ret;
        for (int j=0; j<_M; ++j)
        {
            for (int i=0; i<_N; ++i)
                ret[j+i*_M] = m_values[i+j*_N];
        }
        return ret;
    }

    static Matrix<_N, _M> zeroes()
    {
        Matrix<_N, _M> ret;
        ret.m_values.fill(0);
        return ret;
    }
    static Matrix<_N, _M> ones()
    {
        Matrix<_N, _M> ret;
        ret.m_values.fill(1);
        return ret;
    }

    Matrix<_N, _M>& operator*=(double d)
    {
        for (auto& v : m_values) v *= d;
        return *this;
    }
    Matrix<_N, _M> operator*(double d) const
    {
        Matrix<_N, _M> ret = *this;
        ret *= d;
        return ret;
    }
    Matrix<_N, _M>& operator/=(double d)
    {
        return *this *= (1/d);
    }
    Matrix<_N, _M> operator/(double d) const
    {
        return *this * (1/d);
    }

    Matrix<_N, _M>& operator+=(double d)
    {
        for (auto& v : m_values) v += d;
        return *this;
    }
    Matrix<_N, _M> operator+(double d) const
    {
        Matrix<_N, _M> ret = *this;
        ret += d;
        return ret;
    }
    Matrix<_N, _M>& operator-=(double d)
    {
        return *this += -d;
    }
    Matrix<_N, _M> operator-(double d) const
    {
        return *this + (-d);
    }

    Matrix<_N, _M>& operator+=(const Matrix<_N, _M>& rhs)
    {
        for (int i=0; i<size(); ++i) m_values[i] += rhs.m_values[i];
        return *this;
    }
    Matrix<_N, _M> operator+(Matrix<_N, _M> rhs) const
    {
        rhs += *this;
        return rhs;
    }
    Matrix<_N, _M>& operator-=(const Matrix<_N, _M>& rhs)
    {
        for (int i=0; i<size(); ++i) m_values[i] -= rhs.m_values[i];
        return *this;
    }
    Matrix<_N, _M> operator-(Matrix<_N, _M> rhs) const
    {
        Matrix<_N, _M> ret = *this;
        ret -= rhs;
        return ret;
    }

    auto begin() const { return m_values.begin(); }
    auto end() const { return m_values.end(); }
    auto begin() { return m_values.begin(); }
    auto end() { return m_values.end(); }

    int width() const { return N; }
    int height() const { return M; }
    int size() const { return N*M; }

    static constexpr int N = _N;
    static constexpr int M = _M;
};

template<int _N> using Vector = Matrix<_N,1>;
using Vector2 = Vector<2>;
using Vector3 = Vector<3>;


template<int N1, int _M, int N2>
inline Matrix<N1, N2> operator*(const Matrix<N1, _M>& a, const Matrix<_M, N2>& b)
{
    Matrix<N1, N2> ret;
    for (int j=0; j<N2; ++j)
    {
        for (int k=0; k<_M; ++k)
        {
            for (int i=0; i<N1; ++i)
                ret(i, j) += a(i, k) * b(k, j);
        }
    }
    return ret;
}

template<int _N, int _M>
inline Vector<_N> operator*(const Matrix<_N, _M>& a, const Vector<_M>& b)
{
    return operator*<_N, _M, 1>(a, b);
}

template<int _N>
inline double dot(const Vector<_N>& a, const Vector<_N>& b)
{
    return (a.transpose()*b)[0];
}

template<int _N>
inline double norm(const Vector<_N>& a)
{
    return std::sqrt(dot(a,a));
}


template<int _N, int _M>
inline Matrix<_N,_M> operator+(const Matrix<_N, _M>& a, const Matrix<_N, _M>& b)
{
    Matrix<_N, _M> ret;
    for (int i=0; i<_N*_M; ++i)
        ret.m_values[i] = a.m_values[i] + b.m_values[i];
    return ret;
}

};


#endif // MATRIX_H
