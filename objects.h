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

#ifndef __Objects_H
#define __Objects_H

#include <stdlib.h>
#include <string.h>


template<typename A>
class Array{
public:
	Array(unsigned long  n){
		m_data = (A*)malloc(sizeof(A)*n);
		memset(m_data, 0, sizeof(A)*n);
		m_size = n;
	};
	
	~Array(){
		free(m_data);
	}; 
	
	A getElement(unsigned long  id){
		if(id >= 0 && id < m_size){
			return m_data[id];
		}
		else{
			return 0;
		}	
	};
	void addElement(A a, unsigned long pos){
		if(pos >= 0){
			#define INCREASE 1024
			if(pos >= m_size){
				m_data = (A*)realloc(m_data, sizeof(A)*(pos + INCREASE));
				memset(m_data + m_size, 0, sizeof(A)*(pos + INCREASE - m_size));
				m_size = pos + INCREASE;
			}
			m_data[pos] = a;
		}	
	};
	unsigned long size(){return m_size;}
private:
	A* m_data;
	unsigned long m_size;
};

class ObjectType
{
public:
	ObjectType(unsigned short _id);
	~ObjectType();
	
protected:
	unsigned short id;
	
	bool ground;
	unsigned short speed;
	unsigned short alwaysOnTopOrder;
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
	unsigned short lightLevel;
	unsigned short lightColor;
	unsigned short xOffset;
	unsigned short yOffset;
	bool hasHeight;
	unsigned short mapColor;
	unsigned short width;
	unsigned short height;
	unsigned short blendframes;
	unsigned short xdiv;
	unsigned short ydiv;
	unsigned short unk1;
	unsigned short animcount;
	unsigned short numsprites;
	
	unsigned short* imageData;
	
	static unsigned short minItemId;
	static unsigned short maxItemId;
	
	static unsigned short minOutfitId;
	static unsigned short maxOutfitId;
	
	static unsigned short minEffectId;
	static unsigned short maxEffectId;
	
	static unsigned short minDistanceId;
	static unsigned short maxDistanceId;
	
	friend class Objects;
};

class Objects
{
public:
	Objects();
	~Objects();
	
	static Objects* getInstance();
	
	bool unloadDat();
	bool loadDat(const char* filename);
	
	ObjectType* getItemType(unsigned short id);
	ObjectType* getOutfitType(unsigned short id);
	ObjectType* getEffectType(unsigned short id);
	ObjectType* getDistanceType(unsigned short id);

protected:
	bool m_datLoaded;
	static Objects* m_instance;

	Array<ObjectType*> m_item;
	Array<ObjectType*> m_outfit;
	Array<ObjectType*> m_effect;
	Array<ObjectType*> m_distance;
};

#endif
