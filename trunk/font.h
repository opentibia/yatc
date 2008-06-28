
#ifndef __FONT_H
#define __FONT_H

#include <string>
#include "sprite.h"
class Font{
	public:
		Font(std::string filename, int index, Sprite* spr);
		virtual ~Font() { delete pic; }
		void Blit (char t, int x1, int y1) {
			pic->Blit(x1, y1, charx[(unsigned char)t], chary[(unsigned char)t], charw[(unsigned char)t], charh[(unsigned char)t]);
		}

		int getWidth(char t) { return charw[(unsigned char)t]; }
		int getHeight(char t) { return charh[(unsigned char)t]; }
		void addColor(float r, float g, float b);
		void resetColor();
	private:
		Sprite* pic;
		int charx[256], chary[256], charw[256], charh[256];
};

#endif
