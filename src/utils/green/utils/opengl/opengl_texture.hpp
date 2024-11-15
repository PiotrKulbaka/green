#pragma once

#include <green/utils/opengl/opengl.hpp>

namespace green::utils
{

    template <GLenum Target>
    class opengl_texture : public opengl_object<GLuint>
    {
        static constexpr const char* tag = "opengl_texture";
    public:

        opengl_texture() = default;
        ~opengl_texture() = default;

        void initialize() noexcept
        {
            if (m_object) {
                log_error(tag, "Already initialized.");
            }
            GL_CALL(glGenTextures(1, &m_object));
        }

        void finalize() noexcept
        {
            if (m_object) {
                GL_CALL(glDeleteTextures(1, &m_object));
                m_object = 0;
            } else {
                log_error(tag, "Not initialized yet.");
            }
        }

        void bind() const noexcept
        {
            //GL_CALL(glActiveTexture(GL_TEXTURE0));
            GL_CALL(glBindTexture(Target, m_object));
        }

        void unbind() const noexcept
        {
            GL_CALL(glBindTexture(Target, 0));
        }
    }; // class opengl_texture



    class opengl_texture_2d : public opengl_texture<GL_TEXTURE_2D>
    {
    public:
        void set_texture_data(const void* data, GLsizei stride_in_pixels, GLsizei channels_number, GLsizei width, GLsizei height) noexcept
        {
            assert(channels_number >= 1 && channels_number <= 4);
            static const int32_t texture_formats[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
            auto texture_format = texture_formats[channels_number - 1];

            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, stride_in_pixels));
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0, texture_format, GL_UNSIGNED_BYTE, data));
            GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
            m_width = width;
            m_height = height;
        }

        GLsizei get_width() const noexcept
        {
            return m_width;
        }

        GLsizei get_height() const noexcept
        {
            return m_height;
        }

    private:
        GLsizei m_width {0};
        GLsizei m_height {0};
    }; // opengl_texture_2d

} // namespace green::utils
