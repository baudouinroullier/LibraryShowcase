#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <random>



struct Point
{
    double x, y, z;
};

constexpr float TOP = 4;
constexpr float BOTTOM = -4;
constexpr float RIGHT = 4;
constexpr float LEFT = -4;

struct Projector
{
    int size;
    sf::Vector2f operator()(Point p) const
    {
        constexpr double isqrt12 = 1/std::sqrt(12);
        Point proj{3*isqrt12*p.x + -0.5*p.y, -isqrt12*p.x - 0.5*p.y - std::sqrt(2./3) * p.z};
        return {proj.x*size/(RIGHT-LEFT) + size/2,
                proj.y*size/(TOP-BOTTOM) + size/2};
    }
};

struct Object
{
    Point origin;

};

int main()
{
    const int size = 800;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window{sf::VideoMode{{size, size}}, "OrthoProj", sf::Style::Default, sf::State::Windowed, settings};
    window.setFramerateLimit(60);

    std::array<Point, 8> vertices {{{-1, -1, -1},
                                   { 1, -1, -1},
                                   { 1,  1, -1},
                                   {-1,  1, -1},
                                   {-1, -1,  1},
                                   { 1, -1,  1},
                                   { 1,  1,  1},
                                   {-1,  1,  1}}};
    std::array<sf::Vector2f, 8> projected;
    std::transform(vertices.begin(), vertices.end(), projected.begin(), Projector{size});

    std::array<std::pair<size_t, size_t>, 12> mesh{{{0, 1}, {1, 2}, {2, 3}, {3, 0},
                                                    {4, 5}, {5, 6}, {6, 7}, {7, 4},
                                                    {0, 4}, {1, 5}, {2, 6}, {3, 7}}};


    sf::VertexArray va{sf::PrimitiveType::Lines};
    for (auto e : mesh)
    {
        va.append({projected[e.first], sf::Color::White});
        va.append({projected[e.second], sf::Color::White});
    }

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(va);

        window.display();
    }

    return 0;
}


