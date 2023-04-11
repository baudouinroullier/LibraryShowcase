#include "../src/Vector.h"
#include <fmt/core.h>

int main()
{
    bla::Vec2 test{{1.,1.}};
    fmt::print("Hello! {} {}\n", test.x, test.y);
    return 0;
}
