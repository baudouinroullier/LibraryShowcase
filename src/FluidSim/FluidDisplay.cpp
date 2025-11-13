#include "FluidDisplay.h"


FluidDisplay::FluidDisplay(const FluidSim& sim) :
    m_sim(sim)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            float cellBorder = 0*0.05f;
            int idx = _indexOfC(i, j);
            m_cellsVA[idx    ].position = sf::Vector2f{i     + cellBorder, j     + cellBorder};
            m_cellsVA[idx + 1].position = sf::Vector2f{i + 1 - cellBorder, j     + cellBorder};
            m_cellsVA[idx + 2].position = sf::Vector2f{i     + cellBorder, j + 1 - cellBorder};
            m_cellsVA[idx + 3].position = sf::Vector2f{i + 1 - cellBorder, j     + cellBorder};
            m_cellsVA[idx + 4].position = sf::Vector2f{i     + cellBorder, j + 1 - cellBorder};
            m_cellsVA[idx + 5].position = sf::Vector2f{i + 1 - cellBorder, j + 1 - cellBorder};
        }
    }
}

void FluidDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            int idx = _indexOfC(i, j);
            for (int k=0; k<6; ++k)
                m_cellsVA[idx + k].color = lerp({64, 64, 64}, sf::Color::White, m_sim.density[i, j]);
        }
    }

    states.transform.translate({-N/2., -M/2.});
    target.draw(m_cellsVA, states);
}

int FluidDisplay::_indexOfC(int i, int j) const
{
    return 6 * (M*i + j);
}
