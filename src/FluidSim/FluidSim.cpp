#include "FluidSim.h"
#include <future>

sf::Color operator*(sf::Color lhs, float s)
{
    return {std::clamp<uint8_t>(lhs.r * s, 0, 255),
            std::clamp<uint8_t>(lhs.g * s, 0, 255),
            std::clamp<uint8_t>(lhs.b * s, 0, 255)};
}

FluidSim::FluidSim()
{
    velocity.fill({});
    density.fill({});
    divergence.fill({});
    potential.fill({});

    // velocity.at(N/2-1, M/2-1) = {5, 0};
    // velocity.at(N/2-1, M/2) = {5, 0};

    _project();

    m_newVelocity = velocity;
    m_newDensity = density;
}

void FluidSim::update(sf::Time dt)
{
    // add sources
    velocity[N/2-1, M/2-1] = {2, 0};
    velocity[N/2-1, M/2] = {2, 0};

    _diffuse(dt);
    _advect(dt);
}

std::pair<sf::Vector2f, float> FluidSim::computeLerpCell(sf::Vector2f pos) const
{
    pos.x = std::clamp(pos.x, 0.f, N-2.f);
    pos.y = std::clamp(pos.y, 0.f, M-2.f);

    float i0 = std::floor(pos.x);
    float i1 = i0 + 1;
    float j0 = std::floor(pos.y);
    float j1 = j0 + 1;

    float ti = pos.x - i0;
    float tj = pos.y - j0;

    return {bilerp(velocity[i0, j0], velocity[i1, j0], velocity[i0, j1], velocity[i1, j1], ti, tj),
            bilerp(density[i0, j0], density[i1, j0], density[i0, j1], density[i1, j1], ti, tj)};
}

void FluidSim::_diffuse(sf::Time dt)
{
    float a = dt.asSeconds() * 0.002f;
    for (int k = 0; k < 20; ++k)
    {
        for (int i = 1; i < N-1; ++i)
        {
            for (int j = 1; j < N-1; ++j)
            {
                m_newVelocity[i, j] = (velocity[i, j] + a * (m_newVelocity[i-1, j] + m_newVelocity[i+1, j] + m_newVelocity[i, j-1] + m_newVelocity[i, j+1])) / (1 + 4*a);
                m_newDensity[i, j] = (density[i, j] + a * (m_newDensity[i-1, j] + m_newDensity[i+1, j] + m_newDensity[i, j-1] + m_newDensity[i, j+1])) / (1 + 4*a);
            }
        }
    }
    std::swap(velocity, m_newVelocity);
    std::swap(density, m_newDensity);
    _setBoundsVelocity();
    _setBoundsDensity();

    _project();
}

void FluidSim::_advect(sf::Time dt)
{
    for (int i = 1; i < N-1; ++i)
    {
        for (int j = 1; j < M-1; ++j)
        {
            sf::Vector2f posPrev = sf::Vector2f{i, j} - velocity[i,j] * dt.asSeconds();
            auto newCell = computeLerpCell(posPrev);
            m_newVelocity[i,j] = newCell.first;
            m_newDensity[i,j] = newCell.second;
        }
    }

    std::swap(velocity, m_newVelocity);
    std::swap(density, m_newDensity);
    _setBoundsVelocity();
    _setBoundsDensity();

    _project();
}

void FluidSim::_project()
{
    std::array<std::array<float, M>, N> p;
    for (auto& c : p)
        c.fill(0);

    _computeDivergence();
    for (int k=0; k<20; ++k)
    {
        for (int i=1; i<N-1; ++i)
        {
            for (int j=1; j<M-1; ++j)
            {
                p[i][j] = (divergence[i,j] + p[i-1][j] + p[i+1][j] + p[i][j-1] + p[i][j+1])/4; // solve grad P = div V
            }
        }
        _setBoundsP();
    }
    for (int i=1; i<N-1; ++i)
    {
        for (int j=1; j<M-1; ++j)
        {
            velocity[i,j].x -= (p[i+1][j] - p[i-1][j]) / 2;
            velocity[i,j].y -= (p[i][j+1] - p[i][j-1]) / 2;
        }
    }
    _setBoundsVelocity();
}

void FluidSim::_setBoundsDensity()
{
    for (int i=1; i<N-1; ++i)
    {
        density[i, 0] = density[i, 1];
        density[i, M-1] = density[i, M-2];
    }
    for (int j=1; j<M-1; ++j)
    {
        density[0, j] = density[1, j];
        density[N-1, j] = density[N-2, j];
    }
    density[0, 0]     = (density[0, 1] + density[1, 0]) / 2;
    density[0, M-1]   = (density[0, M-2] + density[1, M-1]) / 2;
    density[N-1, 0]   = (density[N-1, 1] + density[N-2, 0]) / 2;
    density[N-1, M-1] = (density[N-1, M-2] + density[N-2, M-1]) / 2;
}


void FluidSim::_setBoundsVelocity()
{
    for (int i=1; i<N-1; ++i)
    {
        velocity[i, 0] = velocity[i, 1].componentWiseMul({1, -1});
        velocity[i, M-1] = velocity[i, M-2].componentWiseMul({1, -1});
    }
    for (int j=1; j<M-1; ++j)
    {
        velocity[0, j] = velocity[1, j].componentWiseMul({-1, 1});
        velocity[N-1, j] = velocity[N-2, j].componentWiseMul({-1, 1});
    }
    velocity[0, 0]     = 0.5f * (velocity[0, 1] + velocity[1, 0]);
    velocity[0, M-1]   = 0.5f * (velocity[0, M-2] + velocity[1, M-1]);
    velocity[N-1, 0]   = 0.5f * (velocity[N-1, 1] + velocity[N-2, 0]);
    velocity[N-1, M-1] = 0.5f * (velocity[N-1, M-2] + velocity[N-2, M-1]);
}

void FluidSim::_setBoundsP()
{
    for (int i=1; i<N-1; ++i)
    {
        potential[i, 0] = potential[i, 1];
        potential[i, M-1] = potential[i, M-2];
    }
    for (int j=1; j<M-1; ++j)
    {
        potential[0, j] = potential[1, j];
        potential[N-1, j] = potential[N-2, j];
    }
    potential[0, 0]     = (potential[0, 1] + potential[1, 0]) / 2;
    potential[0, M-1]   = (potential[0, M-2] + potential[1, M-1]) / 2;
    potential[N-1, 0]   = (potential[N-1, 1] + potential[N-2, 0]) / 2;
    potential[N-1, M-1] = (potential[N-1, M-2] + potential[N-2, M-1]) / 2;
}

void FluidSim::_computeDivergence()
{
    for (int i=1; i<N-1; ++i)
    {
        for (int j=1; j<M-1; ++j)
        {
            divergence[i,j] = -(velocity[i+1,j].x - velocity[i-1,j].x + velocity[i,j+1].y - velocity[i,j-1].y) / 2;
        }
    }
    for (int i=1; i<N-1; ++i)
    {
        divergence[i, 0] = divergence[i, 1];
        divergence[i, M-1] = divergence[i, M-2];
    }
    for (int j=1; j<M-1; ++j)
    {
        divergence[0, j] = divergence[1, j];
        divergence[N-1, j] = divergence[N-2, j];
    }
    divergence[0, 0]     = (divergence[0, 1] + divergence[1, 0]) / 2;
    divergence[0, M-1]   = (divergence[0, M-2] + divergence[1, M-1]) / 2;
    divergence[N-1, 0]   = (divergence[N-1, 1] + divergence[N-2, 0]) / 2;
    divergence[N-1, M-1] = (divergence[N-1, M-2] + divergence[N-2, M-1]) / 2;
}
