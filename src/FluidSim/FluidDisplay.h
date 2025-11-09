#ifndef FLUIDDISPLAY_H
#define FLUIDDISPLAY_H

#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <array>
#include "FluidSim.h"

class FluidDisplay : public sf::Drawable, public sf::Transform
{
public:
    static int constexpr N = FluidSim::N;
    static int constexpr M = FluidSim::M;

    FluidDisplay(const FluidSim& sim);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    int _indexOfX(int i, int j) const;
    int _indexOfY(int i, int j) const;
    int _indexOfC(int i, int j) const;

    const FluidSim& m_sim;

    mutable sf::VertexArray m_wallsVA{sf::PrimitiveType::Lines, 2*(2*M*N-N-M)};
    mutable sf::VertexArray m_cellsVA{sf::PrimitiveType::Triangles, 6*(M-1)*(N-1)};
};

#endif // FLUIDDISPLAY_H
