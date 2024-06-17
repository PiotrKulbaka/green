#pragma once

#include <cstdint>

namespace green::utils
{

    class opengl_frame_surface_handler
    {
    public:
        opengl_frame_surface_handler();
        ~opengl_frame_surface_handler();

        void draw_surface();

    private:
        uint32_t m_vbo{0};
        uint32_t m_vao{0};
    }; // class opengl_frame_surface_handler

} // namespace green::utils
