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

#ifdef USE_OPENGL
	#include <GL/glu.h>
#endif

#include <string>
#include <SDL/SDL.h>

class SDL_Surface;

class Sprite
{
	public:
		Sprite(const std::string& filename, int index);
		Sprite(const std::string& filename, int index, int x, int y, int w, int h);
		virtual ~Sprite();

		virtual float getWidth() const { return m_stretchimage ? m_stretchimage->w : m_image->w; }
		virtual float getHeight() const { return m_stretchimage ? m_stretchimage->h : m_image->h; }

		bool isLoaded() { return m_loaded;}

		void Blit(float destx, float desty){
			if(m_image){
				Blit(destx, desty, 0, 0, getWidth(), getHeight());
			}
		}

		virtual void Blit(float destx, float desty, float srcx, float srcy, float width, float height) = 0;
		virtual void Blit(float destx, float desty, float srcx, float srcy, float srcw, float srch, float destw, float desth) = 0; // stretch from SRCWxSRCH to DESTWxDESTH

		void Stretch(float neww, float newh, bool smooth = false);
		void unStretch() { if (m_stretchimage) SDL_FreeSurface(m_stretchimage); m_stretchimage = NULL; }

	protected:
		Sprite(const Sprite& original);

		void loadSurfaceFromFile(const std::string& filename, int index);

		SDL_Surface* getImage() { return m_stretchimage ? m_stretchimage : m_image; }
		#ifdef USE_OPENGL
		GLuint getPixelFormat() { return m_pixelformat; }
		#endif
		
		std::string m_filename; int m_index;
	private:
		void putPixel(int x, int y, uint32_t pixel);
		uint32_t getPixel(int x, int y);

		bool m_loaded;
		SDL_Surface *m_image, *m_stretchimage;
		#ifdef USE_OPENGL
		GLuint m_pixelformat;
		#endif

};

#endif
