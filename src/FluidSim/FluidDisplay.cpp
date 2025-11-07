#include "FluidDisplay.h"


FluidDisplay::FluidDisplay(const FluidSim& sim) :
    m_sim(sim)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            float cellBorder = 0;
            int idx = _indexOfC(i, j);
            m_cellsVA[idx + 0].position = sf::Vector2f{m_cellSize * (i - .5f) + cellBorder, m_cellSize * (j - .5f) + cellBorder};
            m_cellsVA[idx + 1].position = \
            m_cellsVA[idx + 3].position = sf::Vector2f{m_cellSize * (i + .5f) - cellBorder, m_cellSize * (j - .5f) + cellBorder};
            m_cellsVA[idx + 2].position = \
            m_cellsVA[idx + 4].position = sf::Vector2f{m_cellSize * (i - .5f) + cellBorder, m_cellSize * (j + .5f) - cellBorder};
            m_cellsVA[idx + 5].position = sf::Vector2f{m_cellSize * (i + .5f) - cellBorder, m_cellSize * (j + .5f) - cellBorder};
        }
    }
}

void FluidDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M - 1; ++j)
        {
            if (const auto [v, free] = m_sim.edgeX(i, j); !free && v == 0)
            {
                int idx = _indexOfX(i, j);
                m_wallsVA[idx + 0].position = sf::Vector2f{m_cellSize * i, m_cellSize * (j)};
                m_wallsVA[idx + 1].position = sf::Vector2f{m_cellSize * i, m_cellSize * (j + 1)};
                m_wallsVA[idx + 0].color = sf::Color::Black;
                m_wallsVA[idx + 1].color = sf::Color::Black;
            }
            else
            {
                // m_wallsVA[_indexOfX(i, j)  ].position = sf::Vector2f{m_cellSize*i, m_cellSize*(j+.5f)};
                // m_wallsVA[_indexOfX(i, j)+1].position = sf::Vector2f{m_cellSize*(i+m_sim.vx(i,j)/2), m_cellSize*(j+.5f)};
                // m_wallsVA[_indexOfX(i, j)  ].color = sf::Color::White;
                // m_wallsVA[_indexOfX(i, j)+1].color = sf::Color::White;
            }
        }
    }
    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (const auto [v, free] = m_sim.edgeY(i, j); !free && v == 0)
            {
                int idx = _indexOfY(i, j);
                m_wallsVA[idx + 0].position = sf::Vector2f{m_cellSize * (i), m_cellSize * j};
                m_wallsVA[idx + 1].position = sf::Vector2f{m_cellSize * (i + 1), m_cellSize * j};
                m_wallsVA[idx + 0].color = sf::Color::Black;
                m_wallsVA[idx + 1].color = sf::Color::Black;
            }
            else
            {
                // m_wallsVA[_indexOfY(i, j)  ].position = sf::Vector2f{m_cellSize*(i+.5f), m_cellSize*j};
                // m_wallsVA[_indexOfY(i, j)+1].position = sf::Vector2f{m_cellSize*(i+.5f), m_cellSize*(j+m_sim.vy(i,j)/2)};
                // m_wallsVA[_indexOfY(i, j)  ].color = sf::Color::White;
                // m_wallsVA[_indexOfY(i, j)+1].color = sf::Color::White;
            }
        }
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            int idx = _indexOfC(i, j);
            m_cellsVA[idx + 0].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[idx + 0].position), 0.5));
            m_cellsVA[idx + 1].color = \
            m_cellsVA[idx + 3].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[idx + 3].position), 0.5));
            m_cellsVA[idx + 2].color = \
            m_cellsVA[idx + 4].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[idx + 4].position), 0.5));
            m_cellsVA[idx + 5].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[idx + 5].position), 0.5));
        }
    }

    states.transform.combine(*this);
    target.draw(m_cellsVA, states);
    target.draw(m_wallsVA, states);
}

int FluidDisplay::_indexOfX(int i, int j) const
{
    return 2 * ((M - 1) * i + j);
}

int FluidDisplay::_indexOfY(int i, int j) const
{
    return 2 * (M * N - N + M * i + j);
}

int FluidDisplay::_indexOfC(int i, int j) const
{
    return 6 * ((M - 1) * i + j);
}
