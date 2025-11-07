#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <random>
#include <array>
#include <Interact/ArrowShape.h>
#include "FluidSim.h"


template <int _N, int _M>
class Display : public sf::Drawable, public sf::Transform
{
public:
    static int constexpr N = _N;
    static int constexpr M = _M;

    Display(const FluidSim<_N,_M>& sim) : m_sim(sim)
    {
        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                float cellBorder = 0;
                m_cellsVA[_indexOfC(i, j)  ].position = sf::Vector2f{m_cellSize* i    + cellBorder, m_cellSize* j    + cellBorder};
                m_cellsVA[_indexOfC(i, j)+1].position = sf::Vector2f{m_cellSize*(i+1) - cellBorder, m_cellSize* j    + cellBorder};
                m_cellsVA[_indexOfC(i, j)+2].position = sf::Vector2f{m_cellSize* i    + cellBorder, m_cellSize*(j+1) - cellBorder};
                m_cellsVA[_indexOfC(i, j)+3].position = sf::Vector2f{m_cellSize*(i+1) - cellBorder, m_cellSize* j    + cellBorder};
                m_cellsVA[_indexOfC(i, j)+4].position = sf::Vector2f{m_cellSize* i    + cellBorder, m_cellSize*(j+1) - cellBorder};
                m_cellsVA[_indexOfC(i, j)+5].position = sf::Vector2f{m_cellSize*(i+1) - cellBorder, m_cellSize*(j+1) - cellBorder};
            }
        }
    }

protected:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        for (int i=0; i<N; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                if (const auto[v,fixed] = m_sim.edgeX(i,j); fixed && v==0)
                {
                    m_wallsVA[_indexOfX(i, j)  ].position = sf::Vector2f{m_cellSize*i, m_cellSize*(j)};
                    m_wallsVA[_indexOfX(i, j)+1].position = sf::Vector2f{m_cellSize*i, m_cellSize*(j+1)};
                    m_wallsVA[_indexOfX(i, j)  ].color = sf::Color::Black;
                    m_wallsVA[_indexOfX(i, j)+1].color = sf::Color::Black;
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
        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M; ++j)
            {
                if (const auto[v,fixed] = m_sim.edgeY(i,j); fixed && v==0)
                {
                    m_wallsVA[_indexOfY(i, j)  ].position = sf::Vector2f{m_cellSize*(i), m_cellSize*j};
                    m_wallsVA[_indexOfY(i, j)+1].position = sf::Vector2f{m_cellSize*(i+1), m_cellSize*j};
                    m_wallsVA[_indexOfY(i, j)  ].color = sf::Color::Black;
                    m_wallsVA[_indexOfY(i, j)+1].color = sf::Color::Black;
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

        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                m_cellsVA[_indexOfC(i, j)  ].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[_indexOfC(i, j)].position), 0.5));
                m_cellsVA[_indexOfC(i, j)+1].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[_indexOfC(i, j)+1].position), 0.5));
                m_cellsVA[_indexOfC(i, j)+2].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[_indexOfC(i, j)+2].position), 0.5));
                m_cellsVA[_indexOfC(i, j)+3].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[_indexOfC(i, j)+3].position), 0.5));
                m_cellsVA[_indexOfC(i, j)+4].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[_indexOfC(i, j)+4].position), 0.5));
                m_cellsVA[_indexOfC(i, j)+5].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_sim.computeDensity(m_cellsVA[_indexOfC(i, j)+5].position), 0.5));
            }
        }

        states.transform.combine(*this);
        target.draw(m_cellsVA, states);
        target.draw(m_wallsVA, states);
    }
    int _indexOfX(int i, int j) const
    {
        return 2*((M-1)*i + j);
    }

    int _indexOfY(int i, int j) const
    {
        return 2*(M*N-N + M*i + j);
    }

    int _indexOfC(int i, int j) const
    {
        return 6*((M-1)*i + j);
    }

    const FluidSim<N,M>& m_sim;

    mutable sf::VertexArray m_wallsVA{sf::PrimitiveType::Lines, 2*(2*M*N-N-M)};
    mutable sf::VertexArray m_cellsVA{sf::PrimitiveType::Triangles, 6*M*N};
    const float m_cellSize = 10.f;
};



int main()
{
    const int size = 800;

    // srand(0);

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0;

    sf::RenderWindow window{sf::VideoMode{{size, size}}, "FluidSim", sf::Style::Default, sf::State::Windowed, settings};
    // window.setFramerateLimit(60);

    FluidSim<40, 40> fluidSim{};
    Display display{fluidSim};
    display.translate({5,5});
    display.scale({2, 2});

    act::ArrowShape cursorSpeed{4, sf::Color::Green};
    // std::vector<act::ArrowShape> arrows;
    // arrows.assign(40*40, {2, sf::Color::Red});

    sf::Clock clock;
    sf::Clock perf;

    while (window.isOpen())
    {
        window.handleEvents(
            [&](const sf::Event::Closed&){ window.close(); },
            [&](const sf::Event::KeyPressed& kp){ if (kp.code == sf::Keyboard::Key::Escape) window.close(); },
            [&](const sf::Event::MouseWheelScrolled& mws){
                if (mws.delta > 0)
                    display.scale({1.1,1.1});
                else
                    display.scale({1/1.1,1/1.1});
            },
            [&](const sf::Event::MouseMoved& mm){
                sf::Vector2f mouseInGrid = display.getInverse().transformPoint(sf::Vector2f{mm.position});
                cursorSpeed.setStartPosition(sf::Vector2f{mm.position});
                cursorSpeed.setEndPosition(sf::Vector2f{mm.position} + 100.f * fluidSim.computeVelocity(mouseInGrid));
            },
            [&](const sf::Event::MouseButtonPressed&){ fluidSim.density(1,17) += 1; fluidSim.density(1,21) += 1; fluidSim.density(25,19) += 1; });

        for (int i=0; i<100; ++i)
            fluidSim.update(sf::seconds(0.01));

        if (clock.getElapsedTime() > sf::seconds(1/30.))
        {
            clock.restart();

            // for (int i=0; i<40; ++i)
            // {
            //     for (int j=0; j<40; ++j)
            //     {
            //         sf::Vector2f startPosInGrid{10*i+5, 10*j+5};
            //         sf::Vector2f startPos = display.transformPoint(startPosInGrid);
            //         sf::Vector2f endPos = startPos + 5.f * grid.computeVelocity(startPosInGrid);
            //         arrows.at(40*i+j).setStartPosition(startPos);
            //         arrows.at(40*i+j).setEndPosition(endPos);
            //     }
            // }
            window.clear({45, 45, 45});
            window.draw(display);
            // for (const auto& a : arrows)
            //     window.draw(a);
            window.draw(cursorSpeed);
            window.display();

            fflush(stdout);
        }
    }

    return 0;
}


