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
#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else
	#ifdef _WIN32
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif
	#include <GL/glu.h>
#endif
#endif

#include <string>
#include <memory>
#include "rgba_storage.h"
#ifndef __USE_GLSDL__
#include <SDL/SDL.h>
#else
#include <SDL/glSDL.h>
#endif
#include "stdinttypes.h"

struct SDL_Surface;
class oRGBA;
class Sprite
{
	public:
        Sprite(int w, int h, const oRGBA& c);
		Sprite(const std::string& filename, int index);
		Sprite(const std::string& filename, int index, int x, int y, int w, int h);
		virtual ~Sprite();

		virtual float getWidth() const { return m_stretchimage ? m_stretchimage->getWidth() : m_image->getWidth(); }
		virtual float getHeight() const { return m_stretchimage ? m_stretchimage->getHeight() : m_image->getHeight(); }
		float getBasicWidth() const { return m_image->getWidth(); }
		float getBasicHeight() const { return m_image->getHeight(); }

		bool isLoaded() { return m_loaded;}

		void Blit(float destx, float desty){
			if(m_image){
				Blit(destx, desty, 0, 0, getBasicWidth(), getBasicHeight());
			}
		}

		virtual void Blit(float destx, float desty, float srcx, float srcy, float width, float height) = 0;
		virtual void Blit(float destx, float desty, float srcx, float srcy, float srcw, float srch, float destw, float desth) = 0; // stretch from SRCWxSRCH to DESTWxDESTH
        virtual void addColor(float r, float g, float b);
        virtual void resetColor() {addColor(1,1,1); }
        virtual void templatedColorize(Sprite* templatespr, uint8_t head, uint8_t body, uint8_t legs, uint8_t feet);
        virtual void rebuildSelf() {} // in case some engine (like GL) needs to rebuild texture from SDL surface ...
		void templatedColorizePixel(uint8_t color, uint8_t &r, uint8_t &g, uint8_t &b);

		void Stretch(float neww, float newh, int smooth = -1, bool force = false);
		void unStretch() { if (m_stretchimage) m_stretchimage.reset(); m_stretchimage = nullptr; }

		void setAsIcon();// used only once, in main

		SDL_Cursor* createCursor(int topx, int topy, int w, int h, int hot_x, int hot_y);



        // use functions for direct modification SPARINGLY!
        virtual std::shared_ptr<RGBAStorage> getSurface() { return m_image; }
        virtual std::shared_ptr<RGBAStorage> lockSurface();
        virtual void unlockSurface();

        void putPixel(int x, int y, uint32_t pixel, std::shared_ptr<RGBAStorage> img = nullptr);
		uint32_t getPixel(int x, int y, std::shared_ptr<RGBAStorage> img = nullptr);

	protected:
		Sprite(const Sprite& original);

		void loadSurfaceFromFile(const std::string& filename, int index);

		std::shared_ptr<RGBAStorage> getColoredImage() { return m_stretchimage ? m_stretchimage : ((m_r != 1. || m_g != 1. || m_b != 1.) ? m_coloredimage : m_image); }
		std::shared_ptr<RGBAStorage> getImage() { return m_stretchimage ? m_stretchimage : m_image; }
		std::shared_ptr<RGBAStorage> getBasicImage() { return m_image; }
		#ifdef USE_OPENGL
		GLuint getPixelFormat() { return m_pixelformat; }
		#endif

		std::string m_filename; int m_index;
		float m_r, m_g, m_b;

	private:

		bool m_loaded;
		std::shared_ptr<RGBAStorage> m_image, m_stretchimage, m_coloredimage;

		bool m_smoothstretch;
		#ifdef USE_OPENGL
		GLuint m_pixelformat;
		#endif
};

#endif
