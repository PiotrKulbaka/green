#include <green/utils/glfw/glfw_window.hpp>

#include <cmath>
#include <cassert>
#include <cstdio>
#include <stdexcept>

#include <glad/glad.h>

namespace
{

    void glfw_error(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error: %s\n", description);
    }

    bool calculate_window_size_and_pos(int32_t& x, int32_t& y, int32_t& w, int32_t& h)
    {
        //
        // All calculation are based on GLFW's screen coordinates
        // so result behaviour is pretty similar on both retina and
        // non-retina screens
        //

        GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();

        if (primary_monitor == nullptr) {
            glfw_error(0, "glfwGetPrimaryMonitor() error.");
            return false;
        }

        const GLFWvidmode* current_video_mode = glfwGetVideoMode(primary_monitor);

        if (current_video_mode == nullptr) {
            glfw_error(0, "glfwGetVideoMode() error.");
            return false;
        }

        auto screen_height = static_cast<float>(current_video_mode->height);
        auto screen_width = static_cast<float>(current_video_mode->width);

        // Chose the window size so that it occupies the 3/4 of the screen's width.
        // Then shrink it if the height is too high. Aspect calculated by the hints is
        // always preserved.

        constexpr float aspect = 16.0f / 9.0f;

        float scaled_w = 0.8f * screen_width;
        float scaled_h = scaled_w / aspect;

        // put in the center of the screen
        x = static_cast<int32_t>(std::max((screen_width - scaled_w), 0.f) / 2.f);
        y = static_cast<int32_t>(std::max((screen_height - scaled_h), 0.f) / 2.f);
        w = static_cast<int32_t>(scaled_w);
        h = static_cast<int32_t>(scaled_h);
        
        return true;
    }

    using namespace green::utils;

    glfw_event_data construct_event(glfw_event t, GLFWwindow* w)
    {
        glfw_event_data e;
        e.type = t;
        e.window = w;
        e.pos_x = e.pos_y = e.keyboard_mods = 0;
        return e;
    }

    glfw_event_data construct_event(glfw_event t, GLFWwindow* w, double a, double b)
    {
        glfw_event_data e;
        e.type = t;
        e.window = w;
        e.scroll_x = a;
        e.scroll_y = b;
        e.keyboard_mods = 0;
        return e;
    }

    glfw_event_data construct_event(glfw_event t, GLFWwindow* w, const char** a, int32_t b)
    {
        glfw_event_data e;
        e.type = t;
        e.window = w;
        e.file_paths = a;
        e.file_count = b;
        e.keyboard_mods = 0;
        return e;
    }

    glfw_event_data construct_event(glfw_event t, GLFWwindow* w, int32_t a, int32_t b, int32_t c = 0)
    {
        glfw_event_data e;
        e.type = t;
        e.window = w;
        e.pos_x = a;
        e.pos_y = b;
        e.keyboard_mods = c;
        return e;
    }

    glfw_event_data construct_event(glfw_event t, GLFWwindow* w, uint32_t a)
    {
        glfw_event_data e;
        e.type = t;
        e.window = w;
        e.codepoint = a;
        e.pos_y = e.keyboard_mods = 0;
        return e;
    }

    void send_event(const glfw_event_data& e)
    {
        auto* cb_raw_ptr = glfwGetWindowUserPointer(e.window);
        if (cb_raw_ptr) {
            (*reinterpret_cast<glfw_window::glfw_event_callback*>(cb_raw_ptr))(e);
        }
    }

    const char* source_string(GLenum source)
    {
        switch (source) {
            case GL_DEBUG_SOURCE_API:
                return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                return "Window System";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                return "Shader Compiler";
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                return "Source: Third Party";
            case GL_DEBUG_SOURCE_APPLICATION:
                return "Application";
            case GL_DEBUG_SOURCE_OTHER:
                return "Other";
            default:
                return "unknown";
        }
    }

    const char* type_string(GLenum type)
    {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                return "Error";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                return"Deprecated Behaviour";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                return "Undefined Behaviour";
            case GL_DEBUG_TYPE_PORTABILITY:
                return "Portability";
            case GL_DEBUG_TYPE_PERFORMANCE:
                return "Performance";
            case GL_DEBUG_TYPE_MARKER:
                return "Marker";
            case GL_DEBUG_TYPE_PUSH_GROUP:
                return "Push Group";
            case GL_DEBUG_TYPE_POP_GROUP:
                return "Pop Group";
            case GL_DEBUG_TYPE_OTHER:
                return "Other";
            default:
                return "unknown";
        }
    }

    const char* severity_string(GLenum severity)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                return "high";
            case GL_DEBUG_SEVERITY_MEDIUM:
                return "medium";
            case GL_DEBUG_SEVERITY_LOW:
                return "low";
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                return "notification";
            default:
                return "unknown";
        }
    }

    void APIENTRY debug_output(GLenum src, GLenum type, unsigned int id, GLenum severity, GLsizei len, const char* msg, const void*)
    {
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) {
            return;
        }
        const auto* ssrc = source_string(src);
        const auto* stype = type_string(type);
        const auto* ssev = severity_string(severity);
        fprintf(stderr, "[OpenGL_debug][%d][%s][%s][%s]: %s\n", id, ssev, ssrc, stype, msg);
    }

} // namespace

namespace green::utils
{

    glfw_window::glfw_window(const std::string_view& title)
    {
        // Init glfw for glfw_window
        if (!glfwInit()) {
            throw std::runtime_error("glfwInit() error");
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

        if ((m_window = glfwCreateWindow(1280, 720, title.data(), nullptr, nullptr)) == nullptr) {
            throw std::runtime_error("glfwCreateWindow() error");
        }

        make_context_current();

        if (0 == gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("gladLoadGLLoader() error");
        }

        glfwSetErrorCallback(glfw_error);
        glfwSwapInterval(0);

        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debug_output, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        make_nothing_current();
    }

    glfw_window::~glfw_window()
    {
        glfwMakeContextCurrent(nullptr);
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    /* glfw_window::make_context_current */
    void glfw_window::make_context_current()
    {
        glfwMakeContextCurrent(m_window);
    }

    /* glfw_window::make_nothing_current */
    void glfw_window::make_nothing_current()
    {
        glfwMakeContextCurrent(nullptr);
    }

    /* glfw_window::swap_buffers */
    void glfw_window::swap_buffers()
    {
        glfwSwapBuffers(m_window);
    }

    /* glfw_window::set_glfw_events_callback */
    void glfw_window::set_glfw_events_callback(glfw_event_callback callback)
    {
        m_glfw_event_callback = callback;
        if (callback == nullptr) {
            untrack_events();
        } else {
            track_events();
        }
    }

    /* glfw_window::show_window_and_run_events_loop */
    void glfw_window::show_window_and_run_events_loop()
    {
        int32_t x, y, w, h;
        if (!calculate_window_size_and_pos(x, y, w, h)) {
            x = y = 0;
            w = 400;
            h = 300;
        }
        glfwSetWindowPos(m_window, x, y);
        glfwSetWindowSize(m_window, w, h);
        glfwShowWindow(m_window);

        while (!glfwWindowShouldClose(m_window)) {
            glfwWaitEvents();
        }
    }

    /* glfw_window::track_events */
    void glfw_window::track_events()
    {
        glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(&m_glfw_event_callback));

        using t = green::utils::glfw_event;
        glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int x, int y) {
            send_event(construct_event(t::window_move, window, x, y));
        });
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            send_event(construct_event(t::window_resize, window, width, height));
        });
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
            send_event(construct_event(t::window_close, window));
        });
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            send_event(construct_event(t::framebuffer_resize, window, width, height));
        });
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            assert(action == GLFW_PRESS || action == GLFW_RELEASE);
            send_event(construct_event(action == GLFW_PRESS ? t::mouse_press : t::mouse_release, window, button, mods));
        });
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) {
            send_event(construct_event(t::mouse_move, window, static_cast<int32_t>(x), static_cast<int32_t>(y)));
        });
        glfwSetCursorEnterCallback(m_window, [](GLFWwindow* window, int entered) {
            send_event(construct_event(entered ? t::mouse_hower : t::mouse_left, window));
        });
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x, double y) {
            send_event(construct_event(t::scroll, window, x, y));
        });
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            assert(action == GLFW_PRESS || action == GLFW_RELEASE || action == GLFW_REPEAT);
            auto type = action == GLFW_PRESS ? t::key_press : (action == GLFW_RELEASE ? t::key_release : t::key_repeat);
            send_event(construct_event(type, window, key, scancode, mods));
        });
        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
            send_event(construct_event(t::input_codepoint, window, codepoint));
        });
        glfwSetDropCallback(m_window, [](GLFWwindow* window, int count, const char** paths) {
            send_event(construct_event(t::drop_files, window, paths, count));
        });
    }

    /* glfw_window::untrack_events */
    void glfw_window::untrack_events()
    {
        glfwSetWindowUserPointer(m_window, nullptr);
        glfwSetWindowPosCallback(m_window, nullptr);
        glfwSetWindowSizeCallback(m_window, nullptr);
        glfwSetWindowCloseCallback(m_window, nullptr);
        glfwSetFramebufferSizeCallback(m_window, nullptr);
        glfwSetMouseButtonCallback(m_window, nullptr);
        glfwSetCursorPosCallback(m_window, nullptr);
        glfwSetCursorEnterCallback(m_window, nullptr);
        glfwSetScrollCallback(m_window, nullptr);
        glfwSetKeyCallback(m_window, nullptr);
        glfwSetCharCallback(m_window, nullptr);
        glfwSetDropCallback(m_window, nullptr);
    }

} // green::utils
