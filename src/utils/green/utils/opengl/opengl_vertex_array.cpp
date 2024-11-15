#include <green/utils/opengl/opengl_vertex_array.hpp>

#include <green/utils/log.hpp>

namespace
{
    green::utils::logger log("opengl_vertex_array");
} // namespace

namespace green::utils
{

    /* opengl_vertex_array::initialize */
    void opengl_vertex_array::initialize() noexcept
    {
        if (m_object) {
            log.error("Already initialized.");
        }
        GL_CALL(glGenVertexArrays(1, &m_object));
    }

    /* opengl_vertex_array::finalize */
    void opengl_vertex_array::finalize() noexcept
    {
        if (m_object) {
            GL_CALL(glDeleteVertexArrays(1, &m_object));
            m_object = 0;
        } else {
            log.error("Not initialized yet.");
        }
    }

    /* opengl_vertex_array::bind */
    void opengl_vertex_array::bind() const noexcept
    {
        GL_CALL(glBindVertexArray(m_object));
    }

    /* opengl_vertex_array::unbind */
    void opengl_vertex_array::unbind() const noexcept
    {
        GL_CALL(glBindVertexArray(0));
    }

    /* opengl_vertex_array::vertex_attribute_pointer */
    void opengl_vertex_array::vertex_attribute_pointer(GLuint index, GLenum type, GLint number, GLsizei stride_in_bytes, GLuint offset_in_bytes) noexcept
    {
        GL_CALL(glVertexAttribPointer(index, number, type, GL_FALSE, stride_in_bytes, reinterpret_cast<void*>(offset_in_bytes)));
        GL_CALL(glEnableVertexAttribArray(index));
    }

} // namespace green::utils
