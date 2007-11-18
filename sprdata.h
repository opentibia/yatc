/*////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
///////////////////////////////////////////////////////////////////// */

#ifndef __SPRDATA_H
#define __SPRDATA_H

#ifdef __cplusplus
extern "C" {
#endif

/*  
    Integer types are defined in SDL_config_win32.h, so including stdint.h causes redefinition errors.
*/
#if !defined(WIN32)
#include <stdint.h>
#endif
int readSprData(FILE* f, SDL_Surface *surface, int offx, int offy);
int writeSprData(FILE* f, SDL_Surface *surface, int offx, int offy, uint16_t *datasize);

#ifdef __cplusplus
}
#endif

#endif

