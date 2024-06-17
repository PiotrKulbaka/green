#include <green/utils/opengl/opengl.hpp>

#include <green/utils/log.hpp>

namespace green::utils
{

    void opengl_check_errors(const char* file, int line)
    {
        static logger opengl_logger("OpenGL");

        GLenum ecode = 0;
        while ((ecode = glGetError()) != GL_NO_ERROR) {
            const char* e;
            switch (ecode) {
                case GL_INVALID_ENUM:
                    e = "INVALID_ENUM";
                    break;
                case GL_INVALID_VALUE:
                    e = "INVALID_VALUE";
                    break;
                case GL_INVALID_OPERATION:
                    e = "INVALID_OPERATION";
                    break;
                case GL_OUT_OF_MEMORY:
                    e = "OUT_OF_MEMORY";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    e = "INVALID_FRAMEBUFFER_OPERATION";
                    break;
                default:
                    e = "Unknown error";
                    break;
            }
            opengl_logger.error("%s(%d): %s", file, line, e);
        }
    }

}
