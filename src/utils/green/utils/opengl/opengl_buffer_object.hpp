#pragma once

#include <green/utils/opengl/opengl.hpp>

namespace green::utils
{

    class opengl_buffer_object
    {
    public:
        opengl_buffer_object() noexcept;
        ~opengl_buffer_object() noexcept;

        void set_vertices_buffer_data(const void* data, GLsizeiptr size) noexcept;
        void set_indices_buffer_data(const void* data, GLsizeiptr size) noexcept;

        void bind() const noexcept;
        void unbind() const noexcept;

        GLuint get_object_id() const noexcept;
    private:
        GLuint m_object {0};
        GLenum m_target {0};
    }; // class opengl_buffer

} // namespace green::utils
