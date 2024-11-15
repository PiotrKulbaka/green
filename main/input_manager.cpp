#include "input_manager.hpp"

namespace green
{

    input_manager::input_manager()
        : m_keyboard_state{false}
    {
    }

    input_manager::~input_manager() = default;

    void input_manager::on_press(key_t key) noexcept
    {
        m_keyboard_state[key] = true;
    }

    void input_manager::on_release(key_t key) noexcept
    {
        m_keyboard_state[key] = false;
    }

} // namespace green
