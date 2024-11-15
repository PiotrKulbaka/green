#pragma once

#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>

namespace green::core
{

    constexpr const auto pi = 3.14159265358979323846;

    template <class T>
    T min(T& x, T& y, T& z)
    {
        return (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
    }

    template <class T>
    T min(T& x, T& y)
    {
        return x < y ? x : y;
    }

    template <class T>
    T max(T& x, T& y, T& z)
    {
        return (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    }

    template <class T>
    T max(T& x, T& y)
    {
        return x > y ? x : y;
    }

    template <class T>
    T sign(T a)
    {
        return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
    }

    template <class T>
    T abs(T v)
    {
        return std::abs(v);
    }

    template <class T>
    T sqrt(T v)
    {
        return std::sqrt(v);
    }

    template <class T>
    T sin(T v)
    {
        return std::sin(v);
    }

    template <class T>
    T cos(T v)
    {
        return std::cos(v);
    }

    template <class T>
    T tan(T v)
    {
        return std::tan(v);
    }

    template <class T>
    T asin(T v)
    {
        return std::asin(v);
    }

    template <class T>
    T acos(T v)
    {
        return std::acos(v);
    }

    template <class T>
    T atan(T v)
    {
        return std::atan(v);
    }

    template <class T>
    T atan2(T x, T y)
    {
        return std::atan2(x, y);
    }

    template <class T>
    T log(T v)
    {
        return std::log(v);
    }

    template <class T>
    T exp(T v)
    {
        return std::exp(v);
    }

    template <class T>
    T pow(T base, T power)
    {
        return std::pow(base, power);
    }

    template <class T>
    T deg2rad(T v)
    {
        return v * (pi / 180.0);
    }

    template <class T>
    T rad2deg(T v)
    {
        return v * (180.0 / pi);
    }

    template <class T>
    T floor(T v)
    {
        return std::floor(v);
    }

    template <class T>
    T ceil(T v)
    {
        return std::ceil(v);
    }

    template <class T>
    T frac(T v)
    {
        return v - std::floor(v);
    }

    template <class T>
    T round(T v)
    {
        return std::floor(v + 0.5);
    }

    template <class T>
    T clamp(T v, T min, T max)
    {
        return v < min ? min : (v > max ? max : v);
    }

} // namespace engine::core
