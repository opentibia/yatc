//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Base gamemode class
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

#include <GLICT/messagebox.h>
#include "gamemode.h"
GameMode* g_game = NULL;


/* ****** gamemode ******* */

void GameMode::centerWindow (glictWindow *win)
{
	glictSize s;
	win->GetSize(&s);
	win->SetPos(glictGlobals.w / 2 - s.w / 2, glictGlobals.h/2 - s.h / 2);
}

void GameMode::MBOnDismiss(glictPos* pos, glictContainer* caller)
{
	if (caller->GetCustomData()) {
		glictContainer* focusOnDismiss = (glictContainer*)caller->GetCustomData();
		focusOnDismiss->SetVisible(true);
		focusOnDismiss->Focus(NULL);
	}

	g_game->renderScene();
}

void GameMode::msgBox (const char* mbox, const char* title, glictContainer* focusondismiss)
{
	glictSize s;
	glictMessageBox *mb;
	desktop.AddObject(mb = new glictMessageBox);

	mb->SetCaption(title);
	mb->SetMessage(mbox);

	mb->SetHeight(glictFontNumberOfLines(mbox)*12 + 35 + 10 + 10);
	int size1 = (int)glictFontSize(title, "system");
	int size2 = (int)glictFontSize(mbox, "system");
	mb->SetWidth(MAX(size1, size2) + 10 + 10);
	mb->Focus(NULL);
    #if (GLICT_APIREV >= 85)
	mb->SetTextOffset(10,10);
	#else
	#warning For nicer msgboxes get GLICT APIREV 85+.
	#endif

	mb->GetSize(&s);

	mb->SetPos(glictGlobals.w / 2 - s.w / 2, glictGlobals.h/ 2 - s.h / 2);

	mb->SetOnDismiss(MBOnDismiss);

	mb->SetCustomData(focusondismiss);

}


/* ****** gamemode with options ***** */

GameModeOptions::GameModeOptions()
{
    winOptions.initiateAll(&desktop);
}

void GameModeOptions::winOptions_btnNetwork_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	winOptions_t* winOptions = g_game->getOptionsWindow();
	winOptions->winOptionsNetwork.Init();
	winOptions->winOptionsNetwork.window.SetVisible(true);
	winOptions->winOptionsNetwork.btnOk.Focus(NULL);
}
void GameModeOptions::winOptionsNetwork_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	winOptions_t* winOptions = g_game->getOptionsWindow();
	winOptions->winOptionsNetwork.Store();
	//((*GM_MainMenu)g_game)->desktop.SetFocus(NULL);
	winOptions->winOptionsNetwork.window.SetVisible(false);
	winOptions->btnOk.Focus(NULL);
}
void GameModeOptions::winOptionsNetwork_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	winOptions_t* winOptions = g_game->getOptionsWindow();
	//((*GM_MainMenu)g_game)->desktop.SetFocus(NULL);
	winOptions->winOptionsNetwork.window.SetVisible(false);
	winOptions->btnOk.Focus(NULL);
}

void GameModeOptions::doResize(float w, float h)
{
    centerWindow(&winOptions.window);
	centerWindow(&winOptions.winOptionsGeneral.window);
	centerWindow(&winOptions.winOptionsConsole.window);
	centerWindow(&winOptions.winOptionsGraphics.window);
	centerWindow(&winOptions.winOptionsGraphicsAdvanced.window);
	centerWindow(&winOptions.winOptionsHotkeys.window);
    centerWindow(&winOptions.winOptionsNetwork.window);
}
