#pragma once

#include <green/utils/opengl/opengl.hpp>

namespace green::utils
{

    class opengl_vertex_array_object
    {
    public:
        opengl_vertex_array_object() noexcept;
        ~opengl_vertex_array_object() noexcept;

        void vertex_attribute_pointer(GLenum type, GLint number, GLsizei stride_in_bytes, GLuint offset_in_bytes) noexcept;

        void bind() const noexcept;
        void unbind() const noexcept;

        GLuint get_object_id() const noexcept;
    private:
        GLuint m_object {0};
        GLuint m_attrib_index {0};
    }; // class opengl_vertex_array_object

} // namespace green::utils
