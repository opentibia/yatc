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

#include "../skin.h"
#include "sbvlpanel.h"
#include "../gm_gameworld.h"
#include "../net/protocolgame.h"


void sbvlPanel_t::onClick_Logout(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;
	gameclass->m_protocol->sendLogout();
}


void sbvlPanel_t::btnSkills_onClick(glictPos* relmousepos, glictContainer* callerclass)
{
	glictButton* btn = (glictButton*)callerclass;
	sbvlPanel_t* panel = (sbvlPanel_t*)btn->GetCustomData();
	if(panel->winSkills.window.GetVisible()) {
		panel->closeSkills();
	}
	else {
		panel->openSkills();
	}
}

void sbvlPanel_t::btnBattle_onClick(glictPos* relmousepos, glictContainer* callerclass)
{
	glictButton* btn = (glictButton*)callerclass;
	sbvlPanel_t* panel = (sbvlPanel_t*)btn->GetCustomData();
	if(panel->winBattle.window.GetVisible()) {
		panel->closeBattle();
	}
	else {
		panel->openBattle();
	}
}

void sbvlPanel_t::btnVIP_onClick(glictPos* relmousepos, glictContainer* callerclass)
{
	glictButton* btn = (glictButton*)callerclass;
	sbvlPanel_t* panel = (sbvlPanel_t*)btn->GetCustomData();
	if(panel->winVIP.window.GetVisible()) {
		panel->closeVIP();
	}
	else {
		panel->openVIP();
	}
}


void sbvlPanel_t::openSkills()
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;

	winSkills.window.SetVisible(true);
	btnSkills.SetHold(true);

	glictList* parentlist = dynamic_cast<glictList*>(winSkills.window.GetParent());
    if (parentlist)
        parentlist->RebuildList();

	gameclass->updateRightSide();

	options.skillsh = winSkills.window.GetHeight();
	options.Save();
}
void sbvlPanel_t::closeSkills()
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;

	winSkills.window.SetVisible(false);
	btnSkills.SetHold(false);

	glictList* parentlist = dynamic_cast<glictList*>(winSkills.window.GetParent());
    if (parentlist)
        parentlist->RebuildList();

	gameclass->updateRightSide();

	options.skillsh = -1;
	options.Save();
}
void sbvlPanel_t::openBattle()
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;

	winBattle.window.SetVisible(true);
	btnBattle.SetHold(true);

	glictList* parentlist = dynamic_cast<glictList*>(winBattle.window.GetParent());
    if (parentlist)
        parentlist->RebuildList();

	gameclass->updateRightSide();

	options.battleh = winBattle.window.GetHeight();
	options.Save();
}
void sbvlPanel_t::closeBattle()
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;

	winBattle.window.SetVisible(false);
	btnBattle.SetHold(false);

	glictList* parentlist = dynamic_cast<glictList*>(winBattle.window.GetParent());
    if (parentlist)
        parentlist->RebuildList();

	gameclass->updateRightSide();

	options.battleh = -1;
	options.Save();
}
void sbvlPanel_t::openVIP()
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;

	winVIP.window.SetVisible(true);
	btnVIP.SetHold(true);

	glictList* parentlist = dynamic_cast<glictList*>(winVIP.window.GetParent());
    if (parentlist)
        parentlist->RebuildList();

	gameclass->updateRightSide();

	options.viph = winVIP.window.GetHeight();
	options.Save();
}
void sbvlPanel_t::closeVIP()
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;

	winVIP.window.SetVisible(false);
	btnVIP.SetHold(false);

	glictList* parentlist = dynamic_cast<glictList*>(winVIP.window.GetParent());
    if (parentlist)
        parentlist->RebuildList();

	gameclass->updateRightSide();

	options.viph = -1;
	options.Save();
}

void winSkills_t::OnClose()
{
	controller->closeSkills();
}
void winBattle_t::OnClose()
{
	controller->closeBattle();
}
void winVIP_t::OnClose()
{
	controller->closeVIP();
}
