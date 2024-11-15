#pragma once

#include <green/utils/opengl/opengl_buffer.hpp>
#include <green/utils/opengl/opengl_vertex_array.hpp>

namespace green::utils
{

    class opengl_frame_surface_handler
    {
    public:
        opengl_frame_surface_handler() noexcept;
        ~opengl_frame_surface_handler() noexcept;

        void draw_surface() const noexcept;
    private:
        opengl_vertex_buffer m_vbo;
        opengl_vertex_array m_vao;
    }; // class opengl_frame_surface_handler

} // namespace green::utils
