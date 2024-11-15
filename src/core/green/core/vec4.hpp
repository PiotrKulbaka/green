#pragma once

#include <green/core/math_base.hpp>
#include <glm/vec4.hpp>

namespace green::core
{

    class vec4
    {
    public:
                    vec4() noexcept {}
                    vec4(float v) noexcept : x(v), y(v), z(v), w(v) {}
                    vec4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}

        float       operator[](size_t index) const noexcept     { return (&x)[index]; }
        float&      operator[](size_t index) noexcept           { return (&x)[index]; }

        vec4&       operator+=(const vec4& a) noexcept          { vec += a.vec; return *this; }
        vec4&       operator-=(const vec4& a) noexcept          { vec -= a.vec; return *this; }
        vec4&       operator*=(float a) noexcept                { vec *= a; return *this; }
        vec4&       operator/=(const vec4& a) noexcept          { vec /= a.vec; return *this; }
        vec4&       operator/=(float a) noexcept                { vec /= a; return *this; }

        vec4        operator-() const noexcept                  { vec4 r; r.vec = -vec; return r; }
        vec4        operator+(const vec4& a) const noexcept     { vec4 r; r.vec = vec + a.vec; return r; }
        vec4        operator-(const vec4& a) const noexcept     { vec4 r; r.vec = vec - a.vec; return r; }
        vec4        operator*(const vec4& a) const noexcept     { vec4 r; r.vec = vec * a.vec; return r; }
        vec4        operator*(float a) const noexcept           { vec4 r; r.vec = vec * a; return r; }
        vec4        operator/(float a) const noexcept           { vec4 r; r.vec = vec / a; return r; }

        float       length() const noexcept                     { return glm::length(vec); }
        float       dot(const vec4& a) const noexcept           { return glm::dot(vec, a.vec); }

        vec4        normalize() const noexcept                  { vec4 r; r.vec = glm::normalize(vec); return r; }
        void        normalize_self() noexcept                   { vec = glm::normalize(vec); }

    public:
        union
        {
            struct
            {
                float   x, y, z, w;
            };
            glm::vec4   vec;
        };
    }; // class vec4

    static_assert(sizeof(vec4) == 16, "incorreect size");

} // namespcace green::core
