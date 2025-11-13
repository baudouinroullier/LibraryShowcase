#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <Interact/ArrowShape.h>
#include "FluidSim.h"
#include "FluidDisplay.h"



int main()
{
    const int size = 1000;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0;

    FluidSim fluidSim{};
    FluidDisplay fluidDisplay{fluidSim};

    act::ArrowShape cursorSpeed{0.05f, sf::Color::Green};
    std::vector<act::ArrowShape> arrows;
    arrows.assign(FluidSim::N * FluidSim::M, {0.05f, sf::Color::Red});

    sf::Clock displayClock;
    sf::Clock updateClock;
    sf::Clock perf;
    int n = 0;

    sf::RenderWindow window{sf::VideoMode{{size, size}}, "FluidSim", sf::Style::Default, sf::State::Windowed, settings};
    sf::View view = window.getView();
    view.setCenter({});
    view.zoom(1./10);
    window.setView(view);
    // window.setFramerateLimit(60);

    while (window.isOpen())
    {
        window.handleEvents(
            [&](const sf::Event::Closed&){ window.close(); },
            // [&](const sf::Event::Resized& resize){
                // view.setSize(sf::Vector2f{resize.size});
                // window.setView(view);
            // },
            [&](const sf::Event::KeyPressed& kp){
                if (kp.code == sf::Keyboard::Key::Escape)
                    window.close();
                if (kp.code == sf::Keyboard::Key::Space)
                {
                    fluidSim.density[FluidSim::N/2-1, FluidSim::M/2-1] += 1;
                    fluidSim.density[FluidSim::N/2-1, FluidSim::M/2] += 1;
                    }
            },
            [&](const sf::Event::MouseWheelScrolled& mws){
                view.zoom(std::pow(1.1, -std::copysign(1, mws.delta)));
                window.setView(view);
            },
            [&](const sf::Event::MouseMoved& mm){
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mm.position);
                    sf::Vector2f mouseInGrid = mousePos + sf::Vector2f{FluidSim::N/2.f, FluidSim::M/2.f};
                    fluidSim.density.at(std::floor(mouseInGrid.x), std::floor(mouseInGrid.y)) += 2;
                }
            },
            [&](const sf::Event::MouseButtonPressed& mbp){
                sf::Vector2f mousePos = window.mapPixelToCoords(mbp.position);
                sf::Vector2f mouseInGrid = mousePos + sf::Vector2f{FluidSim::N/2.f, FluidSim::M/2.f};
                fluidSim.density.at(std::floor(mouseInGrid.x), std::floor(mouseInGrid.y)) += 5;
            });


        if (displayClock.getElapsedTime() > sf::seconds(1/30.))
        {
            displayClock.restart();

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2f mouseInGrid = mousePos + sf::Vector2f{FluidSim::N/2.f-.5, FluidSim::M/2.f-.5};
            cursorSpeed.setStartPosition(mousePos);
            cursorSpeed.setEndPosition(mousePos + 5.f * fluidSim.computeLerpCell(mouseInGrid).first);

            for (int i=1; i<FluidSim::N-1; ++i)
            {
                for (int j=1; j<FluidSim::M-1; ++j)
                {
                    sf::Vector2f startPos{i-FluidSim::N/2.f+.5f, j-FluidSim::M/2.f+.5f};
                    sf::Vector2f vel = fluidSim.velocity.at(i, j);
                    sf::Vector2f endPos = startPos + vel;
                    arrows.at(FluidSim::M*i+j).setStartPosition(startPos);
                    arrows.at(FluidSim::M*i+j).setEndPosition(endPos);
                }
            }
            window.clear({45, 45, 45});
            window.draw(fluidDisplay);
            // for (const auto& a : arrows)
                // window.draw(a);
            window.draw(cursorSpeed);
            window.display();

        }

        {
            perf.start();
            fluidSim.update(200.f*updateClock.restart());
            n++;
            perf.stop();
            if (perf.getElapsedTime() > sf::seconds(1))
            {
                perf.reset();
                fmt::println("ups {}", n);
                n = 0;
            }
        }

        fflush(stdout);

    }

    return 0;
}


