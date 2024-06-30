#pragma once

#include <green/utils/opengl/opengl_buffer_object.hpp>
#include <green/utils/opengl/opengl_vertex_array_object.hpp>

namespace green::utils
{

    class opengl_frame_surface_handler
    {
    public:
        opengl_frame_surface_handler() noexcept;
        ~opengl_frame_surface_handler() noexcept;

        void draw_surface() const noexcept;
    private:
        opengl_buffer_object m_vbo;
        opengl_vertex_array_object m_vao;
    }; // class opengl_frame_surface_handler

} // namespace green::utils
