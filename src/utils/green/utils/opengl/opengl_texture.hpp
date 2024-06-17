#pragma once

#include <green/utils/noncopyable.hpp>
#include <cstdint>

namespace green::utils
{
    
    class opengl_texture
    {
    public:
        class builder : noncopyable
        {
        public:
            builder();
            ~builder();

            opengl_texture build();
        }; // class builder

    public:
        opengl_texture();
        ~opengl_texture();

        void initialize(uint32_t texture_id);
        void finalize();

        void set_texture_data(const void* data, int32_t stride_in_pixels, int32_t channels_number, int32_t width, int32_t height);

        void bind_and_active_texture_unit(uint32_t texture_unit) const noexcept;
        static void unbind() noexcept;

        int32_t get_width() const noexcept;
        int32_t get_height() const noexcept;

        uint32_t get_texture_id() const noexcept;

    private:
        uint32_t m_texture{0};
        int32_t m_width;
        int32_t m_height;
        int32_t m_texture_format;
    }; // class opengl_texture

} // namespace green::utils
