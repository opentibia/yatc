//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Object data
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

#include <iostream>
#include "objects.h"
#include "engine.h" // used to create engine specific sprites
#include "util.h"

uint16_t ObjectType::minItemId = 0;
uint16_t ObjectType::maxItemId = 0;

uint16_t ObjectType::minOutfitId = 0;
uint16_t ObjectType::maxOutfitId = 0;

uint16_t ObjectType::minEffectId = 0;
uint16_t ObjectType::maxEffectId = 0;

uint16_t ObjectType::minDistanceId = 0;
uint16_t ObjectType::maxDistanceId = 0;

Objects* Objects::m_instance = NULL;

ObjectType::ObjectType(uint16_t _id)
{
	id = _id;
	imageData = NULL;
	ground = false;
	speed = 0;
	alwaysOnTopOrder = 5; //0: ground 1-3: topItems 4: creatures 5: default for downItems,
	alwaysOnTop = false;
	container = false;
	stackable = false;
	useable = false;
	rune = false;
	readable = false;
	fluidContainer = false;
	splash = false;
	blockSolid = false;
	moveable = false;
	blockProjectile = false;
	blockPathFind = false;
	pickupable = false;
	isHangable = false;
	isHorizontal = false;
	isVertical = false;
	rotatable = false;
	lightLevel = 0;
	lightColor = 0;
	xOffset = 0;
	yOffset = 0;
	hasHeight = false;
	mapColor = 0;
	width = 1;
	height = 1;
	blendframes = 0;
	xdiv = 1;
	ydiv = 1;
	unk1 = 0;
	animcount = 1;
	numsprites = 0;
	//0x06 property
	alwaysUsed = false;

	instancesOnMap = 0;
}

ObjectType::~ObjectType()
{
    unloadGfx();
}


void ObjectType::loadGfx()
{
    ASSERT(imageData)

    if (m_gfx.size() != numsprites) { // graphics not loaded yet?
        for(uint32_t i = 0; i < numsprites; i++){
            m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", imageData[i]));
        }
    }

}

void ObjectType::unloadGfx()
{
    std::vector<Sprite*>::iterator it;
	for(it = m_gfx.begin(); it != m_gfx.end(); ++it){
		delete *it;
	}
	m_gfx.clear();
}

const std::vector<Sprite*>& ObjectType::getGfx() const {
    return m_gfx;
}

bool ObjectType::isGfxLoaded() const {
    ASSERT(imageData);
    return (m_gfx.size()) && (numsprites != 0);
}

Objects::Objects() : m_item(8192), m_outfit(256),
	m_effect(32), m_distance(32)
{
	m_datLoaded = false;
}

Objects::~Objects()
{

}

Objects* Objects::getInstance()
{
	// TODO (mips_act#3#): Choose Object instance depending on selected version

	if(!m_instance)
		m_instance = new Objects();

	return m_instance;
}

bool Objects::unloadDat()
{
	if(m_datLoaded){
		unsigned int i = 100;
		while(ObjectType* oType = m_item.getElement(i)){
			if(oType->imageData != NULL){
				delete[] oType->imageData;
				oType->imageData = NULL;
			}
			delete oType;
			++i;
		}
		printf("Unloaded %d item objects\n",i);
		i = 1;
		while(ObjectType* oType = m_outfit.getElement(i)){
			if(oType->imageData != NULL){
				delete[] oType->imageData;
				oType->imageData = NULL;
			}
			delete oType;
			++i;
		}
		printf("Unloaded %d outfit objects\n",i);
		i = 1;
		while(ObjectType* oType = m_effect.getElement(i)){
			if(oType->imageData != NULL){
				delete[] oType->imageData;
				oType->imageData = NULL;
			}
			delete oType;
			++i;
		}
		printf("Unloaded %d effect objects\n",i);
		i = 1;
		while(ObjectType* oType = m_distance.getElement(i)){
			if(oType->imageData != NULL){
				delete[] oType->imageData;
				oType->imageData = NULL;
			}
			delete oType;
			++i;
		}
		printf("Unloaded %d distance objects\n",i);

		m_datLoaded = false;
	}
	return true;
}



void Objects::unloadGfx()
{
	if(m_datLoaded){
		unsigned int i = 100;
		while(ObjectType* oType = m_item.getElement(i)){
			oType->unloadGfx();
			++i;
		}
		printf("Unloaded %d item gfx\n",i);
		i = 1;
		while(ObjectType* oType = m_outfit.getElement(i)){
			oType->unloadGfx();
			++i;
		}
		printf("Unloaded %d outfit gfx\n",i);
		i = 1;
		while(ObjectType* oType = m_effect.getElement(i)){
			oType->unloadGfx();
			++i;
		}
		printf("Unloaded %d effect gfx\n",i);
		i = 1;
		while(ObjectType* oType = m_distance.getElement(i)){
			oType->unloadGfx();
			++i;
		}
		printf("Unloaded %d distance gfx\n",i);
	}
}




bool Objects::loadDat(const char* filename)
{
	if(m_datLoaded) {
	    printf("LOADING DATA FILE %s BUT ALREADY LOADED\n", filename);
		return false;
	}

	uint16_t id = 100;
	int32_t size;
	uint16_t read_short, read_short2;
	uint32_t maxObjects = 0;

    printf("LOADING DATA FILE %s\n", filename);

	FILE *fp = yatc_fopen(filename, "rb");
	if(!fp){
		return false;
	}

	fseek(fp,0,SEEK_END);
	size = ftell(fp);

	//get max id
	fseek(fp, 0x04, SEEK_SET);
	//Items
	fread(&read_short, 2, 1, fp);
	ObjectType::minItemId = 100;
	ObjectType::maxItemId = read_short;
	maxObjects += ObjectType::maxItemId;
	//Outfits
	fread(&read_short, 2, 1, fp);
	ObjectType::minOutfitId = 0;
	ObjectType::maxOutfitId = read_short;
	maxObjects += ObjectType::maxOutfitId;
	//Effects
	fread(&read_short, 2, 1, fp);
	ObjectType::minEffectId = 0;
	ObjectType::maxEffectId = read_short;
	maxObjects += ObjectType::maxEffectId;
	//Distance
	fread(&read_short, 2, 1, fp);
	ObjectType::minDistanceId = 0;
	ObjectType::maxDistanceId = read_short;
	maxObjects += ObjectType::maxDistanceId;

	/*
	 * A T T E N T I O N ! ! !
	 *
	 * Do not update the reader to 8.1 without first making it possible
	 * to choose between 8.1 and 8.0 reader. We want to be able to load
	 * older formats as well.
	 */


	while(ftell(fp) < size && id <= maxObjects){
		ObjectType* oType = new ObjectType(id);

		int optbyte;
		//bool colorTemplate = false;
		while(((optbyte = fgetc(fp)) >= 0) && (optbyte != 0xFF)){
			switch(optbyte){
				case 0x00: //Ground tile
						fread(&read_short, 2, 1, fp);
						oType->speed = read_short;
						oType->ground = true;
						oType->alwaysOnTopOrder = 0;
					break;
				case 0x01: //ontop
						oType->alwaysOnTop = true;
						oType->alwaysOnTopOrder = 1;
					break;
				case 0x02: //Walk through (doors etc)
						oType->alwaysOnTop = true;
						oType->alwaysOnTopOrder = 2;
					break;
				case 0x03: //Can walk trough (arces)
						oType->alwaysOnTop = true;
						oType->alwaysOnTopOrder = 3;
					break;
				case 0x04: //Container
						oType->container = true;
					break;
				case 0x05: //Stackable
						oType->stackable = true;
					break;
				case 0x06: //Ladders?
						//printf("Has 0x06 attribute: ID %d\n", id);
						oType->alwaysUsed = true;
					break;
				case 0x07: //Useable
						oType->useable = true;
					break;
				case 0x08: //Runes
						oType->rune = true;
					break;
				case 0x09: //Writtable/Readable Objectss
						oType->readable = true;
						fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
						break;
				case 0x0A: //Writtable Objectss that can't be edited
						oType->readable = true;
						fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
					break;
				case 0x0B: //Fluid containers
						oType->fluidContainer = true;
					break;
				case 0x0C: //Splashes?
						oType->splash = true;
					break;
				case 0x0D: //Is blocking
						oType->blockSolid = true;
					break;
				case 0x0E: //Is not moveable
						oType->moveable = false;
					break;
				case 0x0F: //Blocks missiles (walls, magic wall etc)
						oType->blockProjectile = true;
					break;
				case 0x10: //Blocks monster movement (flowers, parcels etc)
						oType->blockPathFind = true;
					break;
				case 0x11: //Can be equipped
						oType->pickupable = true;
					break;
				case 0x12: //Wall items
						oType->isHangable = true;
					break;
				case 0x13:
						oType->isHorizontal = true;
					break;
				case 0x14:
						oType->isVertical = true;
					break;
				case 0x15: //Rotatable items
						oType->rotatable = true;
					break;
				case 0x16: //Light info
						fread(&read_short, sizeof(read_short), 1, fp);
						oType->lightLevel = read_short;
						fread(&read_short, sizeof(read_short), 1, fp);
						oType->lightColor = read_short;
					break;
				case 0x17:  //Floor change?

					break;
				case 0x18: //??
						optbyte = optbyte;
					break;
				case 0x19: //Offset?
						fread(&read_short, sizeof(read_short), 1, fp);
						oType->xOffset = read_short;
						fread(&read_short, sizeof(read_short), 1, fp);
						oType->yOffset = read_short;
					break;
				case 0x1A:
						oType->hasHeight = true;
						// (should be) the height change in px; Tibia always uses 8
						fread(&read_short, sizeof(read_short), 1, fp); // ?
					break;
				case 0x1B://draw with height offset for all parts (2x2) of the sprite

					break;
				case 0x1C://some monsters

					break;
				case 0x1D:
						fread(&read_short, sizeof(read_short), 1, fp);
						oType->mapColor = read_short;
					break;
				case 0x1E:  //line spot
						int tmp;
						tmp = fgetc(fp); // 86 -> openable holes, 77-> can be used to go down, 76 can be used to go up, 82 -> stairs up, 79 switch,
						if(tmp == 0x58)
							oType->readable = true;

						fgetc(fp); // always 4
					break;
				case 0x1F: //?

					break;
				default:
						optbyte = optbyte;
						//std::cout << "unknown byte: " << (uint16_t)optbyte << std::endl;
						return false;
					break;
			}
		}

		oType->width  = fgetc(fp);
		oType->height = fgetc(fp);
		if((oType->width > 1) || (oType->height > 1)){
			fgetc(fp);
		}

		oType->blendframes = fgetc(fp);
		oType->xdiv        = fgetc(fp);
		oType->ydiv        = fgetc(fp);
		oType->unk1        = fgetc(fp);
		oType->animcount   = fgetc(fp);

		oType->numsprites = oType->width * oType->height * oType->blendframes * oType->xdiv * oType->ydiv * oType->animcount * oType->unk1;

		oType->imageData = new uint16_t[oType->numsprites];

        ASSERT(oType->imageData);

		for(unsigned int i = 0; i < oType->numsprites; i++) {
			fread(&oType->imageData[i], sizeof(uint16_t), 1, fp);
		}

		if(id <= ObjectType::maxItemId){
			m_item.addElement(oType, id);
		}
		else if(id <= (ObjectType::maxItemId + ObjectType::maxOutfitId)){
			m_outfit.addElement(oType, id - ObjectType::maxItemId);
		}
		else if(id <= (ObjectType::maxItemId + ObjectType::maxOutfitId + ObjectType::maxEffectId)){
			m_effect.addElement(oType, id - ObjectType::maxItemId - ObjectType::maxOutfitId);
		}
		else if(id <= (ObjectType::maxItemId + ObjectType::maxOutfitId + ObjectType::maxEffectId + ObjectType::maxDistanceId)){
			m_distance.addElement(oType, id - ObjectType::maxItemId - ObjectType::maxOutfitId - ObjectType::maxEffectId);
		}

		id++;
	}

	fclose(fp);
	m_datLoaded = true;

	return true;
}

ObjectType* Objects::getItemType(uint16_t id)
{
	return m_item.getElement(id);
}

ObjectType* Objects::getOutfitType(uint16_t id)
{
	return m_outfit.getElement(id);
}

ObjectType* Objects::getEffectType(uint16_t id)
{
	return m_effect.getElement(id);
}

ObjectType* Objects::getDistanceType(uint16_t id)
{
	return m_distance.getElement(id);
}
