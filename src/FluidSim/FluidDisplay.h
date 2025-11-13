#ifndef FLUIDDISPLAY_H
#define FLUIDDISPLAY_H

#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <array>
#include "FluidSim.h"

class FluidDisplay : public sf::Drawable
{
public:
    static int constexpr N = FluidSim::N-2;
    static int constexpr M = FluidSim::M-2;

    FluidDisplay(const FluidSim& sim);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    int _indexOfC(int i, int j) const;

    const FluidSim& m_sim;

    mutable sf::VertexArray m_cellsVA{sf::PrimitiveType::Triangles, 6*M*N};
};

#endif // FLUIDDISPLAY_H
