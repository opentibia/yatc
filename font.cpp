
#include "font.h"
#include "engine.h"

Font::Font (std::string fn, int index, Sprite *spr)
{
	// we do not create sprite here because the g_engine is not initialized yet
	// so we can't use g_engine->createSprite
	// at the same time we'll need filename and index (filename so we can add
	// support for TTF and friends at some period in time, and index so we can
	// figure out which PIC font we're loading, and set charxywh accordingly)

	pic = spr;

	// to start with something, we'll presume we're loading all from .pic file ...

	// we'll also hardcode char offsets
	switch (index) {
		case 2: // outline-less font
		case 7: // antialiased outlineless font
			charh[(unsigned char)'\n'] = 14;
			for (int i=0;i<128;i++) {
				if (i<32) {
					charx[i] = 0; chary[i] = 0; charw[i] = 0;
				} else {
					charx[i] = ((i-32) % 32) * 8;
					chary[i] = ((i-32) / 32) * 16;
					charw[i] = 8;
					charh[i] = 8;
				}
			}

			charw[(unsigned char)' '] = 4;
			charw[(unsigned char)'!'] = 3;
			charw[(unsigned char)':'] = 3;
			charw[(unsigned char)'\''] = 3; charh[(unsigned char)'\''] = 3;
			charw[(unsigned char)'_'] = 8; charh[(unsigned char)'_'] = 10;

			charw[(unsigned char)'A'] = 8;
			charw[(unsigned char)'I'] = 5;

			charw[(unsigned char)'c'] = 6;
			charw[(unsigned char)'e'] = 6;
			charw[(unsigned char)'f'] = 6;
			charw[(unsigned char)'g'] = 8; charh[(unsigned char)'g'] = 10;
			charw[(unsigned char)'i'] = 5;
			charw[(unsigned char)'l'] = 5;
			charw[(unsigned char)'o'] = 7;
			charw[(unsigned char)'m'] = 9;
			charw[(unsigned char)'n'] = 6;
			charw[(unsigned char)'p'] = 7; charh[(unsigned char)'p'] = 10;
			charw[(unsigned char)'t'] = 6;
			charw[(unsigned char)'y'] = 8; charh[(unsigned char)'y'] = 10;


			break;
		case 4: // outlined font
			charh[(unsigned char)'\n'] = 14;
			for (int i=0;i<128;i++) {
				if (i<32) {
					charx[i] = 0; chary[i] = 0; charw[i] = 0;
				} else {
					charx[i] = ((i-32) % 32) * 16;
					chary[i] = ((i-32) / 32) * 16;
					charw[i] += 2;
					charh[i] += 2;
				}
			}
			break;
		case 5: // minifont
			charh[(unsigned char)'\n'] = 8;
			for (int i=0;i<128;i++) {
				if (i<32) {
					charx[i] = 0; chary[i] = 0; charw[i] = 0;
				} else {
					charx[i] = ((i-32) % 32) * 8;
					chary[i] = ((i-32) / 32) * 8;
					charw[i] = 7;
					charh[i] = 7;
				}
			}

			charw[(unsigned char)' '] = 2; charh[(unsigned char)' '] = 1;
			charw[(unsigned char)'!'] = 3; charh[(unsigned char)'!'] = 6;
			charw[(unsigned char)'"'] = 4; charh[(unsigned char)'"'] = 3;
			charw[(unsigned char)'#'] = 7; charh[(unsigned char)'#'] = 6;

			charw[(unsigned char)'A'] = 6; charh[(unsigned char)'A'] = 6;
			charw[(unsigned char)'C'] = 6; charh[(unsigned char)'C'] = 6;
			charw[(unsigned char)'E'] = 5; charh[(unsigned char)'E'] = 6;
			charw[(unsigned char)'H'] = 6; charh[(unsigned char)'H'] = 6;
			charw[(unsigned char)'I'] = 4; charh[(unsigned char)'I'] = 6;
			charw[(unsigned char)'O'] = 7; charh[(unsigned char)'O'] = 6;

			charw[(unsigned char)'a'] = 5; charh[(unsigned char)'a'] = 6;
			charw[(unsigned char)'c'] = 5; charh[(unsigned char)'c'] = 6;
			charw[(unsigned char)'e'] = 5; charh[(unsigned char)'e'] = 6;
			charw[(unsigned char)'f'] = 4; charh[(unsigned char)'f'] = 6;
			charw[(unsigned char)'g'] = 5; charh[(unsigned char)'g'] = 8;
			charw[(unsigned char)'i'] = 3; charh[(unsigned char)'i'] = 6;
			charw[(unsigned char)'k'] = 5; charh[(unsigned char)'k'] = 6;
			charw[(unsigned char)'l'] = 3; charh[(unsigned char)'l'] = 6;
			charw[(unsigned char)'m'] = 8; charh[(unsigned char)'m'] = 6;
			charw[(unsigned char)'n'] = 5; charh[(unsigned char)'n'] = 6;
			charw[(unsigned char)'o'] = 5; charh[(unsigned char)'o'] = 6;
			charw[(unsigned char)'p'] = 6; charh[(unsigned char)'p'] = 8;
			charw[(unsigned char)'r'] = 4; charh[(unsigned char)'r'] = 6;
			charw[(unsigned char)'s'] = 5; charh[(unsigned char)'s'] = 6;
			charw[(unsigned char)'t'] = 4; charh[(unsigned char)'t'] = 6;
			charw[(unsigned char)'u'] = 5; charh[(unsigned char)'u'] = 6;
			charw[(unsigned char)'x'] = 5; charh[(unsigned char)'x'] = 6;

			break;
	}
}

