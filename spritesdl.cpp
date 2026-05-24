//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Sprite for SDL engine
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

#include "enginesdl.h"
#include "spritesdl.h"

#include "options.h"

SpriteSDL::SpriteSDL(int w, int h, const oRGBA& c) : Sprite(w, h, c)
{

}

SpriteSDL::SpriteSDL(const std::string& filename, int index) : Sprite(filename, index)
{

}

SpriteSDL::~SpriteSDL()
{

}


void SpriteSDL::Blit(float dx, float dy, float sx, float sy, float w, float h)
{
    if(!getImage()) {
		return;
	}
    unStretch();

    _BlitInternal(dx,dy,sx,sy,w,h);

}

void SpriteSDL::_BlitInternal(float dx, float dy, float sx, float sy, float w, float h)
{
	if(!getImage()) {
		return;
	}

	// code is like this because of dx5.0 ... see docs/html/sdlrect.html in SDL documentation for more info
	SDL_Rect src = {(Sint16)sx,(Sint16)sy,(Uint16)(w),(Uint16)(h)};
	SDL_Rect dst = {(Sint16)dx,(Sint16)dy,(Uint16)(w),(Uint16)(h)};
	while(SDL_BlitSurface(static_cast<SDLSurfaceStorage*>(getColoredImage().get())->getSurface(), &src, static_cast<SDLSurfaceStorage*>(g_engine->m_screen.get())->getSurface(), &dst) == -2){
		while(SDL_LockSurface(static_cast<SDLSurfaceStorage*>(getColoredImage().get())->getSurface()) < 0 ){
				SDL_Delay(10);
		}
		loadSurfaceFromFile(m_filename, m_index);

		SDL_UnlockSurface(static_cast<SDLSurfaceStorage*>(getColoredImage().get())->getSurface());
	}
}


void SpriteSDL::Blit(float dx, float dy, float sx, float sy, float w, float h, float destw, float desth)
{
	if(!getImage())
		return;

    double lambdaw = destw / w;
    double lambdah = desth / h;

    double neww = getBasicImage()->getWidth() * lambdaw;
    double newh = getBasicImage()->getHeight() * lambdaw;
    if(getImage()->getWidth() != neww || getImage()->getHeight() != newh) {
		Stretch(neww, newh, options.smoothstretch);
		oldw = neww;
		oldh = newh;
    }

	_BlitInternal(dx,dy,sx*lambdaw,sy*lambdah,destw,desth);
}
