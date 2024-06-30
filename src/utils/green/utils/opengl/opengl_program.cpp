#include <green/utils/opengl/opengl_program.hpp>

#include <green/utils/log.hpp>
#include <green/utils/opengl/opengl.hpp>

namespace
{
    green::utils::logger log("opengl_program");
    green::utils::logger log_builder("opengl_program::builder");

    void opengl_delete_shader(int32_t& shader)
    {
        GL_CALL(glDeleteShader(shader));
        shader = 0;
    }

    int32_t opengl_compile_shader(uint32_t type, const char* source)
    {
        int32_t shader = glCreateShader(type);
        GL_CALL(glShaderSource(shader, 1, &source, nullptr));
        GL_CALL(glCompileShader(shader));

        // check for shader compile errors
        int32_t success;
        GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            char info_log[512];
            GL_CALL(glGetShaderInfoLog(shader, 512, nullptr, info_log));
            log_builder.error("Compilation error: %s", info_log);
            opengl_delete_shader(shader);
        }

        return shader;
    }

    void opengl_delete_program(int32_t& program)
    {
        GL_CALL(glDeleteProgram(program));
        program = 0;
    }

    int32_t opengl_link_shaders(int32_t* shaders, size_t size)
    {
        int32_t program = glCreateProgram();
        for (auto i = 0; i < size; i++) {
            if (shaders[i]) {
                GL_CALL(glAttachShader(program, shaders[i]));
            }
        }

        GL_CALL(glLinkProgram(program));
        int32_t success;
        GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(program, 512, NULL, info_log);
            log_builder.error("Linking error: %s", info_log);
            opengl_delete_program(program);
        }

        return program;
    }

} // namespace

namespace green::utils
{

    /* builder::builder */
    opengl_program::builder::builder() = default;

    /* builder::~builder */
    opengl_program::builder::~builder()
    {
        opengl_delete_shader(m_vertex_shader);
        opengl_delete_shader(m_fragment_shader);
    }

    /* builder::vertex_shader */
    opengl_program::builder& opengl_program::builder::vertex_shader(const char* source)
    {
        if (m_vertex_shader) {
            log_builder.error("Vertex shader is already compiled.");
        } else {
            m_vertex_shader = opengl_compile_shader(GL_VERTEX_SHADER, source);
        }
        return *this;
    }

    /* builder::fragment_shader */
    opengl_program::builder& opengl_program::builder::fragment_shader(const char* source)
    {
        if (m_fragment_shader) {
            log_builder.error("Fragment shader is already compiled.");
        } else {
            m_fragment_shader = opengl_compile_shader(GL_FRAGMENT_SHADER, source);
        }
        return *this;
    }

    /* builder::build */
    opengl_program opengl_program::builder::build()
    {
        opengl_program program;
        int32_t shaders[] = {m_vertex_shader, m_fragment_shader};
        auto program_id = opengl_link_shaders(shaders, 2);
        program.initialize(program_id);
        return program;
    }



    /* opengl_program::opengl_program */
    opengl_program::opengl_program() = default;

    /* opengl_program::~opengl_program */
    opengl_program::~opengl_program() = default;

    /* opengl_program::initialize */
    void opengl_program::initialize(uint32_t program)
    {
        if (m_program) {
            log.error("Attempt to initialize an already initialized program.");
        }
        m_program = program;
    }

    /* opengl_program::finalize */
    void opengl_program::finalize()
    {
        opengl_delete_program(m_program);
    }

    /* opengl_program::use */
    void opengl_program::use() const
    {
        if (!m_program) {
            log.error("Attempt to use an empty program.");
        }
        GL_CALL(glUseProgram(m_program));
    }

    /* opengl_program::unuse */
    void opengl_program::unuse()
    {
        GL_CALL(glUseProgram(0));
    }

    /* opengl_program::get_program_id */
    int32_t opengl_program::get_program_id() const
    {
        return m_program;
    }

    /* opengl_program::get_attribute_location */
    int32_t opengl_program::get_attribute_location(const char* name)
    {
        if (m_program) {
            int32_t attribute = glGetAttribLocation(m_program, name);
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
    int32_t opengl_program::get_uniform_location(const char* name)
    {
        if (m_program) {
            int32_t attribute = glGetUniformLocation(m_program, name);
            GL_CHECK_ERROR();
            if (attribute == -1) {
                log.error("Unknown uniform name: '%s'.", name);
            }
            return attribute;
        } else {
            log.error("Program is not initialized.");
        }
        return -1;
    }

    /* opengl_program::set_uniform_texture */
    void opengl_program::set_uniform_texture(int32_t uniform, uint32_t texture, uint32_t unit) const
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
        GL_CALL(glUniform1i(uniform, unit));
    }

    /* opengl_program::set_uniform_vec2 */
    void opengl_program::set_uniform_vec2(int32_t uniform, const float* const vec) const
    {
        GL_CALL(glUniform2fv(uniform, 1, vec));
    }

    /* opengl_program::set_uniform_vec4 */
    void opengl_program::set_uniform_vec4(int32_t uniform, const float* const vec) const
    {
        GL_CALL(glUniform4fv(uniform, 1, vec));
    }

    /* opengl_program::set_uniform_mat4 */
    void opengl_program::set_uniform_mat4(int32_t uniform, const float* const mat, bool transpose) const
    {
        GL_CALL(glUniformMatrix4fv(uniform, 1, static_cast<GLboolean>(transpose), mat));
    }

    /* opengl_program::set_uniform_int */
    void opengl_program::set_uniform_int(int32_t uniform, int32_t val) const
    {
        GL_CALL(glUniform1i(uniform, val));
    }

} // namespace green::utils
