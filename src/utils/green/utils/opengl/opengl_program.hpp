#pragma once

#include <green/utils/opengl/opengl.hpp>

namespace green::utils
{

    class opengl_program : public opengl_object<GLuint>
    {
    public:
        opengl_program() = default;
        ~opengl_program() = default;

        void initialize(const char* vertex_shader_source, const char* fragment_shader_source) noexcept;
        void finalize() noexcept;

        void bind() const noexcept;
        void unbind() const noexcept;

        GLint get_attribute_location(const char* name);
        GLint get_uniform_location(const char* name);

        void set_uniform_texture(GLint uniform, GLuint texture, GLenum unit) const;
        void set_uniform_vec2(GLint uniform, const float* const vec) const;
        void set_uniform_vec4(GLint uniform, const float* const vec) const;
        void set_uniform_mat3(GLint uniform, const float* const mat, bool transpose = false) const;
        void set_uniform_mat4(GLint uniform, const float* const mat, bool transpose = false) const;
        void set_uniform_int(GLint uniform, int32_t val) const;
    }; // class opengl_shader_program

} // namespace green::utils
