
#include "font.h"
#include "engine.h"

Font::Font (std::string fn, int index, Sprite *spr) {


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
			charh['\n'] = 14;
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

			charw[' '] = 4;
			charw['!'] = 3;
			charw[':'] = 3;
			charw['\''] = 3; charh['\''] = 3;
			charw['_'] = 8; charh['_'] = 10;

			charw['A'] = 8;
			charw['I'] = 5;

			charw['c'] = 6;
			charw['e'] = 6;
			charw['f'] = 6;
			charw['g'] = 8; charh['g'] = 10;
			charw['i'] = 5;
			charw['l'] = 5;
			charw['o'] = 7;
			charw['m'] = 9;
			charw['n'] = 6;
			charw['p'] = 7; charh['p'] = 10;
			charw['t'] = 6;
			charw['y'] = 8; charh['y'] = 10;


			break;
		case 4: // outlined font
			charh['\n'] = 14;
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
			charh['\n'] = 8;
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

			charw[' '] = 2; charh[' '] = 1;
			charw['!'] = 3; charh['!'] = 6;
			charw['"'] = 4; charh['"'] = 3;
			charw['#'] = 7; charh['#'] = 6;

			charw['A'] = 6; charh['A'] = 6;
			charw['C'] = 6; charh['C'] = 6;
			charw['E'] = 5; charh['E'] = 6;
			charw['H'] = 6; charh['H'] = 6;
			charw['I'] = 4; charh['I'] = 6;
			charw['O'] = 7; charh['O'] = 6;

			charw['a'] = 5; charh['a'] = 6;
			charw['c'] = 5; charh['c'] = 6;
			charw['e'] = 5; charh['e'] = 7;
			charw['f'] = 4; charh['f'] = 6;
			charw['g'] = 5; charh['g'] = 8;
			charw['i'] = 3; charh['i'] = 6;
			charw['k'] = 5; charh['k'] = 6;
			charw['l'] = 3; charh['l'] = 6;
			charw['m'] = 8; charh['m'] = 6;
			charw['n'] = 5; charh['n'] = 6;
			charw['o'] = 5; charh['o'] = 6;
			charw['p'] = 6; charh['p'] = 8;
			charw['r'] = 4; charh['r'] = 6;
			charw['s'] = 5; charh['s'] = 6;
			charw['t'] = 4; charh['t'] = 6;
			charw['u'] = 5; charh['u'] = 6;
			charw['x'] = 5; charh['x'] = 6;



			break;
	}
}

