#include <cstdint>
#include <memory>
#include <thread>
#include <string_view>

#include <green/utils/glfw/glfw_window.hpp>

#include <green/utils/opengl/opengl.hpp>
#include <green/utils/opengl/opengl_program.hpp>
#include <green/utils/opengl/opengl_frame_surface_handler.hpp>
#include <green/utils/opengl/opengl_texture_object.hpp>
#include <green/utils/opengl/opengl_buffer_object.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void load_texture(green::utils::opengl_texture_object& tex, const std::string_view& path)
{
    int32_t w, h, n;
    stbi_set_flip_vertically_on_load(true);
    auto* data = stbi_load(path.data(), &w, &h, &n, 4);
    tex.set_texture2d_data(data, w, n, w, h);
    stbi_image_free(data);
}

green::utils::opengl_program load_program(const std::string_view& vert, const std::string_view& frag)
{
    return green::utils::opengl_program::builder()
        .vertex_shader(vertex_shader_source.data())
        .fragment_shader(fragment_shader_source.data())
        .build();
}

enum class content_mode : int32_t
{
    aspect_fit,
    aspect_fill,
    aspect_fill_width,
    aspect_fill_height,
    fill
};

struct scale
{
    float sx;
    float xy;
};

scale calculate_content_scale(content_mode mode, int32_t view_w, int32_t view_h, int32_t content_w, int32_t content_h)
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

int main(int argc, char** argv)
{
    auto window = std::make_shared<green::utils::glfw_window>("Green test application");

    auto run_render_thread = true;
    auto width = 0;
    auto height = 0;

    window->make_context_current();

    auto prog = load_program(vertex_shader_source, fragment_shader_source);
    green::utils::opengl_texture_object tex1, tex2, tex3;
    load_texture(tex1, GREEN_PATH_TO_RES_DIRECTORY "/image1.png");
    load_texture(tex2, GREEN_PATH_TO_RES_DIRECTORY "/image2.png");
    load_texture(tex3, GREEN_PATH_TO_RES_DIRECTORY "/image3.png");

    auto u_sampler = prog.get_uniform_location("uTexture");
    auto u_matrix = prog.get_uniform_location("uMatrix");

    green::utils::opengl_frame_surface_handler frame_surface_handler;
    green::utils::opengl_frame_surface_handler frame_surface_handler2;
    green::utils::opengl_texture_object* tex = &tex1;

    prog.use();
    prog.set_uniform_texture(u_sampler, tex1.get_object_id(), 0);
    prog.unuse();

    window->make_nothing_current();
    content_mode m = content_mode::fill;

    float angle = 0.0f;

    std::thread t([&]() {
        const auto interval = std::chrono::nanoseconds(uint64_t(1e9 / 60 * 0.9));

        while (run_render_thread) {
            const auto next_frame_time = std::chrono::steady_clock::now() + interval;
            if (width <= 0 || height <= 0) {
                std::this_thread::sleep_until(next_frame_time);
                continue;
            }

            window->make_context_current();

            auto [sx, sy] = calculate_content_scale(m, width, height, tex->get_width(), tex->get_height());

//            float mat[] = {
//                sx, 0.0f, 0.0f, 0.0f,
//                0.0f, sy, 0.0f, 0.0f,
//                0.0f, 0.0f, 1.0f, 0.0f,
//                0.0f, 0.0f, 0.0f, 1.0f
//            };

            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 10.0f);
            projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 10.0f);
            view = glm::translate(view, glm::vec3(1.0f, 0.0f, -3.0f));

            glEnable(GL_DEPTH_TEST);

            auto result =  projection * view;

            result = glm::rotate(result, angle, glm::vec3(1.0f, 0.3f, 0.2f));

            glViewport(0, 0, width, height);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            prog.use();
            prog.set_uniform_mat4(u_matrix, (float*)(void*)&result);
            prog.set_uniform_texture(u_sampler, tex1.get_object_id(), 0);
            frame_surface_handler.draw_surface();

            view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(2.0f, -1.0f, -6.0f));
            result =  projection * view;
            result = glm::rotate(result, angle, glm::vec3(0.0f, 0.4f, 0.8f));

            prog.set_uniform_mat4(u_matrix, (float*)(void*)&result);
            prog.set_uniform_texture(u_sampler, tex2.get_object_id(), 0);
            frame_surface_handler.draw_surface();

            view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(2.0f, 2.0f, -6.0f));
            result =  projection * view;
            result = glm::rotate(result, angle, glm::vec3(0.3f, 0.5f, 0.7f));

            prog.set_uniform_mat4(u_matrix, (float*)(void*)&result);
            prog.set_uniform_texture(u_sampler, tex3.get_object_id(), 0);
            frame_surface_handler.draw_surface();

            prog.unuse();

            window->swap_buffers();
            std::this_thread::sleep_until(next_frame_time);

            angle += 0.007f;
        }
    });

    window->set_glfw_events_callback([&](const green::utils::glfw_event_data& e) {
        if (e.type == green::utils::glfw_event::framebuffer_resize) {
            width = static_cast<int>(e.size_width);
            height = static_cast<int>(e.size_height);
        } else if (e.type == green::utils::glfw_event::window_close) {
            run_render_thread = false;
        } else if (e.type == green::utils::glfw_event::key_press) {
            if (e.keyboard_key == GLFW_KEY_1) {
                m = content_mode::aspect_fit;
            } else if (e.keyboard_key == GLFW_KEY_2) {
                m = content_mode::aspect_fill;
            } else if (e.keyboard_key == GLFW_KEY_3) {
                m = content_mode::aspect_fill_width;
            } else if (e.keyboard_key == GLFW_KEY_4) {
                m = content_mode::aspect_fill_height;
            } else if (e.keyboard_key == GLFW_KEY_5) {
                m = content_mode::fill;
            }
        }
    });

    window->show_window_and_run_events_loop();
    t.join();

    window->make_context_current();
    prog.finalize();

    return 0;
}
