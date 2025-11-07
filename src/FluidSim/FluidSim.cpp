#include "FluidSim.h"

sf::Color operator*(sf::Color lhs, float s)
{
    return {static_cast<uint8_t>(std::clamp<float>(lhs.r * s, 0, 255)),
            static_cast<uint8_t>(std::clamp<float>(lhs.g * s, 0, 255)),
            static_cast<uint8_t>(std::clamp<float>(lhs.b * s, 0, 255))};
}

FluidSim::FluidSim()
{
    m_edgesX.front().fill({1, false});
    m_edgesX.back().fill({1, false});


    for (int j = M / 4; j < M - 1 - M / 4; ++j)
    {
        edgeX(N/2, j) = {0, false};
        edgeX(N/2 - 1, j) = {0, false};
    }

    for (int i = 0; i < N - 1; ++i)
        m_edgesY[i].front() = m_edgesY[i].back() = {0, false};


    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = 0; j < M - 1; ++j)
        {
            m_cells[i][j].freeNeigbours = edgeX(i + 1, j).isFree + edgeX(i, j).isFree + edgeY(i, j + 1).isFree + edgeY(i, j).isFree;
        }
    }

    _computeDivergence();
}

void FluidSim::update(sf::Time dt)
{
    _advect(dt);
    _forceNullDivergence();
}

float FluidSim::computeVelocityX(sf::Vector2f pos) const
{
    float x = pos.x / m_cellSize;
    float y = pos.y / m_cellSize;

    if (x < 0 || x > N - 1 || y < 0 || y > M - 1)
        return {};

    float i = std::floor(x);
    float i1 = std::min(i + 1, N - 1.f);
    float j = std::max(std::floor(y - 0.5), 0.);
    float j1 = std::min(std::floor(y + 0.5f), M - 2.f);

    float ti = x - i;
    float tj = y - j - .5f;

    float v1 = lerp(vx(i, j), vx(i1, j), ti);
    float v2 = lerp(vx(i, j1), vx(i1, j1), ti);
    return lerp(v1, v2, tj);
}

float FluidSim::computeVelocityY(sf::Vector2f pos) const
{
    float x = pos.x / m_cellSize;
    float y = pos.y / m_cellSize;

    if (x < 0 || x > N - 1 || y < 0 || y > M - 1)
        return {};

    float i = std::max(std::floor(x - 0.5), 0.);
    float i1 = std::min(std::floor(x + 0.5f), N - 2.f);
    float j = std::floor(y);
    float j1 = std::min(j + 1, M - 1.f);

    float ti = x - i - .5f;
    float tj = y - j;

    float v1 = lerp(vy(i, j), vy(i1, j), ti);
    float v2 = lerp(vy(i, j1), vy(i1, j1), ti);
    return lerp(v1, v2, tj);
}

sf::Vector2f FluidSim::computeVelocity(sf::Vector2f pos) const
{
    return {computeVelocityX(pos), computeVelocityY(pos)};
}

float FluidSim::computeDensity(sf::Vector2f pos) const
{
    float x = pos.x / m_cellSize;
    float y = pos.y / m_cellSize;

    if (x < 0 || x > N - 1 || y < 0 || y > M - 1)
        return {};

    float i = std::max(std::floor(x - 0.5), 0.);
    float i1 = std::min(std::floor(x + 0.5), N - 2.);
    float j = std::max(std::floor(y - 0.5), 0.);
    float j1 = std::min(std::floor(y + 0.5), M - 2.);

    float ti = x - i - .5;
    float tj = y - j - .5;

    float d1 = lerp(density(i, j), density(i1, j), ti);
    float d2 = lerp(density(i, j1), density(i1, j1), ti);
    float d = lerp(d1, d2, tj);

    return d;
}

Edge FluidSim::edgeX(int i, int j) const
{
    return m_edgesX[i][j];
}

Edge FluidSim::edgeY(int i, int j) const
{
    return m_edgesY[i][j];
}

float FluidSim::vx(int i, int j) const
{
    return m_edgesX[i][j].velocity;
}

float FluidSim::vy(int i, int j) const
{
    return m_edgesY[i][j].velocity;
}

float FluidSim::div(int i, int j) const
{
    return m_cells[i][j].divergence;
}

float FluidSim::density(int i, int j) const
{
    return m_cells[i][j].density;
}

Edge& FluidSim::edgeX(int i, int j)
{
    return m_edgesX[i][j];
}

Edge& FluidSim::edgeY(int i, int j)
{
    return m_edgesY[i][j];
}

float& FluidSim::vx(int i, int j)
{
    return m_edgesX[i][j].velocity;
}

float& FluidSim::vy(int i, int j)
{
    return m_edgesY[i][j].velocity;
}

float& FluidSim::div(int i, int j)
{
    return m_cells[i][j].divergence;
}

float& FluidSim::density(int i, int j)
{
    return m_cells[i][j].density;
}

void FluidSim::_forceNullDivergence()
{
    double div = _computeDivergence();
    while (div > N * M / 5000.)
    {
        _spreadDivergence();
        div = _computeDivergence();
    }
}

void FluidSim::_advect(sf::Time dt)
{
    auto edgesXTmp = m_edgesX;
    auto edgesYTmp = m_edgesY;
    auto cellsTmp = m_cells;

    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = 0; j < M - 1; ++j)
        {
            sf::Vector2f pos{m_cellSize * (i + .5f), m_cellSize * (j + .5f)};
            sf::Vector2f velocity{vx(i, j) + vx(i + 1, j), vy(i, j) + vy(i, j + 1)};
            sf::Vector2f posPrev = pos - velocity * .5f * dt.asSeconds();
            cellsTmp[i][j].density = computeDensity(posPrev);
        }
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M - 1; ++j)
        {
            if (!edgeX(i, j).isFree)
                continue;
            sf::Vector2f pos{m_cellSize * i, m_cellSize * (j + .5f)};
            sf::Vector2f velocity = {vx(i, j), computeVelocityY(pos)};
            sf::Vector2f posPrev = pos - velocity * dt.asSeconds();
            edgesXTmp[i][j].velocity = computeVelocity(posPrev).x;
        }
    }

    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (!edgeY(i, j).isFree)
                continue;
            sf::Vector2f pos{m_cellSize * (i + .5f), m_cellSize * j};
            sf::Vector2f velocity = {computeVelocityX(pos), vy(i, j)};
            sf::Vector2f posPrev = pos - velocity * dt.asSeconds();
            edgesYTmp[i][j].velocity = computeVelocity(posPrev).y;
        }
    }

    m_edgesX = edgesXTmp;
    m_edgesY = edgesYTmp;
    m_cells = cellsTmp;
}

float FluidSim::_computeDivergence()
{
    float totalDiv = 0;
    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = 0; j < M - 1; ++j)
        {
            div(i, j) = (vx(i + 1, j) - vx(i, j) + vy(i, j + 1) - vy(i, j)) / 2;
            totalDiv += std::abs(div(i, j));
        }
    }
    return totalDiv;
}

void FluidSim::_spreadDivergence()
{
    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = 0; j < M - 1; ++j)
        {
            float s = m_cells[i][j].freeNeigbours;
            if (s == 0)
                continue;

            float d = 2 * div(i, j);
            vx(i + 1, j) -= d * edgeX(i + 1, j).isFree / s;
            vx(i, j) += d * edgeX(i, j).isFree / s;
            vy(i, j + 1) -= d * edgeY(i, j + 1).isFree / s;
            vy(i, j) += d * edgeY(i, j).isFree / s;
        }
    }
}
