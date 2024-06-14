set(GREEN_UTILS_SOURCES
    # glad
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/include/glad/glad.h
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/src/glad.c

    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glfw/glfw_window.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/utils/glfw/glfw_window.cpp
)

add_library(green_utils STATIC ${GREEN_UTILS_SOURCES})

target_link_libraries(green_utils glfw)

target_include_directories(green_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(green_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR}/green/utils/glad/include)
