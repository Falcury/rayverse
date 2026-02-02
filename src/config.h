#ifndef CONFIG_H
#define CONFIG_H

// Remove the 4-pixel-wide black bar on the left of the screen.
#define FIX_BLACK_BAR_ON_THE_LEFT

// Older graphics cards might only work if the texture size is a power of two.
// This could be relevant if you want to get Rayverse to work on Win9x on period-correct hardware.
// If set, will use power-of-two sized textures (only applies to Windows in OpenGL 1.1/non-ImGui enabled mode)
#define USE_POWER_OF_2_TEXTURES_ON_WINDOWS 0

// NOTE: To compile with the Dear ImGui debug layout enabled/disabled, pass -DWITH_IMGUI=1 or -DWITH_IMGUI=0 to CMake.
// This will add the ImGui-specific source files and also set the WITH_IMGUI macro to 1 or 0.


// Define compile-time guards to deal with differences across versions.
// These are currently purely for documentation purposes! However, they might help in merging the codebases in the future.
#define PC
//#define PS1

#endif //CONFIG_H
