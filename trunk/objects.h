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

#ifndef __OBJECTS_H
#define __OBJECTS_H

#include <stdlib.h>
#include "stdinttypes.h"
#include <string.h>
#include <vector>

template<typename A>
class Array{
public:
	Array(uint32_t  n){
		m_data = (A*)malloc(sizeof(A)*n);
		memset(m_data, 0, sizeof(A)*n);
		m_size = n;
	};

	~Array(){
		free(m_data);
	};

	A getElement(uint32_t  id){
		if(id < m_size){
			return m_data[id];
		}
		else{
			return 0;
		}
	};
	void addElement(A a, uint32_t pos){
		#define INCREASE 1024
		if(pos >= m_size){
			m_data = (A*)realloc(m_data, sizeof(A)*(pos + INCREASE));
			memset(m_data + m_size, 0, sizeof(A)*(pos + INCREASE - m_size));
			m_size = pos + INCREASE;
		}
		m_data[pos] = a;
	};
	uint32_t size(){return m_size;}
private:
	A* m_data;
	uint32_t m_size;
};

class Sprite;

class ObjectType
{
public:
	ObjectType(uint16_t _id);
	~ObjectType();

    // item only functions (perhaps later they will apply for effects too)
    void loadGfx();
    void unloadGfx();
    bool isGfxLoaded() const;
	const std::vector<Sprite*>& getGfx() const;
	int instancesOnMap; // how many times itemUI for this one was spawned

	uint16_t id;

	bool ground;
	uint16_t speed;
	uint16_t alwaysOnTopOrder;
	bool alwaysOnTop;
	bool container;
	bool stackable;
	bool useable;
	bool rune;
	bool readable;
	bool fluidContainer;
	bool splash;
	bool blockSolid;
	bool moveable;
	bool blockProjectile;
	bool blockPathFind;
	bool pickupable;
	bool isHangable;
	bool isHorizontal;
	bool isVertical;
	bool rotatable;
	//items with 0x06 property
	bool alwaysUsed;
	uint16_t lightLevel;
	uint16_t lightColor;
	uint16_t xOffset;
	uint16_t yOffset;
	bool hasHeight;
	uint16_t mapColor;
	uint16_t width;
	uint16_t height;
	uint16_t blendframes;
	uint16_t xdiv;
	uint16_t ydiv;
	uint16_t unk1;
	uint16_t animcount;
	uint16_t numsprites;

	uint16_t* imageData;

	static uint16_t minItemId;
	static uint16_t maxItemId;

	static uint16_t minOutfitId;
	static uint16_t maxOutfitId;

	static uint16_t minEffectId;
	static uint16_t maxEffectId;

	static uint16_t minDistanceId;
	static uint16_t maxDistanceId;

private:
    std::vector<Sprite*> m_gfx;
};

class Objects
{
public:
	Objects();
	~Objects();

	static Objects* getInstance();
	static void destroyInstance() { delete m_instance; m_instance = NULL; }

	bool unloadDat();
    void unloadGfx();
	bool loadDat(const char* filename);

	ObjectType* getItemType(uint16_t id);
	ObjectType* getOutfitType(uint16_t id);
	ObjectType* getEffectType(uint16_t id);
	ObjectType* getDistanceType(uint16_t id);

    bool isLoaded() { return m_datLoaded; }
protected:
	bool m_datLoaded;
	static Objects* m_instance;

	Array<ObjectType*> m_item;
	Array<ObjectType*> m_outfit;
	Array<ObjectType*> m_effect;
	Array<ObjectType*> m_distance;
};

#endif
