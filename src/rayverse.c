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

#include "data.c"

#include "intrinsics.c"
#include "mathutil.c"
#include "platform.c"

#ifdef _WIN32
// Source files specific to Windows
#include "win32_opengl.c"
#include "win32_sound.c"
#include "win32_main.c"
#else
// Source files specific to Linux / macOS
#include "linux_opengl.c"
#include "linux_sound.c"
#include "linux_main.c"
#endif

#include "render.c"
#include "misc_source.c"
#include "vignet.c"
#include "bray.c"
#include "sound.c"
#include "video.c"
#include "save.c"
#include "world_map.c"
#include "game.c"
#include "engine.c"
#include "object.c"
#include "ray.c"
#include "moskito.c"
#include "saxo.c"
#include "space_mama.c"

