#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <Interact/ArrowShape.h>
#include "FluidSim.h"
#include "FluidDisplay.h"



int main()
{
    const int size = 800;

    // srand(0);

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0;

    sf::RenderWindow window{sf::VideoMode{{size, size}}, "FluidSim", sf::Style::Default, sf::State::Windowed, settings};
    // window.setFramerateLimit(60);

    FluidSim fluidSim{};
    FluidDisplay fluidDisplay{fluidSim};
    fluidDisplay.translate({5,5});
    fluidDisplay.scale({2, 2});

    act::ArrowShape cursorSpeed{4, sf::Color::Green};
    std::vector<act::ArrowShape> arrows;
    arrows.assign(40*40, {1, sf::Color::Red});

    sf::Clock clock;

    while (window.isOpen())
    {
        window.handleEvents(
            [&](const sf::Event::Closed&){ window.close(); },
            [&](const sf::Event::KeyPressed& kp){ if (kp.code == sf::Keyboard::Key::Escape) window.close(); },
            [&](const sf::Event::MouseWheelScrolled& mws){
                if (mws.delta > 0)
                    fluidDisplay.scale({1.1,1.1});
                else
                    fluidDisplay.scale({1/1.1,1/1.1});
            },
            [&](const sf::Event::MouseMoved& mm){
                sf::Vector2f mouseInGrid = fluidDisplay.getInverse().transformPoint(sf::Vector2f{mm.position});
                cursorSpeed.setStartPosition(sf::Vector2f{mm.position});
                cursorSpeed.setEndPosition(sf::Vector2f{mm.position} + 100.f * fluidSim.computeVelocity(mouseInGrid));
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    fluidSim.density(std::floor(mouseInGrid.x)/10., std::floor(mouseInGrid.y)/10.) += 0.2;
            },
            [&](const sf::Event::MouseButtonPressed& mbp){
                sf::Vector2f mouseInGrid = fluidDisplay.getInverse().transformPoint(sf::Vector2f{mbp.position});
                fluidSim.density(std::floor(mouseInGrid.x)/10., std::floor(mouseInGrid.y)/10.) += 2;
            });

        if (clock.getElapsedTime() > sf::seconds(1/30.))
        {
            clock.restart();

            for (int i=0; i<40; ++i)
            {
                for (int j=0; j<40; ++j)
                {
                    sf::Vector2f startPosInGrid{10*i+5, 10*j+5};
                    sf::Vector2f startPos = fluidDisplay.transformPoint(startPosInGrid);
                    sf::Vector2f endPos = startPos + 5.f * fluidSim.computeVelocity(startPosInGrid);
                    arrows.at(40*i+j).setStartPosition(startPos);
                    arrows.at(40*i+j).setEndPosition(endPos);
                }
            }
            window.clear({45, 45, 45});
            window.draw(fluidDisplay);
            for (const auto& a : arrows)
                window.draw(a);
            window.draw(cursorSpeed);
            window.display();

            fflush(stdout);
        }

        fluidSim.update(sf::seconds(0.05));
    }

    return 0;
}


