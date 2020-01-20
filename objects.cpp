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
#include "options.h"
#include "net/protocolconfig.h"
#include "product.h"

#include <fstream>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif


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
	writeable = false;
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
	lookThrough = false;

	width = 1;
	height = 1;
	rendersize = 32;
	blendframes = 0;
	xdiv = 1;
	ydiv = 1;
	zdiv = 0;
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
	bool success = false;
	if(m_datLoaded) {
	    printf("LOADING DATA FILE %s BUT ALREADY LOADED\n", filename);
		return false;
	}


	ClientVersion_t ver = options.protocol;
	if(ver == CLIENT_VERSION_AUTO) {
		// detectVersion will return CLIENT_VERSION_AUTO in case of failure
		ver = ProtocolConfig::detectVersion();
	}



	/*

	outline of revamped loader:

	- we have a std::map with all loaders
	- key of std::map is minimal version where the loader applies
	- value of std::map is pointer to loader function
	- if detected version is larger or equal to a certain member, load, and if successful abort
	- otherwise try again

	in case detection of version failed (e.g. custom data file signatures) then ver == CLIENT_VERSION_AUTO
	and then we try to load with each and every loader. if we're successful, yay, we're happy and we bail out.

	*/

	std::map<ClientVersion_t, bool (Objects::*)(const char*)> loaders;

	loaders[CLIENT_VERSION_780] = &Objects::load780plus; /* 7.80 - 8.5 use almost same .dat format (with an addition hereanddthere)*/
	loaders[CLIENT_VERSION_755] = &Objects::load76_77series; /* 7.55 - 7.72 use same .dat format to the best of my knowledge */
	loaders[CLIENT_VERSION_740] = &Objects::load74_75series;


	for(std::map<ClientVersion_t, bool(Objects::*)(const char*)>::reverse_iterator it = loaders.rbegin(); it != loaders.rend(); it++)
	{
		if(ver >= it->first || ver == CLIENT_VERSION_AUTO)
		{
			success = (this->*it->second)(filename);
			if(success)
				break;
		}

	}

	if (!success)
	{
		NativeGUIError(	str_replace(	"$$PRODUCTSHORT$$", PRODUCTSHORT, gettext(
						"Unrecognized data files.\n"
						"\n"
						"* Please install a supported version of data files, or override\n"
						"  autodetection manually in configuration.\n"
						"\n"
						"* You may be attempting to use a too new version of data files.\n"
						"  Check if a new version of $$PRODUCTSHORT$$ came out which\n"
						"  supports this version of data files.")).c_str(),
				gettext("Data files not recognized"));
	}
	return success;
}



bool Objects::load780plus(const char* filename)
{
	uint16_t id = 100;
	int32_t size;
	uint16_t read_short, read_short2;
	uint32_t maxObjects = 0;

	FILE *fp = yatc_fopen(filename, "rb");
	if(!fp){
		return false;
	}

	fseek(fp,0,SEEK_END);
	size = ftell(fp);

	//get max id
	fseek(fp, 0x04, SEEK_SET);
	//Items
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minItemId = 100;
	ObjectType::maxItemId = read_short;
	maxObjects += ObjectType::maxItemId;
	//Outfits
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minOutfitId = 0;
	ObjectType::maxOutfitId = read_short;
	maxObjects += ObjectType::maxOutfitId;
	//Effects
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minEffectId = 0;
	ObjectType::maxEffectId = read_short;
	maxObjects += ObjectType::maxEffectId;
	//Distance
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minDistanceId = 0;
	ObjectType::maxDistanceId = read_short;
	maxObjects += ObjectType::maxDistanceId;

	while(ftell(fp) < size && id <= maxObjects){
		ObjectType* oType = new ObjectType(id);

		int optbyte;
		//bool colorTemplate = false;
		while(((optbyte = fgetc(fp)) >= 0) && (optbyte != 0xFF)){
			switch(optbyte){
				case 0x00: //Ground tile
						yatc_fread(&read_short, 2, 1, fp);
                        ECORR16(read_short);
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
						oType->writeable = true;
						yatc_fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
                        ECORR16(read_short);
						break;
				case 0x0A: //Writtable Objectss that can't be edited
						oType->readable = true;
						yatc_fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
                        ECORR16(read_short);
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
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
						oType->lightLevel = read_short;
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
						oType->lightColor = read_short;
					break;
				case 0x17:  //Floor change?

					break;
				case 0x18: //??
						optbyte = optbyte;
					break;
				case 0x19: //Offset?
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
						oType->xOffset = read_short;
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
						oType->yOffset = read_short;
					break;
				case 0x1A:
						oType->hasHeight = true;
						// (should be) the height change in px; Cipsoft always uses 8
						yatc_fread(&read_short, sizeof(read_short), 1, fp); // ?
                        ECORR16(read_short);
					break;
				case 0x1B://draw with height offset for all parts (2x2) of the sprite

					break;
				case 0x1C://idle animated
                        oType->idleAnim = true;
					break;
				case 0x1D:
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
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
				case 0x20: // New with Tibia 8.5 - "look through"
                        // NOTE (nfries88): Not sure if client or server does this...
                        // NOTE (ivucica): It's almost certainly the client
                        oType->lookThrough = true;
					break;
				default:
						optbyte = optbyte;
						std::cout << "unknown byte: " << (uint16_t)optbyte << std::endl;
						return false;
					break;
			}
		}

		oType->width  = fgetc(fp);
		oType->height = fgetc(fp);
		if((oType->width > 1) || (oType->height > 1)){
			oType->rendersize = fgetc(fp);
		}

		oType->blendframes = fgetc(fp);
		oType->xdiv        = fgetc(fp);
		oType->ydiv        = fgetc(fp);
		oType->zdiv        = fgetc(fp);
		oType->animcount   = fgetc(fp);

		oType->numsprites = oType->width * oType->height * oType->blendframes * oType->xdiv * oType->ydiv * oType->animcount * oType->zdiv;

		oType->imageData = new uint16_t[oType->numsprites];

        ASSERT(oType->imageData);

		for(unsigned int i = 0; i < oType->numsprites; i++) {
			yatc_fread(&oType->imageData[i], sizeof(uint16_t), 1, fp);
            ECORR16(oType->imageData[i]);
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

	std::ofstream outFile;
	outFile.open("/tmp/Tibia.dat.json");
	asJSON(outFile);
	outFile.close();

	return true;
}

// NOTE (nfries88): This actually loads 755 too
bool Objects::load76_77series(const char* filename)
{
	uint16_t id = 100;
	int32_t size;
	uint16_t read_short, read_short2;
	uint32_t maxObjects = 0;

	FILE *fp = yatc_fopen(filename, "rb");
	if(!fp){
		return false;
	}

	fseek(fp,0,SEEK_END);
	size = ftell(fp);

	//get max id
	fseek(fp, 0x04, SEEK_SET);
	//Items
	yatc_fread(&read_short, 2, 1, fp);
    ECORR16(read_short);
	ObjectType::minItemId = 100;
	ObjectType::maxItemId = read_short;
	maxObjects += ObjectType::maxItemId;
	//Outfits
	yatc_fread(&read_short, 2, 1, fp);
    ECORR16(read_short);
	ObjectType::minOutfitId = 0;
	ObjectType::maxOutfitId = read_short;
	maxObjects += ObjectType::maxOutfitId;
	//Effects
	yatc_fread(&read_short, 2, 1, fp);
    ECORR16(read_short);
	ObjectType::minEffectId = 0;
	ObjectType::maxEffectId = read_short;
	maxObjects += ObjectType::maxEffectId;
	//Distance
	yatc_fread(&read_short, 2, 1, fp);
    ECORR16(read_short);
	ObjectType::minDistanceId = 0;
	ObjectType::maxDistanceId = read_short;
	maxObjects += ObjectType::maxDistanceId;


	while(ftell(fp) < size && id <= maxObjects){
		ObjectType* oType = new ObjectType(id);

		int optbyte;
		//bool colorTemplate = false;
		while(((optbyte = fgetc(fp)) >= 0) && (optbyte != 0xFF)){
			switch(optbyte){
			case 0x00: //is groundtile
				yatc_fread(&read_short, 2, 1, fp);
                ECORR16(read_short);
				oType->speed = read_short;
				oType->ground = true;
				break;
			case 0x01: //all OnTop
				oType->alwaysOnTop = 1;
				break;
			case 0x02: //can walk trough (open doors, arces, bug pen fence ??)
				oType->alwaysOnTop = 2;
				break;
			case 0x03: //can walk trough (arces)
				oType->alwaysOnTop = 3;
				break;
			case 0x04: //is a container
				oType->useable = true;
				oType->container = true;
				//oType->stackable = true;
				break;
			case 0x05: //is stackable
				oType->stackable = true;
				break;
			case 0x06: //ladders
			    oType->useable = true;
				break;
			case 0x07: //is useable
				oType->useable = true;
				//
				break;
			case 0x08: //writtable objects
				oType->readable = true;
				yatc_fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
                ECORR16(read_short);
				break;
			case 0x09: //writtable objects that can't be edited
				oType->readable = true;
				yatc_fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
                ECORR16(read_short);
				break;
			case 0x0A: //can contain fluids
				oType->fluidContainer = true;

				break;
			case 0x0B: //liquid with states
				oType->splash = true;
				break;
			case 0x0C: //is blocking
				oType->blockSolid = true;
				break;
			case 0x0D: //is no moveable
				oType->moveable = false;
				break;
			case 0x0E: //blocks missiles (walls, magic wall etc)
				oType->blockProjectile = true;
				break;
			case 0x0F: //blocks monster movement (flowers, parcels etc)
				oType->blockPathFind = true;
				break;
			case 0x10: //can be equipped
				oType->pickupable = true;
				break;
			case 0x11: //wall items
				oType->isHangable = true;
				break;
			case 0x12:
				oType->isHorizontal = true;
				break;
			case 0x13:
				oType->isVertical = true;
				break;
			case 0x14: //rotable items
				oType->rotatable = true;
				break;
			case 0x15: //light info .. //sprite-drawing related
				unsigned short lightlevel;
				yatc_fread(&lightlevel, sizeof(lightlevel), 1, fp);
                ECORR16(read_short);
				oType->lightLevel = lightlevel;
				unsigned short lightcolor;
				yatc_fread(&lightcolor, sizeof(lightcolor), 1, fp);
                ECORR16(read_short);
				oType->lightColor = lightcolor;
				break;
			case 0x17:  //floor change
				break;
			case 0x18: //offset
				yatc_fread(&read_short, sizeof(read_short), 1, fp);
                ECORR16(read_short);
				oType->xOffset = read_short;
				yatc_fread(&read_short, sizeof(read_short), 1, fp);
                ECORR16(read_short);
				oType->yOffset = read_short;
				break;
			case 0x19: //height
				oType->hasHeight = true;
				// (should be) the height change in px; Cipsoft always uses 8
				yatc_fread(&read_short, sizeof(read_short), 1, fp);
                ECORR16(read_short);
				break;
			case 0x1A://draw with height offset for all parts (2x2) of the sprite
				break;
			case 0x1B: //some monsters
			    break;
			case 0x1C:
				unsigned short color;
				yatc_fread(&color, sizeof(color), 1, fp);
                ECORR16(read_short);
				oType->mapColor = color;
				break;
			case 0x1D:  //line spot
				int tmp;
				tmp = fgetc(fp); // 86 -> openable holes, 77-> can be used to go down, 76 can be used to go up, 82 -> stairs up, 79 switch,
				oType->useable=true;
				if(tmp == 0x58)
					oType->readable = true;
				fgetc(fp); // always 4
				break;
			case 0x1E: //ground items
				break;


			default:
				//optbyte = optbyte;
				//std::cout << "unknown byte: " << (unsigned short)optbyte << std::endl;
				//DEBUGPRINT("DATLOADER: Unknown byte %.02x, item %d, at offset %d; lastok was = %.02x\n", optbyte, id, ftell(fp), lastokbyte);
				return false;
				break;
			}
		}

		oType->width  = fgetc(fp);
		oType->height = fgetc(fp);
		if((oType->width > 1) || (oType->height > 1)){
			oType->rendersize = fgetc(fp);
		}

		oType->blendframes = fgetc(fp);
		oType->xdiv        = fgetc(fp);
		oType->ydiv        = fgetc(fp);
		oType->zdiv        = fgetc(fp);
		oType->animcount   = fgetc(fp);

		oType->numsprites = oType->width * oType->height * oType->blendframes * oType->xdiv * oType->ydiv * oType->animcount * oType->zdiv;

		oType->imageData = new uint16_t[oType->numsprites];

        ASSERT(oType->imageData);

		for(unsigned int i = 0; i < oType->numsprites; i++) {
			yatc_fread(&oType->imageData[i], sizeof(uint16_t), 1, fp);
            ECORR16(oType->imageData[i]);
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

// NOTE (nfries88): 740 - 750 loader
bool Objects::load74_75series(const char* filename)
{
	uint16_t id = 100;
	int32_t size;
	uint16_t read_short, read_short2;
	uint32_t maxObjects = 0;

	FILE *fp = yatc_fopen(filename, "rb");
	if(!fp){
		return false;
	}

	fseek(fp,0,SEEK_END);
	size = ftell(fp);

	//get max id
	fseek(fp, 0x04, SEEK_SET);
	//Items
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minItemId = 100;
	ObjectType::maxItemId = read_short;
	maxObjects += ObjectType::maxItemId;
	//Outfits
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minOutfitId = 0;
	ObjectType::maxOutfitId = read_short;
	maxObjects += ObjectType::maxOutfitId;
	//Effects
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minEffectId = 0;
	ObjectType::maxEffectId = read_short;
	maxObjects += ObjectType::maxEffectId;
	//Distance
	yatc_fread(&read_short, 2, 1, fp);
	ECORR16(read_short);
	ObjectType::minDistanceId = 0;
	ObjectType::maxDistanceId = read_short;
	maxObjects += ObjectType::maxDistanceId;

	while(ftell(fp) < size && id <= maxObjects){
		ObjectType* oType = new ObjectType(id);

		int optbyte;
		//bool colorTemplate = false;
		while(((optbyte = fgetc(fp)) >= 0) && (optbyte != 0xFF)){
			switch(optbyte){
				case 0x00: //Ground tile
						yatc_fread(&read_short, 2, 1, fp);
                        ECORR16(read_short);
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
				case 0x03: //Container
						oType->container = true;
					break;
				case 0x04: //Stackable
						oType->stackable = true;
					break;
				case 0x05: //Useable
						oType->useable = true;
					break;
				case 0x06: //Ladders?
						oType->alwaysUsed = true;
					break;
				case 0x07: //Writtable/Readable Objectss
						oType->readable = true;
						yatc_fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
                        ECORR16(read_short);
						break;
				case 0x08: //Writtable Objectss that can't be edited
						oType->readable = true;
						yatc_fread(&read_short2, sizeof(read_short2), 1, fp); //maximum size of text entry TODO (ivucica#3#) store this data
                        ECORR16(read_short);
					break;
				case 0x09: //Fluid containers
						oType->fluidContainer = true;
					break;
				case 0x0A: //Splashes?
						oType->splash = true;
					break;
				case 0x0B: //Is blocking
						oType->blockSolid = true;
					break;
				case 0x0C: //Is not moveable
						oType->moveable = false;
					break;
				case 0x0D: //Blocks missiles (walls, magic wall etc)
						oType->blockProjectile = true;
					break;
				case 0x0E: //Blocks monster movement (flowers, parcels etc)
						oType->blockPathFind = true;
					break;
				case 0x0F: //Can be equipped
						oType->pickupable = true;
					break;
                case 0x10: //Light info
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
						oType->lightLevel = read_short;
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
						oType->lightColor = read_short;
					break;
                case 0x11:  //Floor change?
					break;
                case 0x12: // "Solid Floor" According to BlackDemon
                    break;
                case 0x13:
                    oType->hasHeight = true;
						// (should be) the height change in px; Cipsoft always uses 8
						yatc_fread(&read_short, sizeof(read_short), 1, fp); // ?
                        ECORR16(read_short);
					break;
                case 0x14: // "Adjusted" According to BlackDemon: 8px x and y offset
                    oType->xOffset = 8;
                    oType->yOffset = 8;
                    break;
                case 0x16: // Minimap
						yatc_fread(&read_short, sizeof(read_short), 1, fp);
                        ECORR16(read_short);
						oType->mapColor = read_short;
					break;
                case 0x17: //Rotatable items
                    oType->rotatable = true;
				case 0x18: // "Bottom Layer" according to BlackDemon?
					break;
				case 0x19: //Wall items
                    oType->isHangable = true;
					break;
                case 0x1A:
                    oType->isHorizontal = true;
					break;
				case 0x1B:
                    oType->isVertical = true;
					break;
				case 0x1C://idle animated
                        oType->idleAnim = true;
					break;
				case 0x1D:  //line spot
						int tmp;
						tmp = fgetc(fp); // 86 -> openable holes, 77-> can be used to go down, 76 can be used to go up, 82 -> stairs up, 79 switch,
						if(tmp == 0x58)
							oType->readable = true;

						fgetc(fp); // always 4
					break;
				default:
						optbyte = optbyte;
						std::cout << "unknown byte: " << (uint16_t)optbyte << std::endl;
						return false;
					break;
			}
		}

		oType->width  = fgetc(fp);
		oType->height = fgetc(fp);
		if((oType->width > 1) || (oType->height > 1)){
			oType->rendersize = fgetc(fp);
		}

		oType->blendframes = fgetc(fp);
		oType->xdiv        = fgetc(fp);
		oType->ydiv        = fgetc(fp);
		// NOTE (nfries88): Black Demon says "zdiv" (our "unk1") was not added until 7.55
		oType->zdiv        = 1;
		oType->animcount   = fgetc(fp);

		oType->numsprites = oType->width * oType->height * oType->blendframes * oType->xdiv * oType->ydiv * oType->animcount * oType->zdiv;

		oType->imageData = new uint16_t[oType->numsprites];

        ASSERT(oType->imageData);

		for(unsigned int i = 0; i < oType->numsprites; i++) {
			yatc_fread(&oType->imageData[i], sizeof(uint16_t), 1, fp);
            ECORR16(oType->imageData[i]);
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

void Objects::asJSON(std::ostream &o) {
	o << "{" << std::endl;
	o << "\t\"items\": [" << std::endl;
	bool first = true;
	//for (std::vector<ObjectType*>::iterator it = m_item.begin(); it != m_item.end(); it++) {
	for (int i = 100; i < m_item.size(); i++) {
		ObjectType *oType = m_item.getElement(i);
		if (oType == NULL) continue;

		if (first)
			o << "\t\t{" << std::endl;
		else
			o << "," << std::endl << "\t\t{" << std::endl;
		first = false;

		o << "\t\t\t\"id\": " << oType->id << "," << std::endl;
		o << "\t\t\t\"width\": " << oType->width << "," << std::endl;
		o << "\t\t\t\"height\": " << oType->height << "," << std::endl;
		if((oType->width > 1) || (oType->height > 1)){
			o << "\t\t\t\"renderSize\" : " << oType->rendersize << "," << std::endl;
		}

		o << "\t\t\t\"blendFrames\": " << oType->blendframes << "," << std::endl;
		o << "\t\t\t\"xDiv\": " << oType->xdiv << "," << std::endl;
		o << "\t\t\t\"yDiv\": " << oType->ydiv << "," << std::endl;
		o << "\t\t\t\"zDiv\": " << oType->zdiv << "," << std::endl;
		o << "\t\t\t\"animCount\": " << oType->animcount << "," << std::endl;
		o << "\t\t\t\"numSprites\": " << oType->numsprites << "," << std::endl;
		o << "\t\t\t\"sprites\": [" << std::endl;
		for(unsigned int i = 0; i < oType->numsprites; i++) {
			o << "\t\t\t\t" << oType->imageData[i];
			if (i < oType->numsprites - 1)
				o << ",";
			o << std::endl;
		}
		o << "\t\t\t]" << std::endl;

		//o << "\t\t\t, \"_comment\": \"" << i << " out of " << m_item.size() << "\"" << std::endl;
		o << "\t\t}";
	}
	o << std::endl;
	o << "\t]" << std::endl;
	o << "}" << std::endl;
}
