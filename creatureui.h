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

#ifndef __CREATUREUI_H
#define __CREATUREUI_H

#include "thingui.h"
#include "objects.h"
class CreatureUI : virtual public ThingUI {
public:

	CreatureUI();
	~CreatureUI();
	virtual void Blit(int x, int y, float scale = 1., int map_x = 0, int map_y = 0) const;

	void loadOutfit();
	void draw(int x, int y, float scale) const;
	void drawName(int x, int y, float scale) const;
	void drawSkullsShields(int x, int y, float scale) const;
	void startWalk() { m_preWalk = true; m_walkState = 0.f; }
	void cancelWalk() { m_preWalk = false; m_walkState = 1.f; }
	void confirmWalk() { m_preWalk = false; }
	void advanceWalk(int groundspeed);
	void getWalkOffset(float &walkoffx, float &walkoffy, float scale=1.f) const;
	bool isPreWalking() const {return m_preWalk;}
	float getWalkState() const {return m_walkState;}
	bool isLoaded() const;
	void resetSelf();

	void unloadGfx();

	static oRGBA getHealthColor(int hp);
protected:
	void setupObject();

private:
	ObjectType *m_obj;

	std::vector<Sprite*> m_gfx;

	bool m_preWalk;
	float m_walkState;

};

#endif
