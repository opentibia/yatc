//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Font class
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

#include "font.h"
#include "engine.h"

Font::Font(std::string fn, int index, Sprite *spr)
{
	// we do not create sprite here because the g_engine is not initialized yet
	// so we can't use g_engine->createSprite
	// at the same time we'll need filename and index (filename so we can add
	// support for TTF and friends at some period in time, and index so we can
	// figure out which PIC font we're loading, and set charxywh accordingly)


	pic = spr;

	// to start with something, we'll presume we're loading all from .pic file ...

	// we'll also hardcode char offsets
	switch(index){
		case 2: // outline-less font
		case 4: // outlined font
		case 7: // antialiased outlineless font
			for(int i = 0; i < 255; i++){
				if(i >= 32 && i < 170){
					charx[i] = ((i-32) % 32) * 8;
					chary[i] = ((i-32) / 32) * 16;
					charw[i] = 8;
					charh[i] = 8;
				}
				else{
					charx[i] = 0; chary[i] = 0; charw[i] = 0; charh[i] = 0;
				}
			}

			charh[(unsigned char)'\n'] = 14;

			charw[(unsigned char)' '] = 2;
			charw[(unsigned char)'.'] = 2;
			charw[(unsigned char)','] = 2; charh[(unsigned char)','] = 10;
			charw[(unsigned char)'!'] = 2;
			charw[(unsigned char)':'] = 2;
			charw[(unsigned char)';'] = 2; charh[(unsigned char)';'] = 10;
			charw[(unsigned char)'\''] = 2; charh[(unsigned char)'\''] = 3;
			charw[(unsigned char)'_'] = 7; charh[(unsigned char)'_'] = 10;
			charw[(unsigned char)'('] = 4; charh[(unsigned char)'('] = 10;
			charw[(unsigned char)')'] = 4; charh[(unsigned char)')'] = 10;

			charw[(unsigned char)'0'] = 6;
			charw[(unsigned char)'1'] = 4;
			charw[(unsigned char)'2'] = 6;


			charw[(unsigned char)'A'] = 7;
			charw[(unsigned char)'B'] = 6;
			charw[(unsigned char)'C'] = 6;
			charw[(unsigned char)'E'] = 6;
			charw[(unsigned char)'F'] = 6;
			charw[(unsigned char)'H'] = 7;
			charw[(unsigned char)'I'] = 4;
			charw[(unsigned char)'J'] = 5;
			charw[(unsigned char)'K'] = 6;
			charw[(unsigned char)'L'] = 6;
			charw[(unsigned char)'N'] = 7;
			charw[(unsigned char)'P'] = 6;
			charw[(unsigned char)'R'] = 6;
			charw[(unsigned char)'Q'] = 7; charh[(unsigned char)'Q'] = 10;
			charw[(unsigned char)'S'] = 6;
			charw[(unsigned char)'V'] = 6;
			charw[(unsigned char)'X'] = 6;
			charw[(unsigned char)'Y'] = 6;
            charw[(unsigned char)'Z'] = 6;

			charw[(unsigned char)'a'] = 6;
			charw[(unsigned char)'b'] = 6;
			charw[(unsigned char)'c'] = 5;
			charw[(unsigned char)'d'] = 6;
			charw[(unsigned char)'e'] = 6;
			charw[(unsigned char)'f'] = 5;
			charw[(unsigned char)'g'] = 6; charh[(unsigned char)'g'] = 10;
			charw[(unsigned char)'h'] = 6;
			charw[(unsigned char)'i'] = 2;
			charw[(unsigned char)'j'] = 4; charh[(unsigned char)'j'] = 10;
			charw[(unsigned char)'k'] = 6;
			charw[(unsigned char)'l'] = 3;
			charw[(unsigned char)'o'] = 6;
			charw[(unsigned char)'m'] = 8;
			charw[(unsigned char)'n'] = 6;
			charw[(unsigned char)'p'] = 6; charh[(unsigned char)'p'] = 10;
			charw[(unsigned char)'r'] = 5;
			charw[(unsigned char)'s'] = 5;
			charw[(unsigned char)'t'] = 5;
			charw[(unsigned char)'u'] = 6;
			charw[(unsigned char)'v'] = 7;
			charw[(unsigned char)'x'] = 6;
			charw[(unsigned char)'y'] = 6; charh[(unsigned char)'y'] = 10;
			charw[(unsigned char)'q'] = 6; charh[(unsigned char)'q'] = 10;
			charw[(unsigned char)'z'] = 5;


			charw[(unsigned char)169] = 8; charh[(unsigned char)169] = 10;  // copyright symbol

			if (index == 4) { // outlined font
				for(int i = 0;i < 255; i++){
					if(i >= 32 && i < 170){
						charx[i] = ((i-32) % 32) * 16;
						chary[i] = ((i-32) / 32) * 16;
						charw[i] += 1;
						charh[i] += 2;
					}
					else{
						charx[i] = 0; chary[i] = 0; charw[i] = 0; charh[i] = 0;
					}
				}
				charh[(unsigned char)'\n'] = 14;
			}
			break;

		case 5: // minifont
			charh[(unsigned char)'\n'] = 8;
			for(int i = 0;i < 255;i++){
				if(i >= 32 && i < 170){
					charx[i] = ((i-32) % 32) * 8;
					chary[i] = ((i-32) / 32) * 8;
					charw[i] = 7;
					charh[i] = 7;
				}
				else{
					charx[i] = 0; chary[i] = 0; charw[i] = 0;
				}
			}

			charw[(unsigned char)' '] = 1; charh[(unsigned char)' '] = 1;
			charw[(unsigned char)'!'] = 2; charh[(unsigned char)'!'] = 6;
			charw[(unsigned char)'"'] = 3; charh[(unsigned char)'"'] = 3;
			charw[(unsigned char)'#'] = 6; charh[(unsigned char)'#'] = 6;

			charw[(unsigned char)'1'] = 3; charh[(unsigned char)'1'] = 6;

			charw[(unsigned char)'A'] = 6; charh[(unsigned char)'A'] = 6;
			charw[(unsigned char)'C'] = 5; charh[(unsigned char)'C'] = 6;
			charw[(unsigned char)'E'] = 5; charh[(unsigned char)'E'] = 6;
			charw[(unsigned char)'G'] = 6; charh[(unsigned char)'G'] = 6;
			charw[(unsigned char)'H'] = 6; charh[(unsigned char)'H'] = 6;
			charw[(unsigned char)'I'] = 4; charh[(unsigned char)'I'] = 6;
			charw[(unsigned char)'O'] = 6; charh[(unsigned char)'O'] = 6;
			charw[(unsigned char)'S'] = 5; charh[(unsigned char)'S'] = 6;

			charw[(unsigned char)'a'] = 5; charh[(unsigned char)'a'] = 6;
			charw[(unsigned char)'c'] = 4; charh[(unsigned char)'c'] = 6;
			charw[(unsigned char)'e'] = 5; charh[(unsigned char)'e'] = 7;
			charw[(unsigned char)'f'] = 3; charh[(unsigned char)'f'] = 6;
			charw[(unsigned char)'g'] = 4; charh[(unsigned char)'g'] = 8;
			charw[(unsigned char)'h'] = 5; charh[(unsigned char)'h'] = 6;
			charw[(unsigned char)'i'] = 2; charh[(unsigned char)'i'] = 6;
			charw[(unsigned char)'j'] = 4; charh[(unsigned char)'j'] = 8;
			charw[(unsigned char)'k'] = 5; charh[(unsigned char)'k'] = 6;
			charw[(unsigned char)'l'] = 2; charh[(unsigned char)'l'] = 6;
			charw[(unsigned char)'m'] = 8; charh[(unsigned char)'m'] = 6;
			charw[(unsigned char)'n'] = 5; charh[(unsigned char)'n'] = 6;
			charw[(unsigned char)'o'] = 5; charh[(unsigned char)'o'] = 6;
			charw[(unsigned char)'p'] = 5; charh[(unsigned char)'p'] = 8;
			charw[(unsigned char)'r'] = 4; charh[(unsigned char)'r'] = 6;
			charw[(unsigned char)'s'] = 4; charh[(unsigned char)'s'] = 6;
			charw[(unsigned char)'t'] = 3; charh[(unsigned char)'t'] = 6;
			charw[(unsigned char)'u'] = 5; charh[(unsigned char)'u'] = 6;
			charw[(unsigned char)'x'] = 5; charh[(unsigned char)'x'] = 6;
			charw[(unsigned char)'y'] = 5; charh[(unsigned char)'y'] = 8;

			break;

		default:
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[Font::Font] index = %d.\n", index);
			break;
	}
}
