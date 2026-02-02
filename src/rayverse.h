#ifndef RAYVERSE_H
#define RAYVERSE_H

// Configuration macros for Rayverse
#include "config.h"

#ifndef WITH_IMGUI
#define WITH_IMGUI 0
#endif

// Some workarounds to deal with MSVC-specific stuff
#ifdef _MSC_VER
#if _MSC_VER < 1900
#define snprintf _snprintf
#define for if(0); else for // prevent error C2374: multiple initialization in for loops
#endif
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#ifdef _WIN32
// Headers specific to Windows
#include <malloc.h>
#include <windows.h>
#include <mmsystem.h> // needed for dsound.h
#include <dsound.h>   // needed for IDirectSound and IDirectSoundBuffer in types.h
#include <GL/gl.h>
#include <GL/glu.h>
#else
// Headers specific to Linux / macOS
#include <SDL.h>
#include <SDL_syswm.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION // silence OpenGL deprecation warning on macOS
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c" // needed for ogg_t in types.h

#include "common.h" // basic macros and types
#include "types.h"  // Rayverse type definitions
#include "proto.h"  // Rayverse function prototypes
#include "data.c"   // Rayverse global variables

#endif //RAYVERSE_H
