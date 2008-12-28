/* pictool: ANSI C converter for Tibia's PIC files
 * (c) 2007-2008 Ivan Vucica
 * Part of OpenTibia project
 *
 * Although written in ANSI C, this makes use of #pragma pack(),
 * make sure your compiler supports packed structures, or else.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef PICTOOL_PICFUNCS_H
#define PICTOOL_PICFUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

/* If your compiler does not support uint32_t and friends, then
 * change #if (0) into #if (1) and we'll try defining them.
 * Note that most modern compilers should support them; GCC
 * supports them even in ANSI mode, but then, GCC is not the
 * only compiler out there. */
#if (0)
#define uint32_t unsigned long
#define uint16_t unsigned short
#define uint8_t unsigned char
#endif

/* Below, 1 means "debugging output on", and 0 means "debugging output off". */
#if (0)
#define dbgprintf printf
#else
#define dbgprintf
#endif

#include <SDL/SDL.h>
#include <stdio.h>
int writesprite (FILE *f, SDL_Surface *s, int offx, int offy, uint16_t *datasize);
int readsprite (FILE *f, uint32_t sprloc, SDL_Surface *s, int offx, int offy);
int picdetails (const char* filename);
extern int (*pictool_dumperror)(char*,...);
int writepic(const char* filename, int index, SDL_Surface *s);
int readpic (const char* filename, int index, SDL_Surface **sr);

#ifdef __cplusplus
}
#endif

#endif
