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
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#include "common.h"
#include "types.h"
#include "proto.h"

#include "data.c"

#include "intrinsics.c"
#include "mathutil.c"
#include "sysutils.c"

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

// Misc source files, need cleanup
#include "engine.c"
#include "render.c"
#include "misc_source.c"

// Source files in (mostly) alphabetical order, corresponding to a possible ordering of the PC version's sources
#include "lowlevel.c"
#include "pcmain.c"
#include "action.c"
#include "affiche.c"
#include "araignee.c"
#include "audiocd.c"
#include "basic.c"
#include "bat.c"
#include "bbmont.c"
#include "blacktoon.c"
#include "blood.c"
#include "bonus.c"
#include "box.c"
#include "blocs.c"
#include "breakout.c"
#include "cage.c"
#include "clown.c"
#include "collision.c"
#include "compress.c"
#include "cymbal.c"
#include "dark.c"
#include "display.c"
#include "effect.c"
#include "fee.c"
#include "frame.c"
#include "guetteur.c"
#include "hifi.c"
#include "hybrid.c"
#include "input.c"
#include "jumelle.c"
#include "load.c"
#include "luciole.c"
#include "menu.c"
#include "meredenis.c"
#include "mite.c"
#include "moskito.c"
#include "moteur.c"
#include "neige.c"
#include "map.c"
#include "ngawe.c"
#include "objinit.c"
#include "objupdate.c"
#include "pmama.c"
#include "poing.c"
#include "poisson.c"
#include "worldmap.c"
#include "rand.c"
#include "ray.c"
#include "sound.c"
#include "saxo.c"
#include "save.c"
#include "skops.c"
#include "special.c"
#include "stoneman.c"
#include "trumpet.c"
#include "ufo.c"
