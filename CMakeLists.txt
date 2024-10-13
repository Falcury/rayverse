cmake_minimum_required(VERSION 3.5)
project(rayverse)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
if (NOT MSVC)
    set(GCC_WARN_OPTIONS "-Wall -Wpedantic -Wno-unused-variable -Wno-unused-but-set-variable")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_WARN_OPTIONS}")
endif()

set(SOURCE_FILES
        src/rayverse.c
        src/stb_vorbis.c
)

if (WIN32)
    add_executable(rayverse ${SOURCE_FILES})
    target_link_libraries(rayverse user32 gdi32 winmm opengl32 glu32)
    # Hide the console window on release builds
    if (MSVC)
        target_link_options(rayverse PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/SUBSYSTEM:windows>" "$<$<NOT:$<CONFIG:Debug>>:/ENTRY:mainCRTStartup>")
    else()
        target_link_options(rayverse PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-mwindows>")
    endif()
elseif(APPLE)
    add_executable(rayverse ${SOURCE_FILES})
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(rayverse ${SDL2_LIBRARIES} "-framework OpenGL")
else()
    add_executable(rayverse ${SOURCE_FILES})
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(rayverse m GL GLEW SDL2)
endif()
