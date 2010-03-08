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

#ifndef __FONT_H
#define __FONT_H

#include <string>
#include <map>
#include "sprite.h"
typedef std::map<uint32_t, Sprite*> FontColorizedsMap;
class YATCFont{
	public:
		YATCFont(std::string filename, int index, Sprite* spr);
		virtual ~YATCFont();
		void Blit (char t, int x1, int y1) {
			m_currentcolorized->Blit(x1, y1, charx[(unsigned char)t], chary[(unsigned char)t], charw[(unsigned char)t], charh[(unsigned char)t]);
		}

        void analyzeFont(int hchars, int vchars, int charblockw, int charblockh);

		int getWidth(char t) { return charw[(unsigned char)t]; }
		int getHeight(char t) { return charh[(unsigned char)t]; }
		int getSpacing();
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
