#pragma once

#include <green/core/math_base.hpp>
#include <glm/vec3.hpp>

namespace green::core
{

    class vec3
    {
    public:
                    vec3() noexcept {}
                    vec3(float v) noexcept : x(v), y(v), z(v) {}
                    vec3(float x, float y, float z) noexcept : x{x}, y{y}, z{z} {}

        float       operator[](size_t index) const noexcept     { return (&x)[index]; }
        float&      operator[](size_t index) noexcept           { return (&x)[index]; }

        vec3&       operator+=(const vec3& a) noexcept          { vec += a.vec; return *this; }
        vec3&       operator-=(const vec3& a) noexcept          { vec -= a.vec; return *this; }
        vec3&       operator*=(float a) noexcept                { vec *= a; return *this; }
        vec3&       operator/=(const vec3& a) noexcept          { vec /= a.vec; return *this; }
        vec3&       operator/=(float a) noexcept                { vec /= a; return *this; }

        vec3        operator-() const noexcept                  { vec3 r; r.vec = -vec; return r; }
        vec3        operator+(const vec3& a) const noexcept     { vec3 r; r.vec = vec + a.vec; return r; }
        vec3        operator-(const vec3& a) const noexcept     { vec3 r; r.vec = vec - a.vec; return r; }
        vec3        operator*(const vec3& a) const noexcept     { vec3 r; r.vec = vec * a.vec; return r; }
        vec3        operator*(float a) const noexcept           { vec3 r; r.vec = vec * a; return r; }
        vec3        operator/(float a) const noexcept           { vec3 r; r.vec = vec / a; return r; }

        float       length() const noexcept                     { return glm::length(vec); }
        float       dot(const vec3& a) const noexcept           { return glm::dot(vec, a.vec); }

        vec3        cross(const vec3& a) const noexcept         { vec3 r; r.vec = glm::cross(vec, a.vec); return r; }
        vec3        normalize() const noexcept                  { vec3 r; r.vec = glm::normalize(vec); return r; }
        void        normalize_self() noexcept                   { vec = glm::normalize(vec); }

    public:
        union
        {
            struct
            {
                float   x, y, z;
            };
            struct
            {
                float   r, g, b;
            };
            glm::vec3   vec;
        };
    }; // class vec3

    static_assert(sizeof(vec3) == 12, "incorreect size");

} // namespcace green::core
