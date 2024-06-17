#include <green/utils/opengl/opengl_texture.hpp>

#include <green/utils/opengl/opengl.hpp>
#include <green/utils/log.hpp>
#include <cassert>

namespace
{
    green::utils::logger log("opengl_texture");
} // namespace

namespace green::utils
{

    /* builder::builder */
    opengl_texture::builder::builder() = default;

    /* builder::~builder */
    opengl_texture::builder::~builder() = default;

    /* builder::build */
    opengl_texture opengl_texture::builder::build()
    {
        uint32_t texture_id;
        GL_CALL(glGenTextures(1, &texture_id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_id));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

        opengl_texture texture;
        texture.initialize(texture_id);
        return texture;
    }



    /* opengl_texture::opengl_texture */
    opengl_texture::opengl_texture() = default;

    /* opengl_texture::~opengl_texture */
    opengl_texture::~opengl_texture() = default;

    /* opengl_texture::initialize */
    void opengl_texture::initialize(uint32_t texture_id)
    {
        if (m_texture) {
            log.error("Attempt to initialize an already initialized texture.");
        }
        m_texture = texture_id;
    }

    /* opengl_texture::finalize */
    void opengl_texture::finalize()
    {
        GL_CALL(glDeleteTextures(1, &m_texture));
        m_texture = 0;
    }

    /* opengl_texture::set_texture_data */
    void opengl_texture::set_texture_data(const void* data, int32_t stride_in_pixels, int32_t channels_number, int32_t width, int32_t height)
    {
        assert(channels_number >= 1 && channels_number <= 4);
        static const int32_t texture_formats[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
        auto texture_format = texture_formats[channels_number - 1];

        if (!m_texture) {
            log.error("Attempt to set data to an empty texture.");
        }

        GL_CALL(glBindTexture(GL_TEXTURE_2D, m_texture));
        GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, stride_in_pixels));
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0, texture_format, GL_UNSIGNED_BYTE, data));
        GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
        unbind();
    }

    /* opengl_texture::bind_and_active_texture_unit */
    void opengl_texture::bind_and_active_texture_unit(uint32_t texture_unit) const noexcept
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + texture_unit));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, m_texture));
        if (!m_texture) {
            log.error("Attempt to use an empty texture.");
        }
    }

    /* opengl_texture::unbind */
    void opengl_texture::unbind() noexcept
    {
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    /* opengl_texture::get_width */
    int32_t opengl_texture::get_width() const noexcept
    {
        return m_width;
    }

    /* opengl_texture::get_height */
    int32_t opengl_texture::get_height() const noexcept
    {
        return m_height;
    }

    /* opengl_texture::get_texture_id */
    uint32_t opengl_texture::get_texture_id() const noexcept
    {
        return m_texture;
    }

} // namespace green::utils
