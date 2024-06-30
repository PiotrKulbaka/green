#pragma once

#include <green/utils/opengl/opengl.hpp>

namespace green::utils
{

    class opengl_texture_object
    {
    public:
        opengl_texture_object() noexcept;
        ~opengl_texture_object() noexcept;

        void set_texture2d_data(const void* data, GLsizei stride_in_pixels, GLsizei channels_number, GLsizei width, GLsizei height) noexcept;

        void bind() const noexcept;
        void unbind() const noexcept;

        GLsizei get_width() const noexcept;
        GLsizei get_height() const noexcept;
        GLuint get_object_id() const noexcept;
    private:
        GLuint m_object {0};
        GLenum m_target {0};
        GLsizei m_width {0};
        GLsizei m_height {0};
    }; // class opengl_texture

} // namespace green::utils
