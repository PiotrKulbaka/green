# GLFW settings
set(BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)

add_subdirectory(glfw)

# GLM settings
set(GLM_ENABLE_CXX_20 ON CACHE BOOL " " FORCE)
set(GLM_BUILD_LIBRARY ON CACHE BOOL " " FORCE)

add_subdirectory(glm)

add_library(stb STATIC
    ${CMAKE_CURRENT_LIST_DIR}/stb/stb.cpp
    ${CMAKE_CURRENT_LIST_DIR}/stb/include/stb/stb.hpp
)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/stb/include)
