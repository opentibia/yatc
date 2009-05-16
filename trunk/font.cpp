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

YATCFont::YATCFont(std::string fn, int index, Sprite *spr)
{
	// we do not create sprite here because the g_engine is not initialized yet
	// so we can't use g_engine->createSprite
	// at the same time we'll need filename and index (filename so we can add
	// support for TTF and friends at some period in time, and index so we can
	// figure out which PIC font we're loading, and set charxywh accordingly)


	pic = spr;
	m_filename = fn;
	m_index = index;
	m_currentcolorized = pic;
	m_currentcolor = 0x00FFFFFF;

	// to start with something, we'll presume we're loading all from .pic file ...

	// we'll also hardcode char offsets
	switch(index){
        case 2: // non-outlined font
        case 7: // antialiased non-outlined font
        {
            int hchars = 256 / 8;               /* horizontal character count -- chars per row */
            int vchars = spr->getHeight() / 16; /* vertical character count -- chars per column */
            int charblockw = 8;
            int charblockh = 16;

            memset(charx, 0, sizeof(*charx)*256);
            memset(chary, 0, sizeof(*chary)*256);
            memset(charw, 0, sizeof(*charw)*256);
            memset(charh, 0, sizeof(*charh)*256);


            analyzeFont(hchars,vchars,charblockw,charblockh);
            break;
        }

		case 4: // outlinefont
        {
            int hchars = 512 / 16;                  /* horizontal character count -- chars per row */
            int vchars = spr->getHeight() / 16;     /* vertical character count -- chars per column */
            int charblockw = 16;
            int charblockh = 16;

            memset(charx, 0, sizeof(*charx)*256);
            memset(chary, 0, sizeof(*chary)*256);
            memset(charw, 0, sizeof(*charw)*256);
            memset(charh, 0, sizeof(*charh)*256);


            analyzeFont(hchars,vchars,charblockw,charblockh);
            break;
        }

		case 5: // minifont
        {
            int hchars = 256 / 8;                  /* horizontal character count -- chars per row */
            int vchars = spr->getHeight() / 8;     /* vertical character count -- chars per column */
            int charblockw = 8;
            int charblockh = 8;

            memset(charx, 0, sizeof(*charx)*256);
            memset(chary, 0, sizeof(*chary)*256);
            memset(charw, 0, sizeof(*charw)*256);
            memset(charh, 0, sizeof(*charh)*256);


            analyzeFont(hchars,vchars,charblockw,charblockh);
            break;
        }

		default:
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[YATCFont::YATCFont] index = %d.\n", index);
			break;
	}




}
YATCFont::~YATCFont() {
    delete pic;
    for(FontColorizedsMap::iterator it = m_colorized.begin(); it != m_colorized.end(); it++) {
        delete it->second;
    }
}


void YATCFont::analyzeFont(int hchars, int vchars, int charblockw, int charblockh)
{
    // calculates char widths and heights
    SDL_Surface * sfc = pic->lockSurface();

    for (int j = 0; j < vchars; j++) // 16 == height of individual char
    {
        for (int i = 0; i < hchars; i++)
        {
            int c = 32 + (j * hchars) + i;

            if (c > 255)
                continue;
            charx[c] = i*charblockw;
            chary[c] = j*charblockh;


            // lets check where within certain char (backward looking)
            // there's last pixel appearing
            int width=0;
            int height=0;

			// FIXME (nfries88): This doesn't seem to work properly for all characters for "gamefont"
			//	"w" and "s" are the ones I've noticed so far.
            for (int k = i*charblockw + 7; k >= i*charblockw; k--)
            {

                // is in this column there a pixel present?
                for (int l = j * charblockh; l < (j+1) * charblockh; l++)
                {
                    uint8_t r,g,b,a;
                    int pixel = pic->getPixel(k,l);

                    SDL_GetRGBA(pixel, sfc->format, &r, &g, &b, &a);
                    if (a && !width)
                        width=k-i*charblockw + 1;
                    if (a && l-j*charblockh + 1 > height)
                            height=l-j*charblockh + 1;


                }

            }

            if (width>1)
                charw[c] = width;
            else
                charw[c] = 3; // let it be space char w

            if (height)
                charh[c] = height;
            else
                charh[c] = 8; // let it be height of "regular" char
        }
    }

    charw[32] = 3; // space size
    charh['\n'] = 12; // height of a line, including spacing between lines

    pic->unlockSurface();



}

void YATCFont::addColor(float r, float g, float b)
{
    // THIS NEEDS TO BE CORRECTED!
    // This is only needed under SDL engine.
    // Under OpenGL engine we should do the old method (just plain pic->addColor())
    // Under GL it creates unneeded duplicate textures which serve no useful purpose.
    // FIXME (ivucica#1#): implement suggestions stated above
    m_currentcolor = (int(r*255) << 16 | int(g*255) << 8 | int(b*255));

    if (m_currentcolor == 0x00FFFFFF) { // if we try to fetch white, this is the original color, so fall back to it and do nothing more
        m_currentcolorized = pic;
        return;
    }

    FontColorizedsMap::iterator it;
    if ((it = m_colorized.find(m_currentcolor)) != m_colorized.end()) { // if we found it cached, go on
        m_currentcolorized = it->second;
        return;
    }
    if (g_engine) {
        Sprite *s = g_engine->createSprite(m_filename, m_index);

        s->addColor(r,g,b);

        m_colorized[m_currentcolor] = s;
        m_currentcolorized = m_colorized[m_currentcolor];
    }
}

void YATCFont::resetColor()
{
	addColor(1,1,1); // setting it to white effectively resets the color
}

int YATCFont::getSpacing() {
// spacing between characters
// minifont has none
// outlinefont has negative
	if (m_index==5) return 0;
	if (m_index==4) return -1;
	return 1;
}
