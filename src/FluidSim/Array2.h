#ifndef ARRAY2_H
#define ARRAY2_H

#include <array>


template <class T, std::size_t _N, std::size_t _M>
class Array2 : public std::array<T, _N*_M>
{
    using parent = std::array<T, _N*_M>;
public:
    [[nodiscard]]
    constexpr T&
    operator[](std::size_t i, std::size_t j) noexcept
    {
        return parent::operator[](indexOf(i, j));
    }

    [[nodiscard]]
    constexpr const T&
    operator[](std::size_t i, std::size_t j) const noexcept
    {
        return parent::operator[](indexOf(i, j));
    }

    constexpr T&
    at(std::size_t i, std::size_t j)
    {
        return parent::at(indexOf(i, j));
    }

    constexpr const T&
    at(std::size_t i, std::size_t j) const
    {
        return parent::at(indexOf(i, j));
    }

    constexpr static std::size_t indexOf(std::size_t i, std::size_t j)
    {
        return _M*i + j;
    }
};



#endif // ARRAY2_H
