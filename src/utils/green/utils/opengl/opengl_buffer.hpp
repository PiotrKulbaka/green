#pragma once

#include <green/utils/opengl/opengl.hpp>

namespace green::utils
{

    template <GLenum Target>
    class opengl_buffer : public opengl_object<GLuint>
    {
        static constexpr const char* tag = "opengl_buffer";
    public:
        opengl_buffer() = default;
        ~opengl_buffer() = default;

        void initialize() noexcept
        {
            if (m_object) {
                log_error(tag, "Already initialized.");
            }
            GL_CALL(glGenBuffers(1, &m_object));
        }

        void finalize() noexcept
        {
            if (m_object) {
                GL_CALL(glDeleteBuffers(1, &m_object));
                m_object = 0;
            } else {
                log_error(tag, "Not initialized yet.");
            }
        }

        void bind() const noexcept
        {
            if (!m_object) {
                log_warning(tag, "Attempt to use an empty buffer object.");
            }
            GL_CALL(glBindBuffer(Target, m_object));
        }

        void unbind() const noexcept
        {
            GL_CALL(glBindBuffer(Target, 0));
        }

        void set_buffer_data(const void* data, GLsizeiptr size) noexcept
        {
            GL_CALL(glBufferData(Target, size, data, GL_STATIC_DRAW));
        }
    }; // class opengl_buffer

    using opengl_vertex_buffer = opengl_buffer<GL_ARRAY_BUFFER>;
    using opengl_index_buffer = opengl_buffer<GL_ELEMENT_ARRAY_BUFFER>;

} // namespace green::utils
