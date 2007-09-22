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

#include <GL/gl.h>
#include "sprite.h"
#ifdef WIN32
#include <GL/glext.h>
#endif

Sprite::Sprite(const std::string& filename, int index) {
	m_pixelformat = GL_NONE;
	m_image = NULL;

	size_t extbegins = filename.rfind(".") + 1;
	std::string extension;
	if(extbegins != std::string::npos){
		extension = filename.substr(extbegins, filename.length() - extbegins);
	}

	if(extension == "bmp"){
		m_image = SDL_LoadBMP(filename.c_str());
		if(!m_image){
			printf("Error [Sprite::loadSurfaceFromFile] SDL_LoadBMP file: %s\n", filename.c_str());
			return;
		}
		m_pixelformat = GL_BGR;
	}
	else {
		// m_image is already marked as NULL, so we're over
		return;
	}

	SDL_SetColorKey(m_image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 0xFF, 0, 0xFF)); // magneta is transparent
}

Sprite::~Sprite()
{
	if(m_image){
		SDL_FreeSurface(m_image);
	}
}
