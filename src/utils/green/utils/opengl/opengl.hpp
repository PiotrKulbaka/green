#pragma once

#include <green/utils/log.hpp>

#include <glad/glad.h>
#include <cassert>
#include <cstdint>

namespace green::utils
{

    void opengl_check_errors(const char* file, int line);

    template <class T>
    class opengl_object
    {
        static_assert(sizeof(T) == sizeof(GLint), "only OpenGL types available");
    public:

        T get_object_id() const noexcept
        {
            return m_object;
        }
    protected:
        T m_object {0};
    };

}

#if defined(_DEBUG) || defined(DEBUG) || true
    #define GL_CHECK_ERROR()                                       \
        do {                                                       \
            green::utils::opengl_check_errors(__FILE__, __LINE__); \
        } while (0)
#else
    #define GL_CHECK_ERROR() ((void) 0)
#endif
#define GL_CALL(FUNC)     \
    do {                  \
        FUNC;             \
        GL_CHECK_ERROR(); \
    } while (0)
