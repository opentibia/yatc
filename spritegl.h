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

#ifndef __SPRITEGL_H
#define __SPRITEGL_H

#include <string>
#include "sprite.h"

class SpriteGL : public Sprite
{
	public:
		SpriteGL(const std::string& filename, int index = 0);
		SpriteGL(const std::string& filename, int index, int x, int y, int w, int h);
		SpriteGL(int w, int h, const oRGBA& c) : Sprite(w, h, c) {}
		virtual ~SpriteGL();

		void Blit(float destx, float desty, float srcx, float srcy, float width, float height);
		void Blit(float destx, float desty, float srcx, float srcy, float srcw, float srch, float destw, float desth);
		void buildGLTexture();
		void addColor(float r, float g, float b);
        void destroyGLTexture();
        void rebuildSelf();
	private:
		GLuint m_texture;
		double m_multiplierx, m_multipliery;
		uint32_t m_engineCreationTimestamp;
		std::shared_ptr<RGBAStorage> m_image;
};

#endif
