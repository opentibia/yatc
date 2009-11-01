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


#include "battlewindow.h"
#include "../gamecontent/creature.h"

#include "../engine.h"

#include "../gm_gameworld.h"
#include "../net/protocolgame.h"
extern int g_lastmousebutton;

winBattle_t::BattleEntry* winBattle_t::get(uint32_t id)
{
	std::vector<BattleEntry*>::iterator it;
	for(it = entries.begin(); it != entries.end(); ++it)
	{
		BattleEntry* e = (*it);
		if(e->creatureId == id)
		{
			return e;
		}
	}
	return NULL;
}

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
		BattleEntry* e = (*it);
		if(e != NULL && e->creatureId == id)
		{
			int hp = Creatures::getInstance().getCreature(id)->getHealth();
			if(hp == 0)
			{
			    printf("removing object from battlewindow\n");
				list.RemoveObject(&e->pnl);
				list.RebuildList();

				entries.erase(it);
				delete e;
				it = entries.begin();
				continue;
			}
			oRGBA col = CreatureUI::getHealthColor(hp);
			e->pbHealth.SetValue(hp);
			#if (GLICT_APIREV < 105)
            #warning GLICT too old, get 105 or newer for visual improvement on some progressbars
            #else
			e->pbHealth.SetFGColor(glictColor(col.r/255., col.g/255., col.b/255., col.a/255.));
			#endif
		}
	}
}
void winBattle_t::remove(uint32_t id)
{
	std::vector<BattleEntry*>::iterator it;
	for(it = entries.begin(); it != entries.end(); ++it)
	{
		BattleEntry* e = (*it);
		if(e->creatureId == id)
		{
			list.RemoveObject(&e->pnl);
			list.RebuildList();
			entries.erase(it);
			delete e;
			return;
		}
	}
}
void winBattle_t::refreshVisibility()
{
	std::vector<BattleEntry*>::iterator it;
	Position player = GlobalVariables::getPlayerPosition();
	bool need_rebuild = false;

	for(it = entries.begin(); it != entries.end(); ++it)
	{
		Creature *c = Creatures::getInstance().getCreature((*it)->creatureId);
		if (!c) continue;
		Position p = c->getCurrentPos();
		bool visibility = (*it)->pnl.GetVisible();

		if (Map::getInstance().playerCanSee(p.x, p.y, p.z) && p.z == player.z && c->getHealth() > 0) {
			(*it)->pnl.SetVisible(true);
		} else
		{
			(*it)->pnl.SetVisible(false);
		}
		if(visibility != (*it)->pnl.GetVisible())
			need_rebuild = true;
	}
	if(need_rebuild)
		list.RebuildList();
}

void winBattle_t::paintEntry(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	Creature* creature = Creatures::getInstance().getCreature((int)caller->GetCustomData());
	oRGBA col;
	if(creature->getID() == GlobalVariables::getAttackID())
		col = oRGBA(180, 50, 20, 255);
	else
		col = oRGBA(.7*255, .7*255, .7*255, .7*255);
	g_engine->drawText(creature->getName().c_str(), "aafont", (int)real->left+23, (int)real->top, col);
	creature->Blit((int)real->left+4, (int)real->top+4, 20.f/32.f, 0, 0);
}

void winBattle_t::onAttack(Popup::Item* parent)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	gw->m_protocol->sendAttackCreature((uint32_t)parent->data);
	GlobalVariables::setAttackID((uint32_t)parent->data);
}

void winBattle_t::onFollow(Popup::Item* parent)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	gw->m_protocol->sendFollowCreature((uint32_t)parent->data);
	GlobalVariables::setFollowID((uint32_t)parent->data);
}

void winBattle_t::onMessageTo(Popup::Item *parent)
{
	Creature* c = Creatures::getInstance().getCreature((uint32_t)parent->data);
	if(c != NULL)
	{
		GM_Gameworld *gw = (GM_Gameworld*)g_game;
		gw->setActiveConsole(gw->findConsole(c->getName()));
	}
}

void winBattle_t::onUnimplemented(Popup::Item *parent)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
    gw->msgBox(gettext("This functionality is not yet finished"),"TODO");
}

void winBattle_t::makeConsolePopup(Popup* popup, void* owner, void* arg)
{
	BattleEntry* e = (BattleEntry*)arg;
	Creature* c = Creatures::getInstance().getCreature(e->creatureId);

	std::stringstream s;
	s.str("");
	s << gettext("Attack");
	popup->addItem(s.str(), winBattle_t::onAttack, (void*)c->getID());

	s.str("");
	s << gettext("Follow");
	popup->addItem(s.str(), winBattle_t::onFollow, (void*)c->getID());

	if (c->isPlayer() && (c->getCurrentPos().z == GlobalVariables::getPlayerPosition().z))
	{
		popup->addItem("-",NULL,NULL);
		s.str("");
		s << gettext("Message to") << " " << c->getName();
		popup->addItem(s.str(),onMessageTo,(void*)c->getID());

		s.str("");
		s << gettext("Add to VIP list");
		popup->addItem(s.str(),onUnimplemented);

		s.str("");
		s << gettext("Ignore") << " " << c->getName();
		popup->addItem(s.str(),onUnimplemented);

		Creature* player = Creatures::getInstance().getPlayer();
		switch(player->getShield())
		{
			case SHIELD_NONE:
			{
				if(c->getShield() == SHIELD_NONE){
					s.str("");
					s << gettext("Invite to Party");
					popup->addItem(s.str(),onInviteToParty,(void*)c->getID());
				}
				else if(c->getShield() == SHIELD_WHITEYELLOW){
					s.str("");
					s << gettext("Accept") << c->getName() << "'s " << gettext("Invitation");
					popup->addItem(s.str(), onAcceptInvite, (void*)c->getID());
				}
				break;
			}
			case SHIELD_YELLOW: case SHIELD_YELLOW_SHAREDEXP:
			case SHIELD_YELLOW_NOSHAREDEXP_BLINK: case SHIELD_YELLOW_NOSHAREDEXP:
			{
				if(c->getShield() == SHIELD_WHITEBLUE){
					s.str("");
					s << gettext("Revoke") << c->getName() << "'s " << gettext("Invitation");
					popup->addItem(s.str(), onRevokeInvite, (void*)c->getID());
					break;
				}
				else if(c->getShield() == SHIELD_NONE){
					s.str("");
					s << gettext("Invite to Party");
					popup->addItem(s.str(),onInviteToParty,(void*)c->getID());
				}
				else if(player->getShield() != SHIELD_WHITEYELLOW){
					// NOTE (nfries88): Because of location, this will only affect players in party.
					s.str("");
					s << gettext("Pass Leadership To") << " " << c->getName();
					popup->addItem(s.str(), onPassLeadership, (void*)c->getID());
				}
				break;
			}
		}
	}
	popup->addItem("-",NULL,NULL);

	s.str("");
	s << gettext("Copy Name");
	popup->addItem(s.str(), GM_Gameworld::onCopyName, (void*)c->getID());
}

void winBattle_t::onInviteToParty(Popup::Item *parent)
{
    GM_Gameworld *gw = (GM_Gameworld*)g_game;
    gw->m_protocol->sendInviteParty((uint32_t)VOIDP2INT(parent->data));
}
void winBattle_t::onRevokeInvite(Popup::Item *parent)
{
    GM_Gameworld *gw = (GM_Gameworld*)g_game;
    gw->m_protocol->sendCancelInviteParty((uint32_t)VOIDP2INT(parent->data));
}
void winBattle_t::onAcceptInvite(Popup::Item *parent)
{
    GM_Gameworld *gw = (GM_Gameworld*)g_game;
    gw->m_protocol->sendJoinParty((uint32_t)VOIDP2INT(parent->data));
}
void winBattle_t::onPassLeadership(Popup::Item *parent)
{
    GM_Gameworld *gw = (GM_Gameworld*)g_game;
    gw->m_protocol->sendPassPartyLeader((uint32_t)VOIDP2INT(parent->data));
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
			// NOTE (nfries88): too lazy to implement this...
			//gw->lookAtPos(creature->getCurrentPos());
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
		winBattle_t::BattleEntry* e = gw->sbvlPanel.winBattle.get(creature->getID());
		gw->performPopup(winBattle_t::makeConsolePopup, NULL, (void*) e);
	}
}

void winBattle_t::OnClose()
{
	controller->closeBattle();
}
