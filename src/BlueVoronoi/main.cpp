#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <random>


#define JC_VORONOI_IMPLEMENTATION
#define JCV_REAL_TYPE double
#define JCV_ATAN2 atan2
#define JCV_SQRT sqrt
#define JCV_FLT_MAX DBL_MAX
#define JCV_PI 3.141592653589793115997963468544185161590576171875
#include "jc_voronoi.h"

int main()
{
    constexpr int size = 800;
    constexpr int poissonDistance = 30;
    int numTries = 10;
    constexpr int gridWidth = std::floor(poissonDistance/std::sqrt(2));
    constexpr int gridSize = std::ceil(size/gridWidth);

    std::array<std::array<double, gridSize>, gridSize> grid;

    for (auto& r : grid)
        r.fill(-1);

    std::mt19937 gen;
    gen.seed(time(nullptr));
    std::uniform_real_distribution<double> radiusDist{poissonDistance, 2 * poissonDistance};
    std::uniform_real_distribution<double> angleDist{0, 2 * M_PI};

    struct Sample
    {
        double x, y;
    };

    std::vector<Sample> samples;
    std::vector<int> activeList;

    Sample firstSample{size/2, size/2};
    samples.push_back(firstSample);
    activeList.push_back(0);
    grid[std::floor(firstSample.x / gridWidth)][std::floor(firstSample.y / gridWidth)] = 0;

    while (!activeList.empty())
    {
        int curSize = activeList.size();
        int index = activeList.back();
        Sample s = samples[index];
        bool inserted = false;

        for (int tries = 0; tries < numTries; ++tries)
        {
            double radius = radiusDist(gen);
            double angle = angleDist(gen);
            Sample candidate{s.x + radius * std::cos(angle), s.y + radius * std::sin(angle)};
            int gridX = std::floor(candidate.x / gridWidth);
            int gridY = std::floor(candidate.y / gridWidth);

            bool testInGrid = (gridX >= 0 && gridX < gridSize && gridY >= 0 && gridY < gridSize);

            auto checkCollisionInGrid = [&grid, &samples, poissonDistance, &candidate](int x, int y)
            {
                if (x > -1 && x < gridSize && y > -1 && y < gridSize)
                {
                    int neighborIndex = grid[x][y];
                    if (neighborIndex != -1)
                    {
                        Sample samp = samples[neighborIndex];
                        double dist2 = (samp.x - candidate.x) * (samp.x - candidate.x) + (samp.y - candidate.y) * (samp.y - candidate.y);
                        return dist2 > poissonDistance * poissonDistance;
                    }
                    else return true;
                }
                else return true;
            };

            bool canInsert = testInGrid;
            for (int i = -2; i < 3; ++i)
            {
                for (int j = -2; j < 3; ++j)
                {
                    if (std::abs(i) == 2 && std::abs(j) == 2) continue;
                    canInsert &= checkCollisionInGrid(gridX + i, gridY + j);
                    if (!canInsert) break;
                }
                if (!canInsert) break;
            }
            if (canInsert)
            {
                int newIdx = samples.size();
                grid[gridX][gridY] = newIdx;
                samples.push_back(candidate);
                activeList.push_back(newIdx);
                inserted = true;
                break;
            }
        }

        if (!inserted)
            activeList.erase(activeList.begin() + curSize - 1);
    }

    fmt::println("numTries, samples size : {}, {}", numTries, samples.size());
    fflush(stdout);

    jcv_diagram diagram{};
    jcv_rect box{{0, 0}, {size, size}};
    jcv_diagram_generate(samples.size(), (jcv_point*)samples.data(), &box, nullptr, &diagram);

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window{sf::VideoMode{{size, size}}, "BlueVoronoi", sf::Style::Default, sf::State::Windowed, settings};
    window.setFramerateLimit(60);


    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        window.clear(sf::Color::White);

        {
            sf::VertexArray va{sf::PrimitiveType::Triangles};

            std::uniform_int_distribution<int> colorDist{0, 255};
            gen.seed(0);
            const jcv_site* sites = jcv_diagram_get_sites(&diagram);
            for (int i = 0; i < diagram.numsites; ++i)
            {
                sf::Color curColor{colorDist(gen), colorDist(gen), colorDist(gen)};
                const jcv_site* site = &sites[i];
                const jcv_graphedge* e = site->edges;
                while (e)
                {
                    va.append({{site->p.x, site->p.y}, curColor});
                    va.append({{e->pos[0].x, e->pos[0].y}, curColor});
                    va.append({{e->pos[1].x, e->pos[1].y}, curColor});
                    e = e->next;
                }
            }
            window.draw(va);
        }

        {
            float r = 4;
            sf::CircleShape c{r};
            c.setFillColor(sf::Color::White);
            c.setOrigin({r, r});
            c.setOutlineColor(sf::Color::Black);
            c.setOutlineThickness(2);
            for (const Sample sample : samples)
            {
                c.setPosition({sample.x, sample.y});
                window.draw(c);
            }
        }

        window.display();
    }

    jcv_diagram_free(&diagram);
    return 0;
}


