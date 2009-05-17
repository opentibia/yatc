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

// BIG OLD TODO
#include "battlewindow.h"
#include "../gamecontent/creature.h"

#include "../engine.h"

#include "../gm_gameworld.h"
#include "../net/protocolgame.h"
extern int g_lastmousebutton;

void winBattle_t::add(uint32_t id)
{
	BattleEntry* entry = new BattleEntry;
	entry->creatureId = id;
	entry->pnl.SetHeight(25);
	entry->pnl.SetWidth(150);
	entry->pnl.SetBGActiveness(false);
	entry->pnl.SetCustomData((void*)id);
	entry->pnl.SetOnClick(winBattle_t::clickEntry);
	entry->pnl.SetOnPaint(winBattle_t::paintEntry);
	entry->pnl.SetVisible(true);
	entry->pnl.AddObject(&entry->pbHealth);
	entry->pbHealth.SetPos(23, 14);
	entry->pbHealth.SetWidth(122);
	entry->pbHealth.SetHeight(5);
	#if (GLICT_APIREV < 105)
	#warning GLICT too old, get 105 or newer for visual improvement on some progressbars
	#else
	entry->pbHealth.SetBGActiveness(false);
	entry->pbHealth.SetBorderColor(glictColor(0,0,0,1));
	#endif
	entry->pbHealth.SetValue(Creatures::getInstance().getCreature(id)->getHealth());
	entry->pbHealth.SetVisible(true);
	entries.push_back(entry);
	// now to update...
	list.AddObject(&entry->pnl);
	list.RebuildList();
	container.SetVirtualSize(container.GetWidth(), list.GetHeight());
}
void winBattle_t::update(uint32_t id)
{
	std::vector<BattleEntry*>::iterator it;
	for(it = entries.begin(); it != entries.end(); ++it)
	{
		if((*it)->creatureId == id)
		{
			int hp = Creatures::getInstance().getCreature(id)->getHealth();
			oRGBA col = CreatureUI::getHealthColor(hp);
			(*it)->pbHealth.SetValue(hp);
			#if (GLICT_APIREV < 105)
            #warning GLICT too old, get 105 or newer for visual improvement on some progressbars
            #else
			(*it)->pbHealth.SetFGColor(glictColor(col.r/255., col.g/255., col.b/255., col.a/255.));
			#endif
		}
	}
}
void winBattle_t::remove(uint32_t id)
{
	std::vector<BattleEntry*>::iterator it;
	for(it = entries.begin(); it != entries.end(); ++it)
	{
		if((*it)->creatureId == id)
		{
			delete (*it);
			entries.erase(it);
		}
	}
}
void winBattle_t::refreshVisibility()
{
	std::vector<BattleEntry*>::iterator it;
	Position player = GlobalVariables::getPlayerPosition();

	for(it = entries.begin(); it != entries.end(); ++it)
	{
		Creature *c = Creatures::getInstance().getCreature((*it)->creatureId);
		if (!c) continue;
		Position p = c->getCurrentPos();
		
		// FIXME (ivucica#5#): we should somehow query MapUI if the other creature is in visible area... concluding it ourselves is not a good way of handling stuff
		if (abs(int(p.x - player.x)) < 9 &&
			abs(int(p.y - player.y)) < 7 &&
			p.z == player.z &&
			c->getHealth() > 0) {
			(*it)->pnl.SetVisible(true);
		} else
		{
			(*it)->pnl.SetVisible(false);
		}
	}
	list.RebuildList();
	
}

void winBattle_t::paintEntry(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	Creature* creature = Creatures::getInstance().getCreature((int)caller->GetCustomData());
	int hp = creature->getHealth();
	oRGBA col;
	if(creature->getID() == GlobalVariables::getAttackID())
		col = oRGBA(180, 50, 20, 255);
	else
		col = oRGBA(.7*255, .7*255, .7*255, .7*255);
	g_engine->drawText(creature->getName().c_str(), "aafont", (int)real->left+23, (int)real->top, col);
	creature->Blit((int)real->left+4, (int)real->top+4, 20.f/32.f, 0, 0);
}
void winBattle_t::clickEntry(glictPos* relmousepos, glictContainer* callerclass)
{
	Creature* creature = Creatures::getInstance().getCreature((int)callerclass->GetCustomData());
	GM_Gameworld* gw = ((GM_Gameworld*)g_game);

	if(g_lastmousebutton == SDL_BUTTON_LEFT){
		if(gw->isExtendedUsing())
		{
			gw->performExtendedUseBattleWindow((int)callerclass->GetCustomData());
		}
		else if(SDL_GetModState() & KMOD_SHIFT)
		{
			// NOTE (nfries88): no idea how to get a creature's position on the map.
			//gw->lookAtCreature(creature->getID());
		}
		else if(creature->getID() != GlobalVariables::getPlayerID())
		{
			if(creature->getID() == GlobalVariables::getAttackID()) {
				gw->m_protocol->sendAttackCreature(0);
			}
			else {
				gw->m_protocol->sendAttackCreature(creature->getID());
				GlobalVariables::setAttackID(creature->getID());
			}
		}
	}
	else
	{
		// TODO (nfries88): popup
	}
}
