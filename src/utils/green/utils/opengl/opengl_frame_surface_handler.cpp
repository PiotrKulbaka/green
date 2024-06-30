#include <green/utils/opengl/opengl_frame_surface_handler.hpp>

namespace green::utils
{

    /* opengl_frame_surface_handler::opengl_frame_surface_handler */
    opengl_frame_surface_handler::opengl_frame_surface_handler() noexcept
    {
        constexpr float drawing_plane_coords[] = {
            // X      Y     Z     U     V
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // vertex 0 bottom left
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // vertex 1 bottom right
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // vertex 2 top left
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // vertex 3 top right
        };
        m_vbo.set_vertices_buffer_data(drawing_plane_coords, sizeof(drawing_plane_coords));
        m_vao.bind();
        m_vbo.bind();
        m_vao.vertex_attribute_pointer(GL_FLOAT, 3, sizeof(float) * 5, 0);
        m_vao.vertex_attribute_pointer(GL_FLOAT, 2, sizeof(float) * 5, sizeof(float) * 3);
        m_vbo.unbind();
        m_vbo.unbind();
    }

    /* opengl_frame_surface_handler::opengl_frame_surface_handler */
    opengl_frame_surface_handler::~opengl_frame_surface_handler() noexcept = default;

    /* opengl_frame_surface_handler::draw_surface */
    void opengl_frame_surface_handler::draw_surface() const noexcept
    {
        m_vao.bind();
        GL_CALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
        m_vao.unbind();
    }

} // namespace green::utils
