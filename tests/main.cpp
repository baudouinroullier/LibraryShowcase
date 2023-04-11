#include "../src/Matrix.h"
#include <fmt/core.h>

int main()
{
    bla::Vector2 test{1,1};
    fmt::print("Hello! {} {}\n", test(0), test(1));
    return 0;
}
