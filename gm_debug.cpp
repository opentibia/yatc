//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Debugging gamemode
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

#include <GLICT/globals.h>
#include <GLICT/messagebox.h>

#include "gm_debug.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"
#include "options.h"
#include "itemui.h"
#include "util.h"

#include "net/connection.h"
#include "net/protocollogin.h"
#include "net/protocolgame80.h"

extern Connection* g_connection;

#include "gamecontent/container.h"
#include "gamecontent/creature.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/inventory.h"
#include "gamecontent/map.h"

#include "gamecontent/item.h"

#include "product.h"


extern bool g_running;

void cb1() {
    ((GM_Debug*)g_game)->msgBox("You have clicked on the \"Hello world\" menu item", "Yipee");
}

void cb2() {
    ((GM_Debug*)g_game)->msgBox("You have clicked on the \"Noob\" menu item", "Yipee");
}

void GM_Debug::ButtonOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	//ProtocolConfig::getInstance().setServerType(SERVER_OTSERV);
	//ProtocolConfig::getInstance().setServer("localhost", 7172);
	//ProtocolConfig::createLoginConnection(1000, "test");

	GM_Debug *gd = (GM_Debug*)g_game;

    if (!gd->popup) {
        gd->popup = new Popup;
        glictList* gl = gd->popup->getGlictList();

        gl->SetPos(callerclass->GetX() + relmousepos->x, callerclass->GetY() + relmousepos->y);
        gd->desktop.AddObject(gl);
        gd->popup->addItem("Hello world", cb1);
        gd->popup->addItem("Noob", cb2);
    }

}

void GM_Debug::ExitOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	g_running = false;
}

void GM_Debug::UpdateOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_Debug *gd = (GM_Debug*)g_game;
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Deleting spr\n");
	delete gd->spr;
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Deleting thing\n");
	delete gd->thing;
        DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Creating spr\n");

	gd->spr = g_engine->createSprite("Tibia.spr", atoi(gd->txtSprite.GetCaption().c_str()));
        DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Creating thing\n");
	gd->thing = Item::CreateItem(atoi(gd->txtItem.GetCaption().c_str()), 1);
        DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "GREATNESS!\n");

}

GM_Debug::GM_Debug()
{

	desktop.SetWidth(options.w);
	desktop.SetHeight(options.h);
	desktop.ResetTransformations();

	desktop.AddObject(&btnButton);
	btnButton.SetPos(100,100);
	btnButton.SetWidth(128);
	btnButton.SetHeight(16);
	btnButton.SetCaption("Button");
	btnButton.SetBGColor(.75,.75,.75,1);
	btnButton.SetOnClick(GM_Debug::ButtonOnClick);

	desktop.AddObject(&btnExit);
	btnExit.SetPos(100,116);
	btnExit.SetWidth(128);
	btnExit.SetHeight(16);
	btnExit.SetCaption("Exit");
	btnExit.SetBGColor(1,0,0,1);
	btnExit.SetOnClick(GM_Debug::ExitOnClick);

	desktop.AddObject(&txtSprite);
	txtSprite.SetPos(100,132);
	txtSprite.SetWidth(128);
	txtSprite.SetHeight(16);
	txtSprite.SetCaption("200");

	desktop.AddObject(&txtItem);
	txtItem.SetPos(100,148);
	txtItem.SetWidth(128);
	txtItem.SetHeight(16);
	txtItem.SetCaption("100");

	desktop.AddObject(&btnUpdate);
	btnUpdate.SetPos(100,164);
	btnUpdate.SetWidth(128);
	btnUpdate.SetHeight(16);
	btnUpdate.SetCaption("Update gfx");
	btnUpdate.SetBGColor(1,0,0,1);
	btnUpdate.SetOnClick(GM_Debug::UpdateOnClick);

    popup = NULL;
    killpopup = false;

	if(g_engine){
		background = g_engine->createSprite("Tibia.pic", 0);
		spr = g_engine->createSprite("Tibia.spr", 200);
		//thing = new ItemUI(6401, 1);
		thing = NULL;

		//background->addColor(.5, 1., 1.);
	}
	else{  // i think that if g_engine does not exist, we might as well crash. what do you think, guys? ivucica
		NativeGUIError("Somehow, engine managed to not initialize.", PRODUCTSHORT " Fatal Error");
		exit(1);
	}
}

GM_Debug::~GM_Debug()
{
	delete background;
	delete spr;
	delete thing;
}
void GM_Debug::updateScene()
{
	renderScene();
	g_engine->resetClipping();
	desktop.DelayedRemove();
	if (killpopup) {
        delete popup;
        popup = NULL;
        killpopup = false;
        renderScene();
	}
}
void GM_Debug::renderScene()
{
	if(background)
		background->Blit(0,0,0,0,background->getWidth(),background->getHeight(),glictGlobals.w, glictGlobals.h);
	if(spr)
		spr->Blit(50,50);
	if(thing)
		thing->Blit(100,50);
/*
    std::stringstream testchar;
    testchar << (char)('u'+32);

    for (int i = 0; i < 255; i++) {
        g_engine->drawText(testchar.str().c_str(), "system", 0+(i%8)*12,300+(i/8)*12, i);
    }
*/
	desktop.RememberTransformations();
	desktop.Paint();

}

void GM_Debug::mouseEvent(SDL_Event& event)
{
	glictPos pos;
	pos.x = ptrx;
	pos.y = ptry;

	desktop.TransformScreenCoords(&pos);

    if (event.type != SDL_MOUSEMOTION)
        DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Casting click on %g %g (%d %d)\n", pos.x, pos.y, ptrx, ptry);

    bool hadpopup = false;
    if (popup) {
        hadpopup = true;
        glictList *gl = popup->getGlictList();
        if (event.type == SDL_MOUSEMOTION) {
            popup->mouseOver(pos.x, pos.y);
            printf("Done mouseover\n");
            return;
        } else
        if (pos.x < gl->GetX() || pos.y < gl->GetY() ||
            pos.x > gl->GetX() + gl->GetWidth() || pos.y > gl->GetY() + gl->GetHeight()) {
                printf("%g %g not between %g,%g and %g %g\n",
                pos.x, pos.y,
                gl->GetX(), gl->GetY(), gl->GetX() + gl->GetWidth(),gl->GetY() + gl->GetHeight()
                );
                if (!killpopup && event.type == SDL_MOUSEBUTTONUP) {
                    desktop.RemoveObject(gl);
                    killpopup = true;
                }
                return;
        } else {
            glictList *gl = popup->getGlictList();

            if (event.button.state == SDL_PRESSED)
                gl->CastEvent(GLICT_MOUSEDOWN, &pos, 0, NULL);
            if (event.button.state != SDL_PRESSED)
                gl->CastEvent(GLICT_MOUSEUP, &pos, 0, NULL);



            if (!killpopup && event.type == SDL_MOUSEBUTTONUP) {
                glictList *gl = popup->getGlictList();

                desktop.RemoveObject(gl);
                killpopup = true;
            }

            return;
        }

    }

    if (event.type == SDL_MOUSEMOTION)
        return;

    if (event.button.state == SDL_PRESSED)
        desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
    if (event.button.state != SDL_PRESSED)
        desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);

    if (hadpopup) {
        if (!killpopup && event.type == SDL_MOUSEBUTTONUP) {
            glictList *gl = popup->getGlictList();

            desktop.RemoveObject(gl);
            killpopup = true;
        }
    }
}

void GM_Debug::keyPress (char key)
{
	desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
}


void GM_Debug::msgBox (const char* mbox, const char* title, glictContainer* focusondismiss)
{
	glictSize s;
	glictMessageBox *mb;
	desktop.AddObject(mb = new glictMessageBox);

	mb->SetCaption(title);
	mb->SetMessage(mbox);

	mb->SetHeight(glictFontNumberOfLines(mbox)*12 + 35);
	int size1 = (int)glictFontSize(title, "system");
	int size2 = (int)glictFontSize(mbox, "system");
	mb->SetWidth(MAX(size1, size2));
	mb->Focus(NULL);

	mb->GetSize(&s);

	mb->SetPos(glictGlobals.w / 2 - s.w / 2, glictGlobals.h/ 2 - s.h / 2);

	mb->SetOnDismiss(GM_Debug::MBOnDismiss);

	mb->SetCustomData(focusondismiss);
}

void GM_Debug::MBOnDismiss(glictPos* pos, glictContainer* caller)
{
	GM_Debug* m = (GM_Debug*)g_game;
	if (caller->GetCustomData()) {
		glictContainer* focusOnDismiss = (glictContainer*)caller->GetCustomData();
		focusOnDismiss->SetVisible(true);
		focusOnDismiss->Focus(NULL);
	}

	//delete caller;
}
