#include <green/utils/opengl/opengl_texture_object.hpp>

#include <cassert>

namespace green::utils
{

    /* opengl_texture_object::opengl_texture_object */
    opengl_texture_object::opengl_texture_object() noexcept
    {
        GL_CALL(glGenTextures(1, &m_object));
    }

    /* opengl_texture_object::~opengl_texture_object */
    opengl_texture_object::~opengl_texture_object() noexcept
    {
        GL_CALL(glDeleteTextures(1, &m_object));
        m_object = 0;
    }

    /* opengl_texture_object::set_texture2d_data */
    void opengl_texture_object::set_texture2d_data(const void* data, GLsizei stride_in_pixels, GLsizei channels_number, GLsizei width, GLsizei height) noexcept
    {
        assert(channels_number >= 1 && channels_number <= 4);
        static const int32_t texture_formats[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
        auto texture_format = texture_formats[channels_number - 1];

        m_target = GL_TEXTURE_2D;
        bind();
        GL_CALL(glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, stride_in_pixels));
        GL_CALL(glTexImage2D(m_target, 0, texture_format, width, height, 0, texture_format, GL_UNSIGNED_BYTE, data));
        GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
        unbind();
        m_width = width;
        m_height = height;
    }

    /* opengl_texture_object::bind */
    void opengl_texture_object::bind() const noexcept
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0));
        GL_CALL(glBindTexture(m_target, m_object));
    }

    /* opengl_texture_object::unbind */
    void opengl_texture_object::unbind() const noexcept
    {
        GL_CALL(glBindTexture(m_target, 0));
    }

    /* opengl_texture_object::get_width */
    GLsizei opengl_texture_object::get_width() const noexcept
    {
        return m_width;
    }

    /* opengl_texture_object::get_height */
    GLsizei opengl_texture_object::get_height() const noexcept
    {
        return m_height;
    }

    /* opengl_texture_object::get_object_id */
    GLuint opengl_texture_object::get_object_id() const noexcept
    {
        return m_object;
    }

} // namespace green::utils
