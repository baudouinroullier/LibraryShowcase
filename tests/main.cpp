#include <bla/Vector.hpp>
#include <fmt/core.h>

int main()
{
    bla::Matrix<3,2> m({1, -2, M_PI, -4, 5, -6});
    bla::Vec2 v{{1., -1.}};
    bla::Vec3 result {m*v};
    fmt::print("{: .3f} × \n{: .3f} = \n{: .3f}\n", m, v, result);
    return 0;
}
