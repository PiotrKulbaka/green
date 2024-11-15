#pragma once

#include <green/utils/opengl/opengl.hpp>

namespace green::utils
{

    class opengl_vertex_array : public opengl_object<GLuint>
    {
    public:
        opengl_vertex_array() = default;
        ~opengl_vertex_array() = default;

        void initialize() noexcept;
        void finalize() noexcept;

        void bind() const noexcept;
        void unbind() const noexcept;

        void vertex_attribute_pointer(GLuint index, GLenum type, GLint number, GLsizei stride_in_bytes, GLuint offset_in_bytes) noexcept;
    }; // class opengl_vertex_array

} // namespace green::utils
