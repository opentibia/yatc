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
#include <GL/glu.h>
#include "spritegl.h"
SpriteGL::SpriteGL(std::string filename, int index)  : Sprite(filename, index) {

	int extbegins = filename.rfind(".")+1;
	std::string extension = filename.substr(extbegins, filename.length() - extbegins);
	GLuint pixelformat = GL_RGBA;
	if (extension == "bmp") {
		pixelformat = GL_BGR;
	}

	glGenTextures(1, &texture);
	SDL_LockSurface(image);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
			 GL_RGBA,
			 image->w, image->h,
			 pixelformat,
			 GL_UNSIGNED_BYTE,
			 image->pixels);

	SDL_UnlockSurface(image);
}

SpriteGL::~SpriteGL() {
	glDeleteTextures(1, &texture);
}

void SpriteGL::Blit(float destx, float desty, float srcx, float srcy, float width, float height) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(destx, desty);
		glTexCoord2f(0,height/image->h);
		glVertex2f(destx, desty+height);
		glTexCoord2f(width/image->w,height/image->h);
		glVertex2f(destx+width, desty+height);
		glTexCoord2f(width/image->w,0);
		glVertex2f(destx+width, desty);
	glEnd();


}
