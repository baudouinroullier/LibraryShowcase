#ifndef FLUIDSIM_H
#define FLUIDSIM_H

#include <array>
#include <SFML/Graphics.hpp>
#include <fmt/core.h>

sf::Color operator*(sf::Color lhs, float s);

template<class T>
T lerp(const T a, const T b, float t)
{
    return a + (b-a)*t;
}


struct Edge
{
    float velocity = 0;
    bool isFree = true;
};

struct Cell
{
    float divergence = 0;
    float density = 0;
    int freeNeigbours = 0;
};

// template <int _N, int _M>
class FluidSim
{
public:
    static int constexpr N = 40; //_N;
    static int constexpr M = 40; //_M;

    FluidSim();

    void update(sf::Time dt);

    float computeVelocityX(sf::Vector2f pos) const;

    float computeVelocityY(sf::Vector2f pos) const;

    sf::Vector2f computeVelocity(sf::Vector2f pos) const;

    float computeDensity(sf::Vector2f pos) const;

    Edge edgeX(int i, int j) const;
    Edge edgeY(int i, int j) const;
    float vx(int i, int j) const;
    float vy(int i, int j) const;
    float div(int i, int j) const;
    float density(int i, int j) const;
    Edge& edgeX(int i, int j);
    Edge& edgeY(int i, int j);
    float& vx(int i, int j);
    float& vy(int i, int j);
    float& div(int i, int j);
    float& density(int i, int j);

protected:
    void _forceNullDivergence();

    void _advect(sf::Time dt);

    float _computeDivergence();

    void _spreadDivergence();
    void _spreadDivergenceRed();
    void _spreadDivergenceBlack();
    void _spreadDivergence(int i, int j);

    std::array<std::array<Edge, M-1>, N> m_edgesX;
    std::array<std::array<Edge, M>, N-1> m_edgesY;
    std::array<std::array<Cell, M-1>, N-1> m_cells;
};

#endif // FLUIDSIM_H
