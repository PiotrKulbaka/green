#include <green/utils/opengl/opengl_program.hpp>

#include <green/utils/log.hpp>

namespace
{
    green::utils::logger log("opengl_program");

    void opengl_delete_shader(GLuint& shader)
    {
        GL_CALL(glDeleteShader(shader));
        shader = 0;
    }

    GLuint opengl_compile_shader(GLenum type, const char* source)
    {
        auto shader = glCreateShader(type);
        GL_CALL(glShaderSource(shader, 1, &source, nullptr));
        GL_CALL(glCompileShader(shader));

        // check for shader compile errors
        GLint success;
        GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            char info_log[512];
            GL_CALL(glGetShaderInfoLog(shader, 512, nullptr, info_log));
            log.error("Compilation error: %s", info_log);
            opengl_delete_shader(shader);
        }
        return shader;
    }

    void opengl_delete_program(GLuint& program)
    {
        GL_CALL(glDeleteProgram(program));
        program = 0;
    }

    GLuint opengl_link_shaders(GLuint* shaders, size_t size)
    {
        auto program = glCreateProgram();
        for (auto i = 0; i < size; i++) {
            if (shaders[i]) {
                GL_CALL(glAttachShader(program, shaders[i]));
            }
        }

        GL_CALL(glLinkProgram(program));
        GLint success;
        GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(program, 512, NULL, info_log);
            log.error("Linking error: %s", info_log);
            opengl_delete_program(program);
        }
        return program;
    }

} // namespace

namespace green::utils
{

    /* opengl_program::initialize */
    void opengl_program::initialize(const char* vertex_shader_source, const char* fragment_shader_source) noexcept
    {
        if (m_object) {
            log.error("Already initialized.");
        }
        auto vertex_shader = opengl_compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = opengl_compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        GLuint shaders[] = {vertex_shader, fragment_shader};
        m_object = opengl_link_shaders(shaders, 2);
        opengl_delete_shader(vertex_shader);
        opengl_delete_shader(fragment_shader);
    }

    /* opengl_program::finalize */
    void opengl_program::finalize() noexcept
    {
        if (m_object) {
            opengl_delete_program(m_object);
        } else {
            log.error("Not initialized yet.");
        }
    }

    /* opengl_program::bind */
    void opengl_program::bind() const noexcept
    {
        if (!m_object) {
            log.error("Attempt to use an empty program.");
        }
        GL_CALL(glUseProgram(m_object));
    }

    /* opengl_program::unbind */
    void opengl_program::unbind() const noexcept
    {
        GL_CALL(glUseProgram(0));
    }

    /* opengl_program::get_attribute_location */
    GLint opengl_program::get_attribute_location(const char* name)
    {
        if (m_object) {
            auto attribute = glGetAttribLocation(m_object, name);
            GL_CHECK_ERROR();
            if (attribute == -1) {
                log.error("Unknown attribute name: '%s'.", name);
            }
            return attribute;
        } else {
            log.error("Program is not initialized.");
        }
        return -1;
    }

    /* opengl_program::get_uniform_location */
    GLint opengl_program::get_uniform_location(const char* name)
    {
        if (m_object) {
            auto uniform = glGetUniformLocation(m_object, name);
            GL_CHECK_ERROR();
            if (uniform == -1) {
                log.error("Unknown uniform name: '%s'.", name);
            }
            return uniform;
        } else {
            log.error("Program is not initialized.");
        }
        return -1;
    }

    /* opengl_program::set_uniform_texture */
    void opengl_program::set_uniform_texture(GLint uniform, GLuint texture, GLenum unit) const
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
        GL_CALL(glUniform1i(uniform, unit));
    }

    /* opengl_program::set_uniform_vec2 */
    void opengl_program::set_uniform_vec2(GLint uniform, const float* const vec) const
    {
        GL_CALL(glUniform2fv(uniform, 1, vec));
    }

    /* opengl_program::set_uniform_vec4 */
    void opengl_program::set_uniform_vec4(GLint uniform, const float* const vec) const
    {
        GL_CALL(glUniform4fv(uniform, 1, vec));
    }

    /* opengl_program::set_uniform_mat3 */
    void opengl_program::set_uniform_mat3(GLint uniform, const float* const mat, bool transpose) const
    {
        GL_CALL(glUniformMatrix3fv(uniform, 1, static_cast<GLboolean>(transpose), mat));
    }

    /* opengl_program::set_uniform_mat4 */
    void opengl_program::set_uniform_mat4(GLint uniform, const float* const mat, bool transpose) const
    {
        GL_CALL(glUniformMatrix4fv(uniform, 1, static_cast<GLboolean>(transpose), mat));
    }

    /* opengl_program::set_uniform_int */
    void opengl_program::set_uniform_int(GLint uniform, int32_t val) const
    {
        GL_CALL(glUniform1i(uniform, val));
    }

} // namespace green::utils
