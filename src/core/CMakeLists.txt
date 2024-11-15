set(GREEN_CORE_SOURCES
    # math
    ${CMAKE_CURRENT_LIST_DIR}/green/core/core.cpp
    ${CMAKE_CURRENT_LIST_DIR}/green/core/math_base.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/core/vec2.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/core/vec3.hpp
    ${CMAKE_CURRENT_LIST_DIR}/green/core/vec4.hpp
)

add_library(green_core STATIC ${GREEN_CORE_SOURCES})

target_link_libraries(green_core glm)

target_include_directories(green_core PUBLIC ${CMAKE_CURRENT_LIST_DIR})
