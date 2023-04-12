#include "Maths.h"

namespace maths
{

constexpr double degrees(double rad)
{
    return 180 * rad / M_PI;
}

constexpr double radians(double deg)
{
    return M_PI * deg / 180;
}

}
