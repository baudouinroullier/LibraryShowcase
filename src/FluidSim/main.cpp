#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <fenv.h>
#include <Interact/ArrowShape.h>


sf::Color operator*(sf::Color lhs, double s)
{
    return {lhs.r*s, lhs.g*s, lhs.b*s};
}

template<class T>
T lerp(const T a, const T b, double t)
{
    return a + (b-a)*t;
}

struct Edge
{
    double velocity = 0;
    bool isFixed = false;
};

template <int N, int M>
class Grid
{
public:
    Grid()
    {
        for (int i=0; i<N; ++i)
            for (int j=0; j<M-1; ++j)
                vx(i,j) = (rand() % 2000) / 1000. - 1;

        m_edgeX.front().fill({0, true});
        m_edgeX.back().fill({0, true});

        for (int i=0; i<N-1; ++i)
            for (int j=0; j<M; ++j)
                vy(i,j) = (rand() % 2000) / 1000. - 1;

        for (int i=0; i<N-1; ++i)
            m_edgeY.at(i).front() = m_edgeY.at(i).back() = {0, true};

        // vx.at(2).at(2).velocity = 1;
        // vx.at(5).at(5) = 1;
        // vx.at(10).at(5) = -1;
        // vy.at(5).at(10) = 1;
        // vy.at(10).at(10) = -1;
    }

    void update(sf::Time dt)
    {
        _advect(dt);
        _forceNullDivergence();
    }

    sf::Vector2f computeVelocity(sf::Vector2f pos) const
    {
        double x = pos.x/m_cellSize;
        double y = pos.y/m_cellSize;

        if (x < 0 || x > N-1 || y < 0 || y > M-1)
            return {};

        double ix = std::floor(x);
        double ix1 = std::min(ix+1, N-1.);
        double jx = std::max(std::floor(y-0.5), 0.);
        double jx1 = std::min(std::floor(y+0.5), M-2.);
        double iy = std::max(std::floor(x-0.5), 0.);
        double iy1 =  std::min(std::floor(x+0.5), N-2.);
        double jy = std::floor(y);
        double jy1 = std::min(jy+1, M-1.);

        // fmt::println("{} {}, {} {} {} {} , {} {} {} {}", x, y, ix, ix1, jx, jx1, iy, iy1, jy, jy1); fflush(stdout);

        double tix = x - ix;
        double tjx = y - jx - .5;
        double tiy = x - iy - .5;
        double tjy = y - jy;

        double vx1 = lerp(vx(ix,jx), vx(ix1,jx), tix);
        double vx2 = lerp(vx(ix,jx1), vx(ix1,jx1), tix);
        double vxx = lerp(vx1, vx2, tjx);

        double vy1 = lerp(vy(iy,jy), vy(iy1,jy), tiy);
        double vy2 = lerp(vy(iy,jy1), vy(iy1,jy1), tiy);
        double vyy = lerp(vy1, vy2, tjy);
        return {vxx, vyy};
    }

    Edge edgeX(int i, int j) const
    {
        return m_edgeX.at(i).at(j);
    }
    Edge edgeY(int i, int j) const
    {
        return m_edgeY.at(i).at(j);
    }
    double vx(int i, int j) const
    {
        return m_edgeX.at(i).at(j).velocity;
    }
    double vy(int i, int j) const
    {
        return m_edgeY.at(i).at(j).velocity;
    }
    double div(int i, int j) const
    {
        return m_div.at(i).at(j);
    }
    double& vx(int i, int j)
    {
        return m_edgeX.at(i).at(j).velocity;
    }
    double& vy(int i, int j)
    {
        return m_edgeY.at(i).at(j).velocity;
    }
    double& div(int i, int j)
    {
        return m_div.at(i).at(j);
    }

protected:

    void _forceNullDivergence()
    {
        while (_updateDivergence() > 1)
            _spreadDivergence();
        _updateDivergence();
    }

    void _advect(sf::Time dt)
    {

    }

    double _updateDivergence()
    {
        double totalDiv = 0;
        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                div(i,j) = (vx(i+1,j) - vx(i,j)+ vy(i,j+1)- vy(i,j))/m_cellSize;
                totalDiv += std::abs(m_div.at(i).at(j));
            }
        }
        return totalDiv;
    }

    void _spreadDivergence()
    {
        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                double s = !m_edgeX.at(i+1).at(j).isFixed + !m_edgeX.at(i).at(j).isFixed + !m_edgeY.at(i).at(j+1).isFixed + !m_edgeY.at(i).at(j).isFixed;
                if (s == 0)
                    continue;

                double d = 4*m_div.at(i).at(j)/4;
                m_edgeX.at(i+1).at(j).velocity -= d * !m_edgeX.at(i+1).at(j).isFixed / s;
                m_edgeX[i][j].velocity += d * !m_edgeX[i][j].isFixed / s;
                m_edgeY.at(i).at(j+1).velocity -= d * !m_edgeY.at(i).at(j+1).isFixed / s;
                m_edgeY[i][j].velocity += d * !m_edgeY[i][j].isFixed / s;
            }
        }
    }

    std::array<std::array<Edge, M-1>, N> m_edgeX;
    std::array<std::array<Edge, M>, N-1> m_edgeY;
    std::array<std::array<double, M-1>, N-1> m_div;

    const float m_cellSize = 10.f;
};



template <int N, int M>
class Display : public sf::Drawable, public sf::Transform
{
public:
    Display(const Grid<N,M>& grid) : m_grid(grid)
    {
        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                m_cellsVA[_indexOfC(i, j)  ].position = sf::Vector2f{m_cellSize* i    + .5f, m_cellSize* j    + .5f};
                m_cellsVA[_indexOfC(i, j)+1].position = sf::Vector2f{m_cellSize*(i+1) - .5f, m_cellSize* j    + .5f};
                m_cellsVA[_indexOfC(i, j)+2].position = sf::Vector2f{m_cellSize* i    + .5f, m_cellSize*(j+1) - .5f};
                m_cellsVA[_indexOfC(i, j)+3].position = sf::Vector2f{m_cellSize*(i+1) - .5f, m_cellSize* j    + .5f};
                m_cellsVA[_indexOfC(i, j)+4].position = sf::Vector2f{m_cellSize* i    + .5f, m_cellSize*(j+1) - .5f};
                m_cellsVA[_indexOfC(i, j)+5].position = sf::Vector2f{m_cellSize*(i+1) - .5f, m_cellSize*(j+1) - .5f};
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
                if (const auto[v,fixed] = m_grid.edgeX(i,j); fixed && v==0)
                {
                    m_wallsVA[_indexOfX(i, j)  ].position = sf::Vector2f{m_cellSize*i, m_cellSize*(j)};
                    m_wallsVA[_indexOfX(i, j)+1].position = sf::Vector2f{m_cellSize*i, m_cellSize*(j+1)};
                    m_wallsVA[_indexOfX(i, j)  ].color = sf::Color::Black;
                    m_wallsVA[_indexOfX(i, j)+1].color = sf::Color::Black;
                }
                else
                {
                    m_wallsVA[_indexOfX(i, j)  ].position = sf::Vector2f{m_cellSize*i, m_cellSize*(j+.5f)};
                    m_wallsVA[_indexOfX(i, j)+1].position = sf::Vector2f{m_cellSize*(i+m_grid.vx(i,j)/2), m_cellSize*(j+.5f)};
                    m_wallsVA[_indexOfX(i, j)  ].color = sf::Color::White;
                    m_wallsVA[_indexOfX(i, j)+1].color = sf::Color::White;
                }
            }
        }
        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M; ++j)
            {
                if (const auto[v,fixed] = m_grid.edgeY(i,j); fixed && v==0)
                {
                    m_wallsVA[_indexOfY(i, j)  ].position = sf::Vector2f{m_cellSize*(i), m_cellSize*j};
                    m_wallsVA[_indexOfY(i, j)+1].position = sf::Vector2f{m_cellSize*(i+1), m_cellSize*j};
                    m_wallsVA[_indexOfY(i, j)  ].color = sf::Color::Black;
                    m_wallsVA[_indexOfY(i, j)+1].color = sf::Color::Black;
                }
                else
                {
                    m_wallsVA[_indexOfY(i, j)  ].position = sf::Vector2f{m_cellSize*(i+.5f), m_cellSize*j};
                    m_wallsVA[_indexOfY(i, j)+1].position = sf::Vector2f{m_cellSize*(i+.5f), m_cellSize*(j+m_grid.vy(i,j)/2)};
                    m_wallsVA[_indexOfY(i, j)  ].color = sf::Color::White;
                    m_wallsVA[_indexOfY(i, j)+1].color = sf::Color::White;
                }
            }
        }

        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                double d = m_grid.div(i,j);
                m_cellsVA[_indexOfC(i, j)  ].color = \
                m_cellsVA[_indexOfC(i, j)+1].color = \
                m_cellsVA[_indexOfC(i, j)+2].color = \
                m_cellsVA[_indexOfC(i, j)+3].color = \
                m_cellsVA[_indexOfC(i, j)+4].color = \
                m_cellsVA[_indexOfC(i, j)+5].color = lerp({64, 64, 64}, d > 0 ? sf::Color::Blue : sf::Color::Red, abs(4*d));
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

    const Grid<N,M>& m_grid;

    mutable sf::VertexArray m_wallsVA{sf::PrimitiveType::Lines, 2*(2*M*N-N-M)};
    mutable sf::VertexArray m_cellsVA{sf::PrimitiveType::Triangles, 6*M*N};
    const float m_cellSize = 10.f;
};



int main()
{
    // feenableexcept(FE_ALL_EXCEPT);

    const int size = 800;

    srand(0);

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0;

    sf::RenderWindow window{sf::VideoMode{{size, size}}, "FluidSim", sf::Style::Default, sf::State::Windowed, settings};
    window.setFramerateLimit(60);

    Grid<5, 5> grid{};
    grid.update({});
    Display display{grid};
    display.translate({5,5});
    display.scale({20,20});

    act::ArrowShape cursorSpeed{4, sf::Color::Green};
    std::vector<act::ArrowShape> arrows;
    arrows.assign(17*17, {2, sf::Color::Red});
    for (int i=0; i<17; ++i)
    {
        for (int j=0; j<17; ++j)
        {
            arrows.at(17*i+j).setStartPosition({5+20*10*i/4, 5+20*10*j/4});
            arrows.at(17*i+j).setEndPosition({5+20*10*i/4+10, 5+20*10*j/4+10});
        }
    }

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
            if (const sf::Event::MouseWheelScrolled* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>();
                mouseWheel)
            {
                if (mouseWheel->delta > 0)
                    display.scale({1.1,1.1});
                else
                    display.scale({1/1.1,1/1.1});
            }
            if (const sf::Event::MouseMoved* mouseMove = event->getIf<sf::Event::MouseMoved>();
                mouseMove)
            {
                sf::Vector2f mouseInGrid = display.getInverse().transformPoint(sf::Vector2f{mouseMove->position});
                cursorSpeed.setStartPosition(sf::Vector2f{mouseMove->position});
                cursorSpeed.setEndPosition(sf::Vector2f{mouseMove->position} + 100.f * grid.computeVelocity(mouseInGrid));
            }
        }

        window.clear({45, 45, 45});
        window.draw(display);
        for (const auto& a : arrows)
            window.draw(a);
        window.draw(cursorSpeed);
        window.display();
    }

    return 0;
}


