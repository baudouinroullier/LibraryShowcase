#ifndef FLUIDSIM_H
#define FLUIDSIM_H

#include "Array2.h"
#include <SFML/Graphics.hpp>
#include <fmt/core.h>

sf::Color operator*(sf::Color lhs, float s);

template<class T>
T lerp(const T a, const T b, float t)
{
    return a + (b-a)*t;
}

template<class T>
T bilerp(const T a00, const T a01, const T a10, const T a11, float tx, float ty)
{
    return lerp(lerp(a00, a01, tx), lerp(a10, a11, tx), ty);
}

// template <int _N, int _M>
class FluidSim
{
public:
    static int constexpr N = 40; //_N;
    static int constexpr M = 40; //_M;

    FluidSim();

    void update(sf::Time dt);

    std::pair<sf::Vector2f, float> computeLerpCell(sf::Vector2f pos) const;

    Array2<sf::Vector2f, N, M> velocity;
    Array2<float, N, M> density;
    Array2<float, N, M> divergence;
    Array2<float, N, M> potential;

protected:
    void _diffuse(sf::Time dt);
    void _advect(sf::Time dt);
    void _project();
    void _setBoundsDensity();
    void _setBoundsVelocity();
    void _setBoundsP();

    void _computeDivergence();

    Array2<sf::Vector2f, N, M> m_newVelocity;
    Array2<float, N, M> m_newDensity;
};

#endif // FLUIDSIM_H
