//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Creature instance class
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

// TODO (ivucica#2#) review code added in 2739 which was written by mrauter, it's probably not 100% the way we want to do things, plus we need colorization

#include "../creatureui.h"
#include "../fassert.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/globalvars.h"
#include <cmath>

extern uint32_t g_frameTime;
extern uint32_t g_frameDiff;

CreatureUI::CreatureUI() : ThingUI()
{
	resetSelf();
}

CreatureUI::~CreatureUI()
{
	this->unloadGfx();
	m_obj = NULL;
}

void CreatureUI::resetSelf()
{
	unloadGfx();

	m_preWalk = false;
	m_walkState = 1.f;
}
void CreatureUI::unloadGfx()
{
	m_gfx.clear();
	m_obj = NULL;
}

void CreatureUI::Blit(int x, int y, float scale, int map_x, int map_y) const
{
}

void CreatureUI::drawInfo(int x, int y, float scale, bool visible) const
{
}

void CreatureUI::drawName(int x, int y, float scale, bool visible) const
{
}

void CreatureUI::drawSkullsShields(int x, int y, float scale) const
{
}

oRGBA CreatureUI::getHealthColor(int hp)
{
    oRGBA col;
    if (hp > 92.0) {
        col = oRGBA(0., 188., 0., 255.);
    }
    else if (hp > 60.0) {
        col = oRGBA(80., 161., 80., 255.);
    }
    else if (hp > 30.0) {
        col = oRGBA(161., 161., 0., 255.);
    }
    else if (hp > 8.0) {
        col = oRGBA(160., 39., 39., 255.);
    }
    else if (hp > 3.0) {
        col = oRGBA(160., 0., 0., 255.);
    }
    else {
        col = oRGBA(79., 0., 0., 255.);
    }
    return col;
}

void CreatureUI::getWalkOffset(float &walkoffx, float &walkoffy, float scale) const
{
	Creature* n = (Creature*)this;
	float tilesize = 32*scale;
	switch(n->getLookDir()) {
		case DIRECTION_WEST:
			walkoffx = -m_walkState * tilesize + (m_preWalk ? 0 : tilesize);
			break;
		case DIRECTION_EAST:
			walkoffx = m_walkState * tilesize - (m_preWalk ? 0 : tilesize);
			break;
		case DIRECTION_NORTH:
			walkoffy = -m_walkState * tilesize + (m_preWalk ? 0 : tilesize);
			break;
		case DIRECTION_SOUTH:
			walkoffy = m_walkState * tilesize - (m_preWalk ? 0 : tilesize);
			break;
		case DIRECTION_NE:
			walkoffx = m_walkState * tilesize - (m_preWalk ? 0 : tilesize);
			walkoffy = -m_walkState * tilesize + (m_preWalk ? 0 : tilesize);
			break;
		case DIRECTION_SE:
			walkoffx = m_walkState * tilesize - (m_preWalk ? 0 : tilesize);
			walkoffy = m_walkState * tilesize - (m_preWalk ? 0 : tilesize);
			break;
		case DIRECTION_NW:
			walkoffx = -m_walkState * tilesize + (m_preWalk ? 0 : tilesize);
			walkoffy = -m_walkState * tilesize + (m_preWalk ? 0 : tilesize);
			break;
		case DIRECTION_SW:
			walkoffx = -m_walkState * tilesize + (m_preWalk ? 0 : tilesize);
			walkoffy = m_walkState * tilesize - (m_preWalk ? 0 : tilesize);
			break;
	}
}
void CreatureUI::advanceWalk(int groundspeed)
{
	Creature *n = (Creature*)this;
	if (m_walkState >= 1) {
		m_walkState = 1;
		return;
	}

	m_walkState += (n->getSpeed() / (float)groundspeed) * (g_frameDiff/1000.);
	if (m_walkState >= 1)
		m_walkState = 1;
}

bool CreatureUI::loadOutfit()
{
	return true;
}

bool CreatureUI::isLoaded() const {
  return true;
}

bool CreatureUI::setupObject() {
	return true;
}
