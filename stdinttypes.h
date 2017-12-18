#ifndef __STDINTTYPES_H
#define __STDINTTYPES_H

#if !defined(_MSC_VER) || _MSC_VER >= 1911 // Visual Studio 2017 15.3.0's MSVC++ 14.1
#include <stdint.h>
#else
#include <SDL/SDL.h>
//_SDL_config_win32_h already included which containts sddint defs
#endif


#endif
