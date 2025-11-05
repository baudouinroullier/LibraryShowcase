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

struct Cell
{
    double divergence = 0;
    double density = 0;
};

template <int N, int M>
class Grid
{
public:
    Grid()
    {
        m_edgesX.front().fill({3, true});
        m_edgesX.back() .fill({3, true});


        for (int j=M/4; j<M-1-M/4; ++j)
            m_edgesX.at(N/2).at(j) = {0, true};

        for (int i=0; i<N-1; ++i)
            m_edgesY.at(i).front() = m_edgesY.at(i).back() = {0, true};

        _computeDivergence();
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

    double computeDensity(sf::Vector2f pos) const
    {
        double x = pos.x/m_cellSize;
        double y = pos.y/m_cellSize;

        if (x < 0 || x > N-1 || y < 0 || y > M-1)
            return {};

        double i = std::max(std::floor(x-0.5), 0.);
        double i1 =  std::min(std::floor(x+0.5), N-2.);
        double j = std::max(std::floor(y-0.5), 0.);
        double j1 = std::min(std::floor(y+0.5), M-2.);

        // fmt::println("{} {}, {} {} {} {} , {} {} {} {}", x, y, ix, ix1, jx, jx1, iy, iy1, jy, jy1); fflush(stdout);

        double tj = y - j - .5;
        double ti = x - i - .5;

        double d1 = lerp(density(i,j), density(i1,j), ti);
        double d2 = lerp(density(i,j1), density(i1,j1), ti);
        double d = lerp(d1, d2, tj);
        return d;
    }

    Edge edgeX(int i, int j) const
    {
        return m_edgesX.at(i).at(j);
    }
    Edge edgeY(int i, int j) const
    {
        return m_edgesY.at(i).at(j);
    }
    double vx(int i, int j) const
    {
        return m_edgesX.at(i).at(j).velocity;
    }
    double vy(int i, int j) const
    {
        return m_edgesY.at(i).at(j).velocity;
    }
    double div(int i, int j) const
    {
        return m_cells.at(i).at(j).divergence;
    }
    double density(int i, int j) const
    {
        return m_cells.at(i).at(j).density;
    }
    double& vx(int i, int j)
    {
        return m_edgesX.at(i).at(j).velocity;
    }
    double& vy(int i, int j)
    {
        return m_edgesY.at(i).at(j).velocity;
    }
    double& div(int i, int j)
    {
        return m_cells.at(i).at(j).divergence;
    }
    double& density(int i, int j)
    {
        return m_cells.at(i).at(j).density;
    }

protected:

    void _forceNullDivergence()
    {
        while (_computeDivergence() > N*M/10000.)
            _spreadDivergence();
        _computeDivergence();
    }

    void _advect(sf::Time dt)
    {
        auto edgesXTmp = m_edgesX;
        auto edgesYTmp = m_edgesY;
        auto cellsTmp = m_cells;

        for (int i=0; i<N; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                if (m_edgesX.at(i).at(j).isFixed)
                    continue;
                sf::Vector2f pos{m_cellSize*i, m_cellSize*(j+.5f)};
                sf::Vector2f velocity = computeVelocity(pos);
                sf::Vector2f posPrev = pos - velocity * dt.asSeconds();
                edgesXTmp.at(i).at(j).velocity = computeVelocity(posPrev).x;
            }
        }

        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M; ++j)
            {
                if (m_edgesY.at(i).at(j).isFixed)
                    continue;
                sf::Vector2f pos{m_cellSize*(i+.5f), m_cellSize*j};
                sf::Vector2f velocity = computeVelocity(pos);
                sf::Vector2f posPrev = pos - velocity * dt.asSeconds();
                edgesYTmp.at(i).at(j).velocity = computeVelocity(posPrev).y;
            }
        }

        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                sf::Vector2f pos{m_cellSize*(i+.5f), m_cellSize*(j+.5f)};
                sf::Vector2f velocity = computeVelocity(pos);
                sf::Vector2f posPrev = pos - velocity * dt.asSeconds();
                density(i,j) = computeDensity(posPrev);
                cellsTmp.at(i).at(j).density = computeDensity(posPrev);
            }
        }


        m_edgesX = edgesXTmp;
        m_edgesY = edgesYTmp;
        m_cells = cellsTmp;
    }

    double _computeDivergence()
    {
        double totalDiv = 0;
        for (int i=0; i<N-1; ++i)
        {
            for (int j=0; j<M-1; ++j)
            {
                div(i,j) = (vx(i+1,j) - vx(i,j)+ vy(i,j+1)- vy(i,j))/2;
                totalDiv += std::abs(div(i,j));
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
                double s = !m_edgesX.at(i+1).at(j).isFixed + !m_edgesX.at(i).at(j).isFixed + !m_edgesY.at(i).at(j+1).isFixed + !m_edgesY.at(i).at(j).isFixed;
                if (s == 0)
                    continue;

                double d = div(i,j);
                vx(i+1,j) -= d * !m_edgesX.at(i+1).at(j).isFixed / s;
                vx(i,j)   += d * !m_edgesX[i][j].isFixed / s;
                vy(i,j+1) -= d * !m_edgesY.at(i).at(j+1).isFixed / s;
                vy(i,j)   += d * !m_edgesY[i][j].isFixed / s;
            }
        }
    }

    std::array<std::array<Edge, M-1>, N> m_edgesX;
    std::array<std::array<Edge, M>, N-1> m_edgesY;
    std::array<std::array<Cell, M-1>, N-1> m_cells;

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
                m_cellsVA[_indexOfC(i, j)  ].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_grid.computeDensity(m_cellsVA[_indexOfC(i, j)].position),2.));
                m_cellsVA[_indexOfC(i, j)+1].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_grid.computeDensity(m_cellsVA[_indexOfC(i, j)+1].position),2.));
                m_cellsVA[_indexOfC(i, j)+2].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_grid.computeDensity(m_cellsVA[_indexOfC(i, j)+2].position),2.));
                m_cellsVA[_indexOfC(i, j)+3].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_grid.computeDensity(m_cellsVA[_indexOfC(i, j)+3].position),2.));
                m_cellsVA[_indexOfC(i, j)+4].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_grid.computeDensity(m_cellsVA[_indexOfC(i, j)+4].position),2.));
                m_cellsVA[_indexOfC(i, j)+5].color = lerp({64, 64, 64}, sf::Color::White, std::pow(m_grid.computeDensity(m_cellsVA[_indexOfC(i, j)+5].position),2.));
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
    // window.setFramerateLimit(60);

    Grid<40, 40> grid{};
    Display display{grid};
    display.translate({5,5});
    display.scale({2, 2});

    act::ArrowShape cursorSpeed{4, sf::Color::Green};
    // std::vector<act::ArrowShape> arrows;
    // arrows.assign(41*41, {2, sf::Color::Red});

    sf::Clock clock;
    double time = 0;
    int n = 0;

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const sf::Event::KeyPressed* keyPress = event->getIf<sf::Event::KeyPressed>();
                keyPress && keyPress->code == sf::Keyboard::Key::Escape)
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
            if (const sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>();
                mousePress)
            {
                grid.density(0,19) = 2;
            }
        }

        auto dt = clock.restart();
        for (int i=0; i<4; ++i)
            grid.update(dt);
        time += dt.asSeconds();
        n++;
        if (n == 100)
        {
            fmt::println("fps = {}", n/time); fflush(stdout);
            n = 0;
            time = 0;
        }

        // for (int i=0; i<41; ++i)
        // {
        //     for (int j=0; j<41; ++j)
        //     {
        //         sf::Vector2f startPosInGrid{10*i/2., 10*j/2.};
        //         sf::Vector2f startPos = display.transformPoint(startPosInGrid);
        //         sf::Vector2f endPos = startPos + 10.f * grid.computeVelocity(startPosInGrid);
        //         arrows.at(41*i+j).setStartPosition(startPos);
        //         arrows.at(41*i+j).setEndPosition(endPos);
        //     }
        // }


        window.clear({45, 45, 45});
        window.draw(display);
        // for (const auto& a : arrows)
        //     window.draw(a);
        window.draw(cursorSpeed);
        window.display();
    }

    return 0;
}


