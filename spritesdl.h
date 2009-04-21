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

#ifndef __SPRITESDL_H
#define __SPRITESDL_H

#include <string>
#include "sprite.h"

class SpriteSDL : public Sprite
{
public:
    SpriteSDL(int w, int h, const oRGBA& c);
	SpriteSDL(const std::string& filename, int index = 0);
	SpriteSDL(const std::string& filename, int index, int x, int y, int w, int h);

	virtual ~SpriteSDL();

	virtual void Blit(float dx, float dy, float sx, float sy, float w, float h);
	virtual void Blit(float destx, float desty, float srcx, float srcy, float srcw, float srch, float destw, float desth);
private:
    void _BlitInternal(float dx, float dy, float sx, float sy, float w, float h);
    friend class Engine;

};

#endif
