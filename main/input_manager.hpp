#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace green
{
    class input_manager
    {
    public:
        using key_t = uint32_t;

    public:
        input_manager();
        ~input_manager();

        bool operator[](key_t key) const noexcept
        {
            return m_keyboard_state[key];
        }

        void on_press(key_t key) noexcept;
        void on_release(key_t key) noexcept;
    private:
        bool m_keyboard_state[GLFW_KEY_LAST];
    }; // class input_manager

} // namespace green
