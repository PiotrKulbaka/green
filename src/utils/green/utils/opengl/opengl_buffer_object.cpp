#include <green/utils/opengl/opengl_buffer_object.hpp>

namespace green::utils
{

    /* opengl_buffer_object::opengl_buffer_object */
    opengl_buffer_object::opengl_buffer_object() noexcept
    {
        GL_CALL(glGenBuffers(1, &m_object));
    }

    /* opengl_buffer_object::~opengl_buffer_object */
    opengl_buffer_object::~opengl_buffer_object() noexcept
    {
        GL_CALL(glDeleteBuffers(1, &m_object));
    }

    /* opengl_buffer_object::set_vertices_buffer_data */
    void opengl_buffer_object::set_vertices_buffer_data(const void* data, GLsizeiptr size) noexcept
    {
        m_target = GL_ARRAY_BUFFER;
        GL_CALL(glBindBuffer(m_target, m_object));
        GL_CALL(glBufferData(m_target, size, data, GL_STATIC_DRAW));
        GL_CALL(glBindBuffer(m_target, 0));
    }

    /* opengl_buffer_object::set_indices_buffer_data */
    void opengl_buffer_object::set_indices_buffer_data(const void* data, GLsizeiptr size) noexcept
    {
        m_target = GL_ELEMENT_ARRAY_BUFFER;
        GL_CALL(glBindBuffer(m_target, m_object));
        GL_CALL(glBufferData(m_target, size, data, GL_STATIC_DRAW));
        GL_CALL(glBindBuffer(m_target, 0));
    }

    /* opengl_buffer_object::bind */
    void opengl_buffer_object::bind() const noexcept
    {
        GL_CALL(glBindBuffer(m_target, m_object));
    }

    /* opengl_buffer_object::unbind */
    void opengl_buffer_object::unbind() const noexcept
    {
        GL_CALL(glBindBuffer(m_target, 0));
    }

    /* opengl_buffer_object::get_object_id */
    GLuint opengl_buffer_object::get_object_id() const noexcept
    {
        return m_object;
    }

} // namespace green::utils
