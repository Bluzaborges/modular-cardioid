cmake_minimum_required(VERSION 3.21)

project(modular-cardioid VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

find_package(SDL2 CONFIG REQUIRED)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.9
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC SDL2::SDL2)

set(MODULAR_CARDIOID_SOURCES
    main.cpp
    include/App.h
    include/Pattern.h
    src/App.cpp
    src/Pattern.cpp
)

add_executable(modular-cardioid ${MODULAR_CARDIOID_SOURCES})

if(WIN32)
    set_target_properties(modular-cardioid PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES ${MODULAR_CARDIOID_SOURCES}
)

target_compile_features(modular-cardioid PRIVATE cxx_std_17)
target_include_directories(modular-cardioid PRIVATE include)
target_link_libraries(modular-cardioid PRIVATE SDL2::SDL2 imgui)

if(MSVC)
    target_compile_options(modular-cardioid PRIVATE /W4)
else()
    target_compile_options(modular-cardioid PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    add_custom_command(
        TARGET modular-cardioid
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:modular-cardioid>
        VERBATIM
    )
endif()
