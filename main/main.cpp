#include <cstdint>
#include <memory>
#include <thread>

#include <green/utils/glfw/glfw_window.hpp>

#include <glad/glad.h>

int main(int argc, char** argv)
{
    auto window = std::make_shared<green::utils::glfw_window>("Green test application");

    auto run_render_thread = true;
    auto width = 0;
    auto height = 0;
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

    return 0;
}
