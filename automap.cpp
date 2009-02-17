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
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include "automap.h"
#include "sprite.h"
#include "engine.h"
#include "util.h"
#include "gamecontent/globalvars.h"


bool MiniMapArea::save()
{
	std::stringstream x, y, z, minimapfnss;
	//get the name of the file
	x << setw(3) << setfill('0') << m_basepos.x / 256;
    y << setw(3) << setfill('0') << m_basepos.y / 256;
    z << setw(2) << setfill('0') << m_basepos.z;
    minimapfnss << x.str() << y.str() << z.str() << ".map";
    FILE* f = yatc_fopen(minimapfnss.str().c_str(), "wb");
    if(!f){
    	return false;
    }
    fwrite(m_color, 1, 256*256, f);
    fwrite(m_speed, 1, 256*256, f);

    //save map marks if any
    for(std::list<MapMark*>::iterator it = m_marks.begin(); it != m_marks.end(); ++it){
    	fwrite(&((*it)->x), 1, 4, f);
    	fwrite(&((*it)->y), 1, 4, f);
    	fwrite(&((*it)->type), 1, 4, f);
		uint16_t length = (*it)->text.size();
    	fwrite(&length, 1, 2, f);
    	fwrite((*it)->text.c_str(), 1, length, f);
    }


    fclose(f);
    return true;
}

bool MiniMapArea::load()
{
	//get the name of the file
	std::stringstream x, y, z, minimapfnss;
	x << setw(3) << setfill('0') << m_basepos.x / 256;
    y << setw(3) << setfill('0') << m_basepos.y / 256;
    z << setw(2) << setfill('0') << m_basepos.z;
    minimapfnss << x.str() << y.str() << z.str() << ".map";
    FILE* f = yatc_fopen(minimapfnss.str().c_str(), "rb");
    if(!f){
    	memset(m_color, 0, 256*256);
    	memset(m_speed, 255, 256*256);
    	return false;
    }

    fread(m_color, 1, 256*256, f);
    fread(m_speed, 1, 256*256, f);

	if(!feof(f)){ //there are map marks
		int32_t marksCount = 0;
		fread(&marksCount, 4, 1, f);
		if(marksCount > 100) marksCount = 100;
		for(int i = 0; i < marksCount; ++i){
			uint32_t x, y, type;
			uint16_t length;
			char *text;

			fread(&x, 4, 1, f);
			fread(&y, 4, 1, f);
			fread(&type, 4, 1, f);
			fread(&length, 2, 1, f);
			text = new char[length + 1];
			fread(text, length, 1, f);
			text[length] = 0;

			MapMark* mark = new MapMark(x, y, type, text);
			m_marks.push_back(mark);

			delete[] text;
		}
	}
    fclose(f);

    return true;
}

Automap::Automap()
{
	m_mapw = 256;
	m_maph = 256;
	oRGBA color; //default color
	m_bitmap = g_engine->createSprite(m_mapw, m_maph, color);
}

Automap::~Automap()
{
	delete m_bitmap;
	for(std::map<uint32_t, MiniMapArea*>::iterator it = m_areas.begin(); it != m_areas.end(); ++it){
		delete it->second;
	}
}

void Automap::setTileColor(int x, int y, int z, uint8_t color, uint8_t speedindex)
{
	uint32_t posindex = (z & 0xFF) | ((y & 0xFF00) << 8) | ((x & 0xFF00) << 16);
	std::map<uint32_t, MiniMapArea*>::iterator it = m_areas.find(posindex);
	if(it != m_areas.end()){
		it->second->setTileColor(x & 0xFF, y & 0xFF, color, speedindex);
	}
	else{
		MiniMapArea* area = new MiniMapArea(x, y, z);
		area->setTileColor(x & 0xFF, y & 0xFF, color, speedindex);
		m_areas[posindex] = area;
	}
}

void Automap::getTileColor(int x, int y, int z, uint8_t &color, uint8_t &speedindex)
{
	uint32_t posindex = (z & 0xFF) | ((y & 0xFF00) << 8) | ((x & 0xFF00) << 16);
	std::map<uint32_t, MiniMapArea*>::iterator it = m_areas.find(posindex);
	if(it != m_areas.end()){
		it->second->getTileColor(x & 0xFF, y & 0xFF, color, speedindex);
	}
	else{
		MiniMapArea* area = new MiniMapArea(x, y, z);
		area->getTileColor(x & 0xFF, y & 0xFF, color, speedindex);
		m_areas[posindex] = area;
	}
}

void Automap::updateSelf()
{
	//TODO: optimize how is generated the sprite
	// now it is completly rebuilt for every step
    int x1 = m_pos.x - m_mapw/2;
    int y1 = m_pos.y - m_maph/2;

	SDL_Surface* s = m_bitmap->lockSurface();
	for(int i = 0; i < m_mapw; i++){
		for(int j = 0; j < m_maph; j++){

			int tmpx = x1 + i;
			int tmpy = y1 + j;
			if(tmpx < 0 || tmpy < 0 || tmpx > 0xFFFF || tmpy > 0xFFFF){
				m_bitmap->putPixel(i, j, SDL_MapRGB(s->format, 0, 0, 0) ,s);
			}
			else{
				uint8_t color, speedIndex;
				getTileColor(tmpx, tmpy, m_pos.z, color, speedIndex);

				uint8_t b = uint8_t((color % 6) / 5. * 255);
				uint8_t g = uint8_t(((color / 6) % 6) / 5. * 255);
				uint8_t r = uint8_t((color / 36.) / 6. * 255);

				m_bitmap->putPixel(i, j, SDL_MapRGB(s->format, r, g, b) ,s);
			}
		}
	}
	m_bitmap->unlockSurface();
}

void Automap::setPos(const Position& pos)
{
    m_pos = pos;
    updateSelf();
}

void Automap::renderSelf(int x, int y, int w, int h) // parameters specify where on the screen it should be painted
{
    // FIXME (ivucica#1#): fix use of w and h;
    //currently they need to be default values 256x256

    if(GlobalVariables::getPlayerPosition() != m_pos){
    	setPos(GlobalVariables::getPlayerPosition());
    }

	//draw the minimap
    m_bitmap->Blit(x, y);

    //mark where is the player
    g_engine->drawRectangle(x + w/2, y + h/2, 3, 3, oRGBA(1,1,1,1));
}

void Automap::flushTiles()
{
	//
}
