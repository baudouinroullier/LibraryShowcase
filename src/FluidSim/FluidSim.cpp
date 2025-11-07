#include "FluidSim.h"

sf::Color operator*(sf::Color lhs, float s)
{
    return {static_cast<uint8_t>(std::clamp<float>(lhs.r * s, 0, 255)),
            static_cast<uint8_t>(std::clamp<float>(lhs.g * s, 0, 255)),
            static_cast<uint8_t>(std::clamp<float>(lhs.b * s, 0, 255))};
}
