#ifdef _WIN32
// Headers specific to Windows
#include "StdAfx.h"
#include <malloc.h>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
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


#ifdef _MSC_VER
#if _MSC_VER < 1900
#define snprintf _snprintf
#define for if(0); else for // prevent error C2374: multiple initialization in for loops
#endif
#endif

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#include "common.h"
#include "types.h"
#include "proto.h"

#include "data.cpp"

#include "intrinsics.cpp"
#include "mathutil.cpp"
#include "platform.cpp"

#ifdef _WIN32
// Source files specific to Windows
#include "win32_opengl.cpp"
#include "win32_sound.cpp"
#include "win32_main.cpp"
#else
// Source files specific to Linux / macOS
#include "linux_opengl.cpp"
#include "linux_sound.cpp"
#include "linux_main.cpp"
#endif

#include "render.cpp"
#include "misc_source.cpp"
#include "vignet.cpp"
#include "bray.cpp"
#include "sound.cpp"
#include "save.cpp"
#include "game.cpp"
#include "engine.cpp"
#include "object.cpp"
