#include <cstdint>
#include <memory>
#include <thread>
#include <string_view>
#include <iostream>

#include "global_manager.hpp"

#include <green/utils/opengl/opengl.hpp>
#include <green/utils/opengl/opengl_program.hpp>
#include <green/utils/opengl/opengl_frame_surface_handler.hpp>
#include <green/utils/opengl/opengl_texture.hpp>
#include <green/utils/opengl/opengl_buffer.hpp>

#include <green/core/vec2.hpp>
#include <green/core/vec3.hpp>
#include <green/core/vec4.hpp>

//constexpr std::string_view vertex_shader_source =
//    "#version 330 core\n"
//    "precision highp float;\n "
//    "layout (location = 0) in vec2 a_pos;\n"
//    "layout (location = 1) in vec2 a_tex_coord;\n"
//    "uniform vec2 u_scale;\n"
//    "out vec2 tex_coord;\n"
//    "void main() {\n"
//    "  gl_Position = vec4(a_pos.x * u_scale.x, a_pos.y * u_scale.y, 0.0, 1.0);\n"
//    "  tex_coord = a_tex_coord;\n"
//    "}\n";
//
//constexpr std::string_view fragment_shader_source =
//    "#version 330 core\n"
//    "precision highp float;\n"
//    "in vec2 tex_coord;\n"
//    "out vec4 frag_color;\n"
//    "uniform sampler2D u_sampler;\n"
//    "void main() {\n"
//    "  frag_color = texture(u_sampler, tex_coord);\n"
////    "  frag_color = vec4(tex_coord.x, tex_coord.y, 0.0, 0.0);\n"
//    "}\n";

constexpr std::string_view vertex_shader_source =
    "#version 330 core\n"
    "precision highp float;\n "
    "layout (location = 0) in vec3 a_pos;\n"
    "out vec3 pos;\n"
    "void main() {\n"
    "  pos = a_pos;\n"
    "  gl_Position = vec4(a_pos, 1.0);\n"
    "}\n";

constexpr std::string_view fragment_shader_source =
    "#version 330 core\n"
    "precision highp float;\n"
    "in vec3 pos;\n"
    "out vec4 frag_color;\n"
    "uniform sampler2D u_sampler;\n"
    "void main() {\n"
    "  frag_color = vec4((pos.x + 1.0) / 2.0, (pos.y + 1.0) / 2.0, (pos.z + 1.0) / 2.0, 0.0);\n"
    "}\n";

#include <stb/stb.hpp>


std::unique_ptr<green::global_manager> global;

extern const float g_vertices[3311*3];
extern const uint16_t g_indices[19437];

enum class content_mode : int32_t
{
    aspect_fit,
    aspect_fill,
    aspect_fill_width,
    aspect_fill_height,
    fill
};

green::core::vec2 calculate_content_scale(content_mode mode, int32_t view_w, int32_t view_h, int32_t content_w, int32_t content_h)
{
    auto ws = float(view_w) / float(content_w);
    auto hs = float(view_h) / float(content_h);
    switch (mode) {
        case content_mode::aspect_fit:
            return {ws > hs ? hs / ws : 1.0f, ws > hs ? 1.0f : ws / hs};
        case content_mode::aspect_fill:
            return {ws < hs ? hs / ws : 1.0f, ws < hs ? 1.0f : ws / hs};
        case content_mode::aspect_fill_width:
            return {1.0f, ws / hs};
        case content_mode::aspect_fill_height:
            return {hs / ws, 1.0f};
        case content_mode::fill:
            return {1.0f, 1.0f};
    }
}

class game
{
public:
    game()
        : m_shader()
    {
        m_shader.initialize(vertex_shader_source.data(), fragment_shader_source.data());
//        u_sampler = m_shader.get_uniform_location("u_sampler");
//        u_scale = m_shader.get_uniform_location("u_scale");
        constexpr float rect_vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
        };

        constexpr uint8_t rect_indices[] = {
            0, 1, 2, 2, 3, 0,
        };

        m_vbo.initialize();
        m_ibo.initialize();
        m_vao.initialize();

        m_vao.bind();
        m_vbo.bind();
//        m_vbo.set_buffer_data(rect_vertices, sizeof(rect_vertices));
        m_vbo.set_buffer_data(g_vertices, sizeof(g_vertices));
        m_ibo.bind();
//        m_ibo.set_buffer_data(rect_indices, sizeof(rect_indices));
        m_ibo.set_buffer_data(g_indices, sizeof(g_indices));
//        m_vao.vertex_attribute_pointer(0, GL_FLOAT, 2, sizeof(float) * 4, sizeof(float) * 0);
//        m_vao.vertex_attribute_pointer(1, GL_FLOAT, 2, sizeof(float) * 4, sizeof(float) * 2);
        m_vao.vertex_attribute_pointer(0, GL_FLOAT, 3, sizeof(float) * 3, sizeof(float) * 0);
        m_vao.unbind();

        load_texture(tex_board, RESOURCES_DIRECTORY "/chess_board.jpg");
        load_texture(tex_checker_black, RESOURCES_DIRECTORY "/checkers_chip_black.png");
        load_texture(tex_checker_black_king, RESOURCES_DIRECTORY "/checkers_chip_black_king.png");
        load_texture(tex_checker_white, RESOURCES_DIRECTORY "/checkers_chip_white.png");
        load_texture(tex_checker_white_king, RESOURCES_DIRECTORY "/checkers_chip_white_king.png");


        auto min = green::core::vec3(g_vertices[0], g_vertices[1], g_vertices[2]);
        auto max = green::core::vec3(g_vertices[0], g_vertices[1], g_vertices[2]);

        for (int i = 3; i < 3311*3; i += 3) {
            auto v = green::core::vec3(g_vertices[i + 0], g_vertices[i + 1], g_vertices[i + 2]);
            min.vec = glm::min(min.vec, v.vec);
            max.vec = glm::max(max.vec, v.vec);
        }



        std::cout << "min(" << min.x << ", " << min.y << ", " << min.z << ")" << std::endl;
        std::cout << "max(" << max.x << ", " << max.y << ", " << max.z << ")" << std::endl;
    }

    ~game()
    {
        m_shader.finalize();
        tex_board.finalize();
        tex_checker_black.finalize();
        tex_checker_black_king.finalize();
        tex_checker_white.finalize();
        tex_checker_white_king.finalize();
    }

    void update()
    {

    }

    void render()
    {
        global->main_window.make_context_current();

        auto width = global->render_width;
        auto height = global->render_height;

        glViewport(0, 0, width, height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        m_shader.bind();

//        auto scale = calculate_content_scale(content_mode::aspect_fit, width, height, tex_board.get_width(), tex_board.get_height());
//        m_shader.set_uniform_vec2(u_scale, &scale.x);
//
//        m_shader.set_uniform_texture(u_sampler, tex_board.get_object_id(), 0);
        m_vao.bind();
        GL_CALL(glDrawElements(GL_TRIANGLES, 19434, GL_UNSIGNED_SHORT, nullptr));
        m_vao.unbind();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//        m_shader.set_uniform_texture(u_sampler, tex_checker_black_king.get_object_id(), 0);
//        m_vao.bind();
//        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr));
//        m_vao.unbind();

        m_shader.unbind();

        global->main_window.swap_buffers();
    }

private:
    void load_texture(green::utils::opengl_texture_2d& tex, const std::string_view& path)
    {
        int32_t w, h, n;
        stbi_set_flip_vertically_on_load(true);
        auto* data = stbi_load(path.data(), &w, &h, &n, 0);
        tex.initialize();
        tex.bind();
        tex.set_texture_data(data, w, n, w, h);
        tex.unbind();
        stbi_image_free(data);
    }

private:
    green::utils::opengl_vertex_buffer m_vbo;
    green::utils::opengl_index_buffer m_ibo;
    green::utils::opengl_vertex_array m_vao;
    green::utils::opengl_program m_shader;
    green::utils::opengl_texture_2d tex_board, tex_checker_black, tex_checker_black_king, tex_checker_white, tex_checker_white_king;
    int32_t u_sampler, u_scale;
}; // class game



class application
{
public:
    application()
    {
        global = std::make_unique<green::global_manager>();
        global->run = true;
        global->main_window.set_glfw_events_callback([](const green::utils::glfw_event_data& e) {
            if (e.type == green::utils::glfw_event::framebuffer_resize) {
                global->render_width = static_cast<int>(e.size_width);
                global->render_height = static_cast<int>(e.size_height);
            } else if (e.type == green::utils::glfw_event::window_close) {
                global->run = false;
            } else if (e.type == green::utils::glfw_event::key_press) {
                global->input.on_press(e.keyboard_key);
            } else if (e.type == green::utils::glfw_event::key_release) {
                global->input.on_release(e.keyboard_key);
            }
        });
    }

    ~application() = default;

    int execute()
    {
        std::thread t([&]() {
            while (global->run && (global->render_width == 0 || global->render_height == 0)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            if (global->run) {
                global->main_window.make_context_current();
                game g;
                do {
                    constexpr auto interval = std::chrono::nanoseconds(uint64_t(1e9 / 60 * 0.9));
                    const auto next_frame_time = std::chrono::steady_clock::now() + interval;
                    g.update();
                    g.render();
                    std::this_thread::sleep_until(next_frame_time);
                } while (global->run);
            }
        });

        global->main_window.show_window_and_run_events_loop();
        t.join();

        return 0;
    }
}; // class application

int main(int argc, char** argv)
{
    application app;
    return app.execute();
}
