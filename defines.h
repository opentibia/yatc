//////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////

#ifndef __DEFINES_H
#define __DEFINES_H

#define PI 3.14159
#ifndef MIN
    #define MIN(a,b) (a > b ? b : a)
    #define MAX(a,b) (a < b ? b : a)
#endif

#if defined(_MSC_VER) && !defined(__PRETTY_FUNCTION__)
#define __PRETTY_FUNCTION__ "(msvc)"
#endif

#include <stdio.h>

// FIXME (ivucica#5#) Perhaps this one should move to "utils.h" or sth ... but it'd suck if it'd be there alone. So find some more functions to go there

class oRGBA
{
	public:
	oRGBA(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f){ //Should contain values from 0.0f->255.0f where 255.0f = max
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	float r, g, b, a;
};

inline oRGBA makeLightColor(uint16_t lightColor){
    // FIXME (nfries88): doesn't seem to work; at least for world light color...
    oRGBA color;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    color.r = ((float)(lightColor & 0xF000));// / 15.f) * 255.f;
    color.g = ((float)(lightColor & 0x0F00));// / 15.f) * 255.f;
    color.b = ((float)(lightColor & 0x00F0));// / 15.f) * 255.f;
    color.a = ((float)(lightColor & 0x000F));// / 15.f) * 255.f;
    #else
    color.r = ((float)(lightColor & 0x000F));// / 15.f) * 255.f;
    color.g = ((float)(lightColor & 0x00F0));// / 15.f) * 255.f;
    color.b = ((float)(lightColor & 0x0F00));// / 15.f) * 255.f;
    color.a = ((float)(lightColor & 0xF000));// / 15.f) * 255.f;
    #endif
    return color;
}

#endif
