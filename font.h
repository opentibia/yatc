
#ifndef __FONT_H
#define __FONT_H

#include <string>
#include <map>
#include "sprite.h"
typedef std::map<uint32_t, Sprite*> FontColorizedsMap;
class Font{
	public:
		Font(std::string filename, int index, Sprite* spr);
		virtual ~Font();
		void Blit (char t, int x1, int y1) {
			m_currentcolorized->Blit(x1, y1, charx[(unsigned char)t], chary[(unsigned char)t], charw[(unsigned char)t], charh[(unsigned char)t]);
		}

		int getWidth(char t) { return charw[(unsigned char)t]; }
		int getHeight(char t) { return charh[(unsigned char)t]; }
		void addColor(float r, float g, float b);
		void resetColor();
	private:
        std::string m_filename;
        int m_index;
		Sprite* pic;
		int charx[256], chary[256], charw[256], charh[256];

		FontColorizedsMap m_colorized;
		uint32_t m_currentcolor;
		Sprite* m_currentcolorized;
};

#endif
