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

#ifndef __SPRITE_H
#define __SPRITE_H

#include <string>
#include <SDL/SDL.h>
#include "debugprint.h"
class Sprite
{
	public:
		Sprite();
		Sprite(std::string, int = 0);
		Sprite(const Sprite&);
		virtual ~Sprite();

		void Blit(float destx, float desty) {
			if (!image) throw(std::string("Sprite::Blit(): Invalid image"));
			Blit(destx,desty,0,0,image->w,image->h);
		}

		virtual void Blit(float destx, float desty, float srcx, float srcy, float width, float height) {
			DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_DEBUGGING, "Sprite::Blit(float,float): Function not overloaded");
		}

		void loadSurface();
	protected:
		SDL_Surface *image;
		std::string filename;
		int index;
};

#endif
