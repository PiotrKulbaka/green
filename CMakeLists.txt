message("CMake version: ${CMAKE_VERSION}")

cmake_minimum_required(VERSION 3.13)

project(green_project C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src)

set(APP_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/res/app.icns)

# main
set(GREEN_APP_EXECUTABLE
    ${CMAKE_CURRENT_LIST_DIR}/main/main.cpp
)

add_executable(green_app ${GREEN_APP_EXECUTABLE} ${APP_RESOURCES})

target_link_libraries(green_app
    green_utils
    stb
    glm
)

target_include_directories(green_app PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

target_compile_definitions(green_app PUBLIC -DGREEN_PATH_TO_RES_DIRECTORY="${CMAKE_CURRENT_LIST_DIR}/res")
target_compile_definitions(green_app PUBLIC -DGREEN_PATH_TO_OUT_DIRECTORY="${CMAKE_CURRENT_LIST_DIR}/out")

if(APPLE)
    set(APP_BUNDLE_NAME "Green app.") # This name can contain up to 15 characters according to Apple's documentation
    set(APP_BUNDLE_VERSION 1.0.0)
    set(APP_COPYRIGHT "Green application.")
    set(APP_INFO "Green test application.")

    set_target_properties(green_app PROPERTIES
        OUTPUT_NAME ${APP_BUNDLE_NAME} # Name of the executable and the Bundle
        RESOURCE ${APP_RESOURCES}
        MACOSX_BUNDLE TRUE

        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/res/Info.plist"

        MACOSX_BUNDLE_BUNDLE_NAME           ${APP_BUNDLE_NAME}      # Sets CFBundleName
        MACOSX_BUNDLE_BUNDLE_VERSION        ${APP_BUNDLE_VERSION}   # Sets CFBundleVersion
        MACOSX_BUNDLE_COPYRIGHT             ${APP_COPYRIGHT}        # Sets NSHumanReadableCopyright
        MACOSX_BUNDLE_GUI_IDENTIFIER        "com.green.app"         # Sets CFBundleIdentifier
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.green.app"   # Suppress warning that CFBundleIdentifier and PRODUCT_BUNDLE_IDENTIFIER must be the same

        MACOSX_BUNDLE_ICON_FILE             app.icns                # Sets CFBundleIconFile
        MACOSX_BUNDLE_INFO_STRING           ${APP_INFO}             # Sets CFBundleGetInfoString
        MACOSX_BUNDLE_LONG_VERSION_STRING   ${APP_BUNDLE_VERSION}   # Sets CFBundleLongVersionString
        MACOSX_BUNDLE_SHORT_VERSION_STRING  ${APP_BUNDLE_VERSION}   # Sets CFBundleShortVersionString
    )
endif()
