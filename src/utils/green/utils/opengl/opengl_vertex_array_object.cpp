#include <green/utils/opengl/opengl_vertex_array_object.hpp>

namespace green::utils
{

    /* opengl_vertex_array_object::opengl_vertex_array_object */
    opengl_vertex_array_object::opengl_vertex_array_object() noexcept
    {
        GL_CALL(glGenVertexArrays(1, &m_object));
    }

    /* opengl_vertex_array_object::~opengl_vertex_array_object */
    opengl_vertex_array_object::~opengl_vertex_array_object() noexcept
    {
        GL_CALL(glDeleteVertexArrays(1, &m_object));
        m_object = 0;
    }

    /* opengl_vertex_array_object::vertex_attribute_pointer */
    void opengl_vertex_array_object::vertex_attribute_pointer(GLenum type, GLint number, GLsizei stride_in_bytes, GLuint offset_in_bytes) noexcept
    {
        GL_CALL(glBindVertexArray(m_object));
        GL_CALL(glVertexAttribPointer(m_attrib_index, number, type, GL_FALSE, stride_in_bytes, reinterpret_cast<void*>(offset_in_bytes)));
        GL_CALL(glEnableVertexAttribArray(m_attrib_index));
        GL_CALL(glBindVertexArray(0));
        m_attrib_index++;
    }

    /* opengl_vertex_array_object::bind */
    void opengl_vertex_array_object::bind() const noexcept
    {
        GL_CALL(glBindVertexArray(m_object));
    }

    /* opengl_vertex_array_object::unbind */
    void opengl_vertex_array_object::unbind() const noexcept
    {
        GL_CALL(glBindVertexArray(0));
    }

    /* opengl_vertex_array_object::get_object_id */
    GLuint opengl_vertex_array_object::get_object_id() const noexcept
    {
        return m_object;
    }

} // namespace green::utils
