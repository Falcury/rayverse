cmake_minimum_required(VERSION 2.8)
project(rayverse)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(GCC_WARN_OPTIONS "-Wall -Wpedantic -Wno-unused-variable -Wno-unused-but-set-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_WARN_OPTIONS} -Wno-class-memaccess")

set(SOURCE_FILES
        src/rayverse.cpp
        src/stb_vorbis.c
)

if (WIN32)
    add_executable(rayverse ${SOURCE_FILES})
    target_link_libraries(rayverse user32 gdi32 winmm opengl32 glu32)
    # Hide the console window on release builds
    target_link_options(rayverse PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-mwindows>")
endif()
