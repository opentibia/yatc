#include <stdint.h>
#include "gamecontent/globalvars.h"
#include "gamecontent/creature.h"
#include "gamecontent/item.h"
#include "mapui.h"
#include "debugprint.h"
#include "engine.h"

extern uint32_t g_frameTime;
extern Engine* g_engine;


MapUI::MapUI() {
	m_x = 0;
	m_y = 0;


	// tilecount: width and height to render  -- "viewport" width and height
	#ifndef WINCE
	m_vpw=18, m_vph=14;
	#else
	m_vpw=8, m_vph=6;
	#endif


	m_w = m_vpw*32;
	m_h = m_vph*32;
}

MapUI::~MapUI() {
}

void MapUI::renderMap() {

	// FIXME (ivucica#2#) make renderMap use m_x,m_y and m_w,m_h



	static Sprite* ui = NULL; // FIXME (ivucica#4#) move this local ui, GM_Gameworld::ui and GM_MainMenu::ui to Engine::m_ui, since over there it can be global and not reloaded every now and then
	if (ui == NULL) {
		ui = g_engine->createSprite("Tibia.pic", 3);
	}




	// set up scale
	float scale = 1.f;
	float scaledSize = 32*scale;

	// get player position
	Position pos = GlobalVariables::getPlayerPosition();

	// find out how far above the player shall be visible
	int m = getMinZ();
	int sz;
	if(pos.z > 7){ // underground
		sz = pos.z + 3;
	}
	else{
		sz = 7;
	}

	// find out scrolling offset
	float walkoffx = 0.f, walkoffy = 0.f;
	Creature* player = Creatures::getInstance().getPlayer();
	if(player){
		player->getWalkOffset(walkoffx, walkoffy, scale);
	}
	walkoffx *= -1; walkoffy *= -1;

	for(int z = sz; z >= m; z--){

		ASSERT(z >= 0);
		int offset = z - pos.z;

		for(uint32_t i = 0; i < m_vpw; ++i){
			for(uint32_t j = 0; j < m_vph; ++j){

				uint32_t tile_height = 0;

				uint32_t tile_x = pos.x + i - m_vpw/2 - offset;
				uint32_t tile_y = pos.y + j - m_vph/2 - offset;

				const Tile* tile = Map::getInstance().getTile(tile_x, tile_y, z);

				if(!tile){
					//printf("No tile?\n");
					continue;
				}

				int screenx = (int)(i*scaledSize + walkoffx);
				int screeny = (int)(j*scaledSize + walkoffy);

				const Item* ground = tile->getGround();
				if(ground){
					ground->Blit(screenx, screeny, scale, tile_x, tile_y);
				}

				enum drawingStates_t{
					DRAW_ITEMTOP1 = 1,
					DRAW_ITEMTOP2,
					DRAW_ITEMDOWN,
					DRAW_CREATURE,
					DRAW_ITEMTOP3
				};
				drawingStates_t drawState = DRAW_ITEMTOP1;

				int32_t thingsCount = tile->getThingCount() - 1;
				int32_t drawIndex = ground ? 1 : 0, lastTopIndex = 0;
				bool drawnEffects = false;

				while(drawIndex <= thingsCount && drawIndex >= 0){

					const Thing* thing = tile->getThingByStackPos(drawIndex);
					if(thing){
						int32_t thingOrder = thing->getOrder();

						switch(drawState){
						case DRAW_ITEMTOP1: //topItems 1,2
						case DRAW_ITEMTOP2:
							if(thingOrder > 2){
								drawState = DRAW_ITEMDOWN;
								lastTopIndex = drawIndex;
								drawIndex = thingsCount;
								continue;
							}
							break;

						case DRAW_ITEMDOWN: //downItems
							if(thingOrder != 5){
								drawState = DRAW_CREATURE;
								continue;
							}
							break;

						case DRAW_CREATURE: //creatures
							if(thingOrder != 4){
								//Draw effects
								drawTileEffects(const_cast<Tile*>(tile), screenx, screeny, scale, tile_height);
								drawnEffects = true;
								//
								drawState = DRAW_ITEMTOP3;
								drawIndex = lastTopIndex;
								continue;
							}
							break;

						case DRAW_ITEMTOP3: //topItems 3
							if(thingOrder != 3){
								drawIndex = -1; //force exit loop
								continue;
							}
							break;
						}

						thing->Blit(screenx - (int)(tile_height*8.*scale),
									screeny - (int)(tile_height*8.*scale),
									scale, tile_x, tile_y);

						if(const Item* item = thing->getItem()){
							if(item->hasHeight())
								tile_height++;
						}

						switch(drawState){
						case DRAW_ITEMTOP1: //topItems 1,2
						case DRAW_ITEMTOP2:
							drawIndex++;
							break;
						case DRAW_ITEMDOWN: //downItems
						case DRAW_CREATURE: //creatures
							drawIndex--;
							break;
						case DRAW_ITEMTOP3: //topItems 3
							drawIndex++;
							break;
						}

					}
					else{
						DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "Thing invalid %d\n", drawIndex);
						break;
					}
				}
				if(!drawnEffects){
					drawTileEffects(const_cast<Tile*>(tile), screenx, screeny, scale, tile_height);
				}
			}
		}

		//draw animated texts
		{
		Map::AnimatedTextList& aniTexts = Map::getInstance().getAnimatedTexts(z);
		Map::AnimatedTextList::iterator it = aniTexts.begin();
		while(it != aniTexts.end()){
			if((*it).canBeDeleted()){
				aniTexts.erase(it++);
			}
			else{
				const Position& txtpos = (*it).getPosition();

				float textYOffset = (g_frameTime - (*it).getStartTime())/1000.f*0.75f;

				int screenx = (int)((txtpos.x - pos.x + m_vpw/2 + offset + 0.4)*scaledSize + walkoffx);
				int screeny = (int)((txtpos.y - pos.y + m_vph/2 + offset - textYOffset)*scaledSize + walkoffy);

				g_engine->drawText((*it).getText().c_str() , "gamefont", screenx, screeny, (*it).getColor());
				++it;
			}
		}
		}
		//draw distance effects
		{
		Map::DistanceEffectList& distanceEffects = Map::getInstance().getDistanceEffect(z);
		Map::DistanceEffectList::iterator it = distanceEffects.begin();
		while(it != distanceEffects.end()){
			float flightProgress = (*it)->getFlightProgress();

			if(flightProgress > 1.f){
				delete *it;
				distanceEffects.erase(it++);
			}
			else{
				const Position& fromPos = (*it)->getFromPos();
				const Position& toPos = (*it)->getToPos();

				float screenxFrom = ((fromPos.x - pos.x + m_vpw/2 + offset)*scaledSize + walkoffx);
				float screenyFrom = ((fromPos.y - pos.y + m_vph/2 + offset)*scaledSize + walkoffy);

				float screenxTo = ((toPos.x - pos.x + m_vpw/2 + offset)*scaledSize + walkoffx);
				float screenyTo = ((toPos.y - pos.y + m_vph/2 + offset)*scaledSize + walkoffy);

				int screenx = (int)(screenxFrom + (screenxTo - screenxFrom)*flightProgress);
				int screeny = (int)(screenyFrom + (screenyTo - screenyFrom)*flightProgress);

				(*it)->Blit(screenx, screeny, scale);
				++it;
			}
		}
		}
	}

	// draw always-on-top things
	// (currently only creature names)

	int playerspeed = 0;
	for(uint32_t i = 0; i < m_vpw; ++i){
		for(uint32_t j = 0; j < m_vph; ++j){
			const Tile* tile = Map::getInstance().getTile(pos.x + i - m_vpw/2, pos.y + j - m_vph/2, pos.z);
			if(!tile){
				continue;
			}

			int screenx = (int)(i*scaledSize + walkoffx);
			int screeny = (int)(j*scaledSize + walkoffy);
			int groundspeed = tile->getGround() ? Objects::getInstance()->getItemType( tile->getGround()->getID() )->speed : 500;

			int32_t thingsCount = tile->getThingCount() - 1;
			int32_t drawIndex = 1;
			while(drawIndex <= thingsCount){

				Thing* thing = (Thing*)tile->getThingByStackPos(drawIndex); // FIXME (ivucica#3#) getThingByStackPos() should allow changing (should not be returning const)
				if(thing){
					if(thing->getCreature()){

						thing->getCreature()->drawName(screenx, screeny, scale);
						thing->getCreature()->drawSkullsShields(screenx, screeny, scale, ui);
						if(!player || thing->getCreature()->getId() != player->getId())
							thing->getCreature()->advanceWalk(groundspeed);
						else
							playerspeed = groundspeed;
					}
				}
				drawIndex++;
			}

		}
	}

	if(player)
		player->advanceWalk(playerspeed);
}

int MapUI::getMinZ() { // find out how far can we render... if anything is directly above player, then don't render above that floor
	// FIXME (ivucica#2#) minz should be m_minz and thus be cached. code below should be called only in onWalk()
	Position pos = GlobalVariables::getPlayerPosition();
	const Tile* tile = Map::getInstance().getTile(pos.x, pos.y, pos.z);

	int minz = 0;
	for (int z = pos.z-1; z>=0; z--) {
		tile = Map::getInstance().getTile(pos.x-(z-pos.z), pos.y-(z-pos.z), z);
		if (tile && tile->getThingCount() ) {
			minz = z+1;

			return minz;
		}
	}
	return 0;
}

void MapUI::drawTileEffects(Tile* tile, int screenx, int screeny, float scale, uint32_t tile_height)
{
	Tile::EffectList& effects = const_cast<Tile*>(tile)->getEffects();
	Tile::EffectList::iterator it = effects.begin();
	while(it != effects.end()){
		if((*it)->canBeDeleted()){
			delete *it;
			effects.erase(it++);
		}
		else{
			(*it)->Blit(screenx - (int)(tile_height*8.*scale),
					screeny - (int)(tile_height*8.*scale),
					scale, 0, 0);
			++it;
		}
	}
}


void MapUI::useItem(int x, int y, const Thing* &thing, int &retx, int &rety, int &retz, int &stackpos, bool &extended)
{
	// FIXME (ivucica#1#) make this use m_x,m_y and m_w,m_h

	x /= 32; // divide by tile size
	y /= 32; // we need the tile coordinates, not the mouse coordinates

	printf("Click on %d %d\n", x, y);
	printf("Limitx: 0 - %d\n", m_vpw);
	printf("Limity: 0 - %d\n", m_vph);

	if (x < 0 || x > m_vpw) {
		stackpos = -1;
		return;
	}
	if (y < 0 || y > m_vph) {
		stackpos = -1;
		return;
	}

	// get player position
	Position pos = GlobalVariables::getPlayerPosition();

	printf("Click on %d %d\n", x,y);
	printf("That translates into %d %d %d\n", pos.x + x - m_vpw/2, pos.y + y - m_vph/2, pos.z);
	// get the tile we clicked on
	const Tile* tile = Map::getInstance().getTile(pos.x + x - m_vpw/2, pos.y + y - m_vph/2, pos.z);

	// get stackpos of thing that we clicked on
	stackpos = tile->getUseStackpos();


	// get thing that we clicked on
	thing = tile->getThingByStackPos(stackpos);

	retx = pos.x + x - m_vpw/2;
	rety = pos.y + y - m_vph/2;
	retz = pos.z;

	// is this an extended use
	// stub!
	extended = false;
}

void MapUI::attackCreature(int x, int y, const Creature* &creature)
{
	// FIXME (ivucica#1#) make this use m_x,m_y and m_w,m_h

	x /= 32; // divide by tile size
	y /= 32; // we need the tile coordinates, not the mouse coordinates

	if (x < 0 || x > m_vpw) {
		return;
	}
	if (y < 0 || y > m_vph) {
		return;
	}

	// get player position
	Position pos = GlobalVariables::getPlayerPosition();

	printf("Attack on %d %d\n", x,y);
	printf("That translates into %d %d %d\n", pos.x + x - m_vpw/2, pos.y + y - m_vph/2, pos.z);
	// get the tile we clicked on
	const Tile* tile = Map::getInstance().getTile(pos.x + x - m_vpw/2, pos.y + y - m_vph/2, pos.z);

	// get the creature we're attacking
	creature = tile->getTopCreature();
}
