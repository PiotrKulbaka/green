#pragma once

#include <green/utils/glfw/glfw_window.hpp>
#include "input_manager.hpp"

namespace green
{
    class global_manager
    {
    public:
        global_manager()
            : main_window("Green test application")
            , input()
            , run(false)
            , render_width(0)
            , render_height(0)
        {}

        ~global_manager() = default;

    public:
        green::utils::glfw_window main_window;
        green::input_manager input;
        bool run;
        int32_t render_width;
        int32_t render_height;
    };

} // namespace green
