#include <cstdint>
#include <memory>
#include <thread>

#include <green/utils/glfw/glfw_window.hpp>

#include <green/utils/opengl/opengl.hpp>
#include <green/utils/opengl/opengl_program.hpp>
#include <green/utils/opengl/opengl_frame_surface_handler.hpp>
#include <green/utils/opengl/opengl_texture.hpp>

constexpr std::string_view vertex_shader_source =
    "#version 330 core\n"
    "precision highp float;\n "
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "uniform mat4 uMatrix;\n"
    "out vec2 vTexCoord;\n"
    "void main() {\n"
    "  gl_Position = uMatrix * vec4(aPos, 1.0);\n"
    "  vTexCoord = aTexCoord;\n"
    "}\n";

constexpr std::string_view fragment_shader_source =
    "#version 330 core\n"
    "precision highp float;\n"
    "in vec2 vTexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D uTexture;\n"
    "void main() {\n"
    "  FragColor = texture(uTexture, vTexCoord);\n"
    "}\n";

#include <stb/stb.hpp>

int main(int argc, char** argv)
{
    auto window = std::make_shared<green::utils::glfw_window>("Green test application");

    auto run_render_thread = true;
    auto width = 0;
    auto height = 0;

    window->make_context_current();

    auto prog = green::utils::opengl_program::builder()
        .vertex_shader(vertex_shader_source.data())
        .fragment_shader(fragment_shader_source.data())
        .build();

    float mat[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    auto u_sampler = prog.get_uniform_location("uTexture");
    auto u_matrix = prog.get_uniform_location("uMatrix");
    green::utils::opengl_frame_surface_handler frame_surface_handler;

    int32_t w, h, n;
    auto* data = stbi_load(GREEN_PATH_TO_RES_DIRECTORY "/image1.png", &w, &h, &n, 4);

    auto tex = green::utils::opengl_texture::builder().build();

    tex.set_texture_data(data, w, n, w, h);

    stbi_image_free(data);

    window->make_nothing_current();

    std::thread t([&]() {
        const auto interval = std::chrono::nanoseconds(uint64_t(1e9 / 60 * 0.9));

        while (run_render_thread) {
            const auto next_frame_time = std::chrono::steady_clock::now() + interval;
            if (width <= 0 || height <= 0) {
                std::this_thread::sleep_until(next_frame_time);
                continue;
            }

            window->make_context_current();

            glViewport(0, 0, width, height);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            prog.use();

            tex.bind_and_active_texture_unit(0);
            prog.set_uniform_texture_unit(u_sampler, 0);
            prog.set_uniform_mat4(u_matrix, mat);

            frame_surface_handler.draw_surface();

            window->swap_buffers();
            std::this_thread::sleep_until(next_frame_time);
        }
    });

    window->set_glfw_events_callback([&](const green::utils::glfw_event_data& e) {
        if (e.type == green::utils::glfw_event::framebuffer_resize) {
            width = static_cast<int>(e.size_width);
            height = static_cast<int>(e.size_height);
        } else if (e.type == green::utils::glfw_event::window_close) {
            run_render_thread = false;
        }
    });

    window->show_window_and_run_events_loop();
    t.join();
    
    
    window->make_context_current();
    tex.finalize();
    prog.finalize();

    return 0;
}
