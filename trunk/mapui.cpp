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

#ifndef _MSC_VER
#include <stdint.h>
#endif
#include "gm_gameworld.h"
#include "popup.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/creature.h"
#include "gamecontent/item.h"
#include "mapui.h"
#include "debugprint.h"
#include "engine.h"
#include "net/protocolgame.h"
#if defined(HAVE_LIBINTL_H)
	#include <libintl.h>
#else
	#define gettext(x) (x)
#endif

extern uint32_t g_frameTime;
extern Engine* g_engine;


MapUI::MapUI()
{
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

MapUI::~MapUI()
{
}

void MapUI::renderMap()
{

	// FIXME (ivucica#2#) make renderMap use m_x,m_y and m_w,m_h

    g_engine->setClipping(64,64,15*32,11*32);

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
					if(ground->hasHeight())
						tile_height++;
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
				bool drawnEffectsGhosts = false;

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
							    //Draw ghosting and effects
							    drawTileGhosts(tile_x, tile_y, z, screenx, screeny, scale, tile_height);
								drawTileEffects(const_cast<Tile*>(tile), screenx, screeny, scale, tile_height);
								drawnEffectsGhosts = true;
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

                        bool performPaint = true;
                        if(const Creature* c = thing->getCreature()){
                            if(c->getWalkState() != 1. || c->isPreWalking()){// it's walking?
                                if (!((c->getLookDir() == DIRECTION_SOUTH && !c->isPreWalking()) ||
                                    (c->getLookDir() == DIRECTION_NORTH && c->isPreWalking()) ||
                                    (c->getLookDir() == DIRECTION_EAST && !c->isPreWalking()) ||
                                    (c->getLookDir() == DIRECTION_WEST && c->isPreWalking())))
                                    performPaint = false;
                            }
                        }

						if (performPaint)
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
				if(!drawnEffectsGhosts){
				    drawTileGhosts(tile_x, tile_y, z, screenx, screeny, scale, tile_height);
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
			int groundspeed = tile->getSpeedIndex();

			int32_t thingsCount = tile->getThingCount() - 1;
			int32_t drawIndex = 1;
			while(drawIndex <= thingsCount){

				Thing* thing = (Thing*)tile->getThingByStackPos(drawIndex); // FIXME (ivucica#3#) getThingByStackPos() should allow changing (should not be returning const)
				if(thing){
					if(thing->getCreature()){

						thing->getCreature()->drawName(screenx, screeny, scale);
						thing->getCreature()->drawSkullsShields(screenx, screeny, scale);
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

	g_engine->resetClipping();

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

void MapUI::drawTileGhosts(int x, int y, int z, int screenx, int screeny, float scale, uint32_t tile_height)
{
    // go through all tiles around it and see if any of the creatures is moving in direction of this tile
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            if (!(i || j)) continue;
            int tile_x = x+i;
            int tile_y = y+j;
            const Tile* tile = Map::getInstance().getTile(tile_x, tile_y, z);
            if (!tile) continue;
            int32_t thingsCount = tile->getThingCount() - 1;
            for (int k = 1; k < thingsCount+1; k++) {
                const Creature* c = tile->getThingByStackPos(k)->getCreature();

                int screenx2=screenx, screeny2=screeny;

                if (c && (c->getWalkState() != 1. || c->isPreWalking())) {
                    if (c->getLookDir() == DIRECTION_SOUTH && !c->isPreWalking()) continue;
                    if (c->getLookDir() == DIRECTION_NORTH && c->isPreWalking()) continue;
                    if (c->getLookDir() == DIRECTION_EAST && !c->isPreWalking()) continue;
                    if (c->getLookDir() == DIRECTION_WEST && c->isPreWalking()) continue;
                    switch (c->getLookDir()) {

                        case DIRECTION_NORTH:
                        case DIRECTION_SOUTH:
                            if (j != -1 || i != 0) continue;
                            tile_y -= 1;
                            screeny2 -= 32;
                            break;
                        case DIRECTION_WEST:
                        case DIRECTION_EAST:
                            if (i != -1 || j != 0) continue;
                            tile_x -= 1;
                            screenx2 -= 32;
                            break;


                        default:
                            continue;
                    }
                    c->Blit(screenx2 - (int)(tile_height*8.*scale),
                            screeny2 - (int)(tile_height*8.*scale),
                            scale, tile_x, tile_y);
                }
            }
        }

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


void MapUI::useItem(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos, bool &extended)
{
	Tile* tile = translateClickToTile(x,y,retx,rety,retz);

    useItem(tile,thing,stackpos,extended);
}

void MapUI::useItem(Tile* tile, const Thing* &thing, int &stackpos, bool &extended)
{
    if (!tile) {
        thing = NULL;
        stackpos = -1;
        extended = false;
        return;
    }
	// get stackpos of thing that we clicked on
	stackpos = tile->getUseStackpos();


	// get thing that we clicked on
	thing = tile->getThingByStackPos(stackpos);

	// is this an extended use
	if (thing->getItem()->isExtendedUseable())
        extended = true;
    else
        extended = false;
}


void MapUI::attackCreature(int x, int y, const Creature* &creature)
{
	// get the creature we're attacking from the tile we're clicking on

	if(Tile* t=translateClickToTile(x,y)){
        creature = t->getTopCreature();
	} else {
	    creature = NULL;
	}
}


void MapUI::lookAtItem(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos)
{
    Position p;
	Tile* tile = translateClickToTile(x,y,retx,rety,retz);

    if(!tile){
        thing = NULL;
        stackpos = -1;
        return;
    }
	// get stackpos of thing that we clicked on
	stackpos = tile->getUseStackpos();

	// get thing that we clicked on
    thing = tile->getThingByStackPos(stackpos);

}


void MapUI::dragThing(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos)
{
    Position p;
	Tile* tile = translateClickToTile(x,y,retx,rety,retz);

    if(!tile){
        thing = NULL;
        stackpos = -1;
        return;
    }
	// get stackpos of thing that we clicked on
	stackpos = tile->getThingCount()-1;

	// get thing that we clicked on
    thing = tile->getThingByStackPos(stackpos);

}

Tile* MapUI::translateClickToTile(int x, int y, uint32_t &retx, uint32_t &rety, uint32_t &retz)
{
    // FIXME (ivucica#1#) make this use m_x,m_y and m_w,m_h

	x /= 32; // divide by tile size
	y /= 32; // we need the tile coordinates, not the mouse coordinates

	printf("Click on %d %d\n", x, y);
	printf("Limitx: 0 - %d\n", m_vpw);
	printf("Limity: 0 - %d\n", m_vph);

	if (x < 0 || x > m_vpw) {
		return NULL;
	}
	if (y < 0 || y > m_vph) {
		return NULL;
	}

	// get player position
	Position pos = GlobalVariables::getPlayerPosition();

	printf("Click on %d %d\n", x,y);
	printf("That translates into %d %d %d\n", pos.x + x - m_vpw/2, pos.y + y - m_vph/2, pos.z);
	// get the tile we clicked on
	Tile* tile = Map::getInstance().getTile(pos.x + x - m_vpw/2, pos.y + y - m_vph/2, pos.z);



	retx = pos.x + x - m_vpw/2;
	rety = pos.y + y - m_vph/2;
	retz = pos.z;
	return tile;
}

bool MapUI::handlePopup(int x, int y)
{
    if (x > 64 && y > 64 && x < 64+15*32 && y < 64+11*32) { // click within visible area of map?
        glictPos p;
        p.x = x; p.y = y;
        ((GM_Gameworld*)g_game)->performPopup(MapUI::makePopup,this,&p);
    }
	return true;
}

void MapUI::makePopup(Popup*popup,void*owner,void*arg)
{
    MapUI *m = (MapUI*)(owner);
    glictPos &pos = *((glictPos*)(arg));
    uint32_t retx, rety, retz;
	Position playerpos = GlobalVariables::getPlayerPosition();


    Tile* t = m->translateClickToTile(pos.x, pos.y, retx, rety, retz);
    m->m_lastRightclickTilePos.x = retx;
    m->m_lastRightclickTilePos.y = rety;
    m->m_lastRightclickTilePos.z = retz;


    // TODO (ivucica#5#): if clicked on dark, right click must say "you cant see anything"
    // but first we must implement lighting at all ... :)

    std::stringstream s;

    // look at appears always
    s.str("");
    s << gettext("Look") << " (Shift)";
    popup->addItem(s.str(), MapUI::onLookAt, m);

    // use depends on kind of item
    Item* item = dynamic_cast<Item*>(t->getThingByStackPos(t->getUseStackpos()));
    s.str("");
    if (item)
    {
        if (item->isExtendedUseable())
            s << gettext("Use with...") << " (Ctrl)";
        else
            if (Objects::getInstance()->getItemType(item->getID())->container)
                s << gettext("Open") << " (Ctrl)";
            else
                s << gettext("Use") << " (Ctrl)";
        popup->addItem(s.str(), MapUI::onUse, m);
    }



    if (retx == playerpos.x && rety == playerpos.y && retz == playerpos.z)
    {
        popup->addItem("-",NULL,NULL);

        s.str("");
        s << gettext("Set Outfit") << "...";
        popup->addItem(s.str(), GM_Gameworld::onSetOutfit);
    } else
    // attack depends if there is a creature and we're NOT on player pos
    if (const Creature *c = t->getTopCreature())
    {
        popup->addItem("-",NULL,NULL);


        s.str("");
        s << gettext("Attack") << " (Alt)";
        popup->addItem(s.str(),onAttack,m);
        m->m_popupCreatureID = c->getID();

        s.str("");
        s << gettext("Follow");
        popup->addItem(s.str(),onFollow,m);

        if (!c->isMonster() && !c->isNpc())
        {
            popup->addItem("-",NULL,NULL);

            s.str("");
            s << gettext("Message to") << " " << c->getName();
            popup->addItem(s.str(),onUnimplemented);

            s.str("");
            s << gettext("Add to VIP list");
            popup->addItem(s.str(),onUnimplemented);

            s.str("");
            s << gettext("Ignore") << " " << c->getName();
            popup->addItem(s.str(),onUnimplemented);

            s.str("");
            s << gettext("Invite to Party");
            popup->addItem(s.str(),onUnimplemented);

        }

        popup->addItem("-",NULL,NULL);
        s.str("");
        s << gettext("Copy Name");
        popup->addItem(s.str(),onUnimplemented);

    }



}


void MapUI::onLookAt(PopupItem *parent)
{
    onUnimplemented(parent);
}
void MapUI::onUse(PopupItem *parent)
{
    MapUI *m = (MapUI*)(parent->data);
    GM_Gameworld *gw = (GM_Gameworld*)g_game;
    Tile* t = Map::getInstance().getTile(m->m_lastRightclickTilePos);

    const Thing* thing;
    int stackpos;
    bool isextended;

    m->useItem(t, thing, stackpos, isextended);

    if(stackpos != -1){
        if(!isextended){
            gw->m_protocol->sendUseItem(t->getPos(), thing->getID(), stackpos );
        } else {
            gw->beginExtendedUse(thing,stackpos,t->getPos());
        }
    }


}

void MapUI::onAttack(PopupItem *parent)
{
    MapUI *m = (MapUI*)(parent->data);
    GM_Gameworld *gw = (GM_Gameworld*)g_game;

    gw->m_protocol->sendAttackCreature(m->m_popupCreatureID);
    GlobalVariables::setAttackID(m->m_popupCreatureID);
}
void MapUI::onFollow(PopupItem *parent)
{
    MapUI *m = (MapUI*)(parent->data);
    GM_Gameworld *gw = (GM_Gameworld*)g_game;

    gw->m_protocol->sendFollowCreature(m->m_popupCreatureID);
    GlobalVariables::setFollowID(m->m_popupCreatureID);
}


void MapUI::onUnimplemented(PopupItem *parent)
{
    MapUI *m = (MapUI*)(parent->data);
    GM_Gameworld *gw = (GM_Gameworld*)g_game;

    gw->msgBox(gettext("This functionality is not yet finished"),"TODO");
}
