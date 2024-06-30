#pragma once

#include <green/utils/noncopyable.hpp>
#include <cstdint>

namespace green::utils
{

    class opengl_program
    {
    public:
        class builder : noncopyable
        {
        public:
            builder();
            ~builder();

            builder& vertex_shader(const char* source);
            builder& fragment_shader(const char* source);

            opengl_program build();
        private:
            int32_t m_vertex_shader {0};
            int32_t m_fragment_shader {0};
        }; // class builder

    public:
        opengl_program();
        ~opengl_program();

        void initialize(uint32_t program_id);
        void finalize();

        void use() const;
        static void unuse();

        int32_t get_program_id() const;
        int32_t get_attribute_location(const char* name);
        int32_t get_uniform_location(const char* name);

        void set_uniform_texture(int32_t uniform, uint32_t texture, uint32_t unit) const;
        void set_uniform_vec2(int32_t uniform, const float* const vec) const;
        void set_uniform_vec4(int32_t uniform, const float* const vec) const;
        void set_uniform_mat4(int32_t uniform, const float* const mat, bool transpose = false) const;
        void set_uniform_int(int32_t uniform, int32_t val) const;

    private:
        int32_t m_program {0};
    }; // class opengl_shader_program

} // namespace green::utils
