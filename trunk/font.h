
#ifndef __FONT_H
#define __FONT_H

#include <string>
#include "sprite.h"
class Font {
	public:
		Font (std::string filename, int index, Sprite* spr);

		void Blit (char t, int x1, int y1) {
			pic->Blit(x1, y1, charx[(int)t], chary[(int)t], charw[(int)t], charh[(int)t]);
		}

		int getWidth (char t) { return charw[(int)t]; }
		int getHeight (char t) { return charh[(int)t]; }
	private:
		Sprite* pic;
		int charx[256], chary[256], charw[256], charh[256];
};

#endif
