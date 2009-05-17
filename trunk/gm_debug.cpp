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
#include <iomanip>

#include "gm_debug.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"
#include "options.h"
#include "itemui.h"
#include "util.h"

#include "net/connection.h"
#include "net/protocollogin.h"

extern Connection* g_connection;

#include "gamecontent/container.h"
#include "gamecontent/creature.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/inventory.h"
#include "gamecontent/map.h"

#include "gamecontent/item.h"

#include "product.h"


extern bool g_running;

#include "stackpanel.h"
#define YSPWINDOWS 5
static yatcStackPanel yatcstackpanel;
static glictWindow yspwTest[YSPWINDOWS];

#ifdef SDLTTF_EXPERIMENT
#warning Compiling with SDL_ttf test. If you dont have SDL_ttf library, undefine SDLTTF_EXPERIMENT.
#warning This is only temporary experiment for generating Tibia fonts from ttf files
#warning (bold Arial is source for most Tibias fonts, as demonstrable with GIMP)

#include <SDL/SDL_ttf.h>

static TTF_Font* font;
#endif

void cb1(Popup::Item*) {
    ((GM_Debug*)g_game)->msgBox("You have clicked on the \"Hello world\" menu item", "Yipee");
}

void cb2(Popup::Item*) {
    ((GM_Debug*)g_game)->msgBox("You have clicked on the \"Noob\" menu item", "Yipee");
}

void cb3(ChoiceGrid* parent, ChoiceGrid::Item* item, ChoiceGrid::Item* olditem) {
    if (olditem)
        printf("Old item: %s\n", olditem->txt.c_str());
    ((GM_Debug*)g_game)->msgBox("You have clicked on a choicegrid item", item->txt.c_str());
}



void GM_Debug::ButtonOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	//ProtocolConfig::getInstance().setServerType(SERVER_OTSERV);
	//ProtocolConfig::getInstance().setServer("localhost", 7172);
	//ProtocolConfig::createLoginConnection(1000, "test");

	GM_Debug *gd = (GM_Debug*)g_game;

    if (!gd->popup) {
        gd->popup = new Popup;
        glictPanel* gl = gd->popup->getPanel();

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
void GM_Debug::UpdateMapOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
    GM_Debug *gd = (GM_Debug*)g_game;

    gd->px = atoi(gd->txtLocX.GetCaption().c_str()); // playerx
    gd->py = atoi(gd->txtLocY.GetCaption().c_str());
    gd->pz = atoi(gd->txtLocZ.GetCaption().c_str());

    int xs = gd->px - gd->mapw/2;
    int ys = gd->py - gd->maph/2;
    int xe = gd->px + gd->mapw/2;
    int ye = gd->py + gd->maph/2;



    gd->mapcount = 0;

    for (int i = 0; i < 4; i++){
        delete gd->map[i];
        gd->map[i] = NULL;
        gd->mapfns[i] = "";
    }

    for (int j = ys; j<ye; j+=gd->maph-1)
        for (int i = xs; i<xe; i+=gd->mapw-1)
        {

            std::stringstream x,y,z,minimapfnss;
            x << setw(3) << setfill('0') << i / 256;
            y << setw(3) << setfill('0') << j / 256;
            z << setw(2) << setfill('0') << gd->pz;
            minimapfnss << x.str() << y.str() << z.str() << ".map";

            bool has_map = false;
            /*for(int k=0; k<gd->mapcount; k++)
            {
                if(gd->mapfns[i] == minimapfnss.str())
                {
                    has_map = true;
                    break;
                }
            }*/

            if (!has_map)
            {
                gd->mapcount++;
                gd->mapfns[gd->mapcount-1] = minimapfnss.str();
                gd->map[gd->mapcount-1] = g_engine->createSprite(minimapfnss.str());
            }

        }
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


    desktop.AddObject(&txtLocX);
    desktop.AddObject(&txtLocY);
    desktop.AddObject(&txtLocZ);
    txtLocX.SetWidth(50);
    txtLocY.SetWidth(50);
    txtLocZ.SetWidth(50);
    txtLocX.SetCaption("32600");
    txtLocY.SetCaption("31750");
    txtLocZ.SetCaption("7");
    txtLocX.SetHeight(16);
    txtLocY.SetHeight(16);
    txtLocZ.SetHeight(16);
    txtLocX.SetPos(50,190);
    txtLocY.SetPos(100,190);
    txtLocZ.SetPos(150,190);

    desktop.AddObject(&btnUpdateMap);
    btnUpdateMap.SetPos(200,190);
    btnUpdateMap.SetHeight(16);
    btnUpdateMap.SetWidth(120);
    btnUpdateMap.SetCaption("Update map");
    btnUpdateMap.SetOnClick(GM_Debug::UpdateMapOnClick);


    desktop.AddObject(grid.getGrid());
    grid.getGrid()->SetPos(0, 0);
    grid.setRows(3);
    grid.setItemSize(100,20);
    grid.setPadding(5,5);
    grid.addItem("Hello world", NULL, NULL);
    grid.addItem("Testing grid", NULL, NULL);
    grid.addItem("How about this", NULL, NULL);
    grid.addItem("This rox", NULL, NULL);
    grid.addItem("Sure?", NULL, NULL);
    grid.setOnClick(cb3);

    desktop.AddObject(&yatcstackpanel);
    yatcstackpanel.SetPos(400,100);
    yatcstackpanel.SetHeight(200);
    yatcstackpanel.SetWidth(100);
    yatcstackpanel.SetBGActiveness(false);
    for (int i = 0; i < YSPWINDOWS; i++) {
        std::stringstream s;
        s << "yspwTest[" << i << "]";
        yspwTest[i].SetHeight(15);
        yspwTest[i].SetCaption(s.str().c_str());
        yatcstackpanel.AddObject(&yspwTest[i]);
    }


    popup = NULL;
    killpopup = false;
    map[0] = map[1] = map[2] = map[3] = NULL;
    mapcount = 0;
    mapw = 256;
    maph = 256;

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
	printf("2\n");

	#ifdef SDLTTF_EXPERIMENT
	{
	    int ptsize=11;
        char fontname[]="arial.ttf";

        TTF_Init();

        font = TTF_OpenFont(fontname,ptsize);
        if ( font == NULL ) {
            fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",
                                    ptsize, fontname, SDL_GetError());

        }

        int renderstyle=0;
        renderstyle |= TTF_STYLE_BOLD;
        TTF_SetFontStyle(font, renderstyle);

	}

	#endif
}

GM_Debug::~GM_Debug()
{
	delete background;
	delete spr;
	delete map;
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
		background->Blit(0,0,0,0,background->getBasicWidth(),background->getBasicHeight(),glictGlobals.w, glictGlobals.h);
	if(spr)
		spr->Blit(50,50);
 	if(thing)
		thing->Blit(100,50);

	if(background)
		background->Blit(150,50,125,51,85,136,32,48);

/*
    int xs = px - mapw/2;
    int ys = py - maph/2;
    int xe = px + mapw/2;
    int ye = py + maph/2;

    if (map[0]) map[0]->Blit(0,0, xs%256, ys%256, 256-(xs%256), 256-(ys%256));
    if (map[1]) map[1]->Blit(256-(xs%256),0, 0, ys%256, mapw-(256-(xs%256)), 256-(ys%256));
    if (map[2]) map[2]->Blit(0,256-(ys%256), xs%256, 0, 256-(xs%256), maph-(256-(ys%256)));
    if (map[3]) map[3]->Blit(256-(xs%256),256-(ys%256), 0, 0, mapw-(256-(xs%256)), maph-(256-(ys%256)));
*/
	GM_Debug *gd = (GM_Debug*)g_game;
    a.renderSelf(700,0,256,256,Position(gd->px, gd->py, gd->pz));

/*
    std::stringstream testchar;
    testchar << (char)('u'+32);

    for(int i = 0; i < 255; i++) {
        g_engine->drawText(testchar.str().c_str(), "system", 0+(i%8)*12,300+(i/8)*12, i);
    }
*/


	desktop.RememberTransformations();
	desktop.Paint();

#ifdef SDLTTF_EXPERIMENT
    if (font) {

        SDL_Surface* glyph = NULL;
        SDL_Color forecol, backcol;

        forecol.r = 255;
        forecol.g = 255;
        forecol.b = 255;

        backcol.r = 0;
        backcol.g = 0;
        backcol.b = 0;


        SDL_SetClipRect(g_engine->m_screen, NULL);

        int YOffset = TTF_FontAscent(font);  // Actually a baseline


        for (int i = 32; i < 256; i++)
        {

            int minx, maxx, miny, maxy, advance;


            glyph = TTF_RenderGlyph_Solid( font, i, forecol);//backcol ); // solid or shaded; shaded uses backcol, solid doesnt


            TTF_GlyphMetrics(font, i, &minx, &maxx, &miny, &maxy, &advance);


            SDL_Rect src = {0, 0, glyph->w, glyph->h};
            SDL_Rect dest = {((i - 32) % 32)*8 + minx,
                             ((i - 32) / 32)*16 + YOffset-maxy,
                             glyph->w,
                             glyph->h};





            SDL_BlitSurface(glyph, &src, g_engine->m_screen, &dest);


            SDL_FreeSurface(glyph);
        }
    }
#endif

}

void GM_Debug::mouseEvent(SDL_Event& event)
{
	glictPos pos;
	pos.x = ptrx;
	pos.y = ptry;

	desktop.TransformScreenCoords(&pos);


    bool hadpopup = false;
    if (popup) {
        hadpopup = true;
        glictPanel *gl = popup->getPanel();
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
            glictPanel *gl = popup->getPanel();

            if (event.button.state == SDL_PRESSED)
                gl->CastEvent(GLICT_MOUSEDOWN, &pos, 0, NULL);
            if (event.button.state != SDL_PRESSED)
                gl->CastEvent(GLICT_MOUSEUP, &pos, 0, NULL);



            if (!killpopup && event.type == SDL_MOUSEBUTTONUP) {
                glictPanel *gl = popup->getPanel();

                desktop.RemoveObject(gl);
                killpopup = true;
            }

            return;
        }

    }

    if (event.type != SDL_MOUSEMOTION) {
        if (event.button.state == SDL_PRESSED)
            desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
        else
            desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);
    } else {
        #if (GLICT_APIREV >= 67)
        desktop.CastEvent(GLICT_MOUSEMOVE, &pos, 0);
        #else
        #warning We need GLICT apirev 67 or greater to support basic movable windows.
        #endif
    }

    if (hadpopup) {
        if (!killpopup && event.type == SDL_MOUSEBUTTONUP) {
            glictPanel *gl = popup->getPanel();

            desktop.RemoveObject(gl);
            killpopup = true;
        }
    }
}

void GM_Debug::keyPress (int key)
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
	//GM_Debug* m = (GM_Debug*)g_game;
	if (caller->GetCustomData()) {
		glictContainer* focusOnDismiss = (glictContainer*)caller->GetCustomData();
		focusOnDismiss->SetVisible(true);
		focusOnDismiss->Focus(NULL);
	}

	//delete caller;
}
