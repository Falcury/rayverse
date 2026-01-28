cmake_minimum_required(VERSION 3.5)
project(rayverse)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
if (NOT MSVC)
    set(GCC_WARN_OPTIONS "-Wall -Wpedantic -Wno-unused-variable -Wno-unused-but-set-variable")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_WARN_OPTIONS}")
endif()

add_executable(rayverse
        src/rayverse.c
        src/stb_vorbis.c
)

# NOTE: To suppress building with Dear ImGui, pass -DWITH_IMGUI=0 to CMake (or change the value below to 0)
if (NOT DEFINED WITH_IMGUI)
    set(WITH_IMGUI 1)
endif()
if (WITH_IMGUI)
    target_compile_definitions(rayverse PRIVATE
            WITH_IMGUI=1
    )
    target_include_directories(rayverse PRIVATE
            src/imgui
            src/imgui/backends
    )
    target_sources(rayverse PRIVATE
            src/rayverse_imgui.cpp
            src/imgui/imgui.cpp
            src/imgui/imgui_demo.cpp
            src/imgui/imgui_draw.cpp
            src/imgui/imgui_tables.cpp
            src/imgui/imgui_widgets.cpp
            src/imgui/backends/imgui_impl_win32.cpp
            src/imgui/backends/imgui_impl_opengl3.cpp
    )
endif()

if (WIN32)
    target_link_libraries(rayverse user32 gdi32 winmm opengl32 glu32)
    if (WITH_IMGUI)
        target_link_libraries(rayverse dwmapi)
    endif()
    # Hide the console window on release builds
    if (MSVC)
        target_link_options(rayverse PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/SUBSYSTEM:windows>" "$<$<NOT:$<CONFIG:Debug>>:/ENTRY:mainCRTStartup>")
    else()
        target_link_options(rayverse PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-mwindows>")
    endif()
elseif(APPLE)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(rayverse ${SDL2_LIBRARIES} "-framework OpenGL")
else()
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(rayverse m GL GLEW SDL2)
endif()
