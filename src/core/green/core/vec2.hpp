#pragma once

#include <green/core/math_base.hpp>
#include <glm/vec2.hpp>

namespace green::core
{

    class vec2
    {
    public:
                    vec2() noexcept {}
                    vec2(float v) noexcept : x(v), y(v) {}
                    vec2(float x, float y) noexcept : x(x), y(y) {}

        float       operator[](size_t index) const noexcept     { return (&x)[index]; }
        float&      operator[](size_t index)  noexcept          { return (&x)[index]; }

        vec2&       operator+=(const vec2& a) noexcept          { vec += a.vec; return *this; }
        vec2&       operator-=(const vec2& a) noexcept          { vec -= a.vec; return *this; }
        vec2&       operator*=(float a) noexcept                { vec *= a; return *this; }
        vec2&       operator/=(const vec2& a) noexcept          { vec /= a.vec; return *this; }
        vec2&       operator/=(float a) noexcept                { vec /= a; return *this; }

        vec2        operator-() const noexcept                  { vec2 r; r.vec = -vec; return r; }
        vec2        operator+(const vec2& a) const noexcept     { vec2 r; r.vec = vec + a.vec; return r; }
        vec2        operator-(const vec2& a) const noexcept     { vec2 r; r.vec = vec - a.vec; return r; }
        vec2        operator*(const vec2& a) const noexcept     { vec2 r; r.vec = vec * a.vec; return r; }
        vec2        operator*(float a) const noexcept           { vec2 r; r.vec = vec * a; return r; }
        vec2        operator/(float a) const noexcept           { vec2 r; r.vec = vec / a; return r; }

        float       length() const noexcept                     { return glm::length(vec); }
        float       dot(const vec2& a) const noexcept           { return glm::dot(vec, a.vec); }

        vec2        normalize() const noexcept                  { vec2 r; r.vec = glm::normalize(vec); return r; }
        void        normalize_self() noexcept                   { vec = glm::normalize(vec); }

    public:
        union
        {
            struct
            {
                float   x, y;
            };
            struct
            {
                float   left, top;
            };
            struct
            {
                float   width, height;
            };
            glm::vec2   vec;
        };
    }; // class vec2

    static_assert(sizeof(vec2) == 8, "incorreect size");

} // namespace green::core
