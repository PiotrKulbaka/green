set(GREEN_UTILS_SOURCES
    # glad
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/include/glad/glad.h
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/src/glad.c

    # glfw utils
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glfw/glfw_window.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glfw/glfw_window.cpp

    # opengl
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_buffer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_frame_surface_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_frame_surface_handler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_program.cpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_program.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_texture.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_vertex_array.cpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/opengl/opengl_vertex_array.hpp

    # utils
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/log.cpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/log.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/noncopyable.hpp
)

add_library(green_utils STATIC ${GREEN_UTILS_SOURCES})

target_link_libraries(green_utils glfw)

target_include_directories(green_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(green_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/include)
