

#include <GLICT/globals.h>
#include "skin.h"

Skin skin;

#if (GLICT_APIREV >= 49)


#include <GLICT/skinner.h>

#include "assert.h"
#include "defines.h" // for fileexists



Skin::Skin() {
    wintl = NULL;
    wint  = NULL;
    wintr = NULL;
    winl  = NULL;
    winc  = NULL;
    winr  = NULL;
    winbl = NULL;
    winb  = NULL;
    winbr = NULL;

    btntl = NULL;
    btnt  = NULL;
    btntr = NULL;
    btnl  = NULL;
    btnc  = NULL;
    btnr  = NULL;
    btnbl = NULL;
    btnb  = NULL;
    btnbr = NULL;

    bthtl = NULL;
    btht  = NULL;
    bthtr = NULL;
    bthl  = NULL;
    bthc  = NULL;
    bthr  = NULL;
    bthbl = NULL;
    bthb  = NULL;
    bthbr = NULL;

    txttl = NULL;
    txtt  = NULL;
    txttr = NULL;
    txtl  = NULL;
    txtc  = NULL;
    txtr  = NULL;
    txtbl = NULL;
    txtb  = NULL;
    txtbr = NULL;

    tmmtl = NULL;
    tmmt  = NULL;
    tmmtr = NULL;
    tmml  = NULL;
    tmmc  = NULL;
    tmmr  = NULL;
    tmmbl = NULL;
    tmmb  = NULL;
    tmmbr = NULL;


}
Skin::~Skin() {
    unloadSkin();

}
void Skin::loadSkin(const char* what) {
	// FIXME (ivucica#1#) we should load skin from tibia.pic
	printf("[Skin::loadSkin] Stub! Loading outcast-style skin %s...\n", what);

    std::string dirname = "skins/";
    dirname += what;
    dirname += "/";

    unloadSkin();

	{
		FILE *f = fopen((dirname + "window/tl.bmp").c_str(), "r");
		if (f) fclose(f); else {
			printf("[Skin::loadSkin] No such skin %s!\n", what);
			return;
		}
	}

    printf("[Skin::loadSkin] Loading colors\n");
    {
        FILE *f = fopen((dirname + "colors.txt").c_str(), "r");
        if (f) {
            fscanf(f, "%g %g %g %g", &glictGlobals.buttonTextColor.r, &glictGlobals.buttonTextColor.g, &glictGlobals.buttonTextColor.b, &glictGlobals.buttonTextColor.a);
            fscanf(f, "%g %g %g %g", &glictGlobals.buttonHighlightTextColor.r, &glictGlobals.buttonHighlightTextColor.g, &glictGlobals.buttonHighlightTextColor.b, &glictGlobals.buttonHighlightTextColor.a);
            fscanf(f, "%g %g %g %g", &glictGlobals.panelTextColor.r,  &glictGlobals.panelTextColor.g,  &glictGlobals.panelTextColor.b,  &glictGlobals.panelTextColor.a);
            fscanf(f, "%g %g %g %g", &glictGlobals.textboxTextColor.r,  &glictGlobals.textboxTextColor.g,  &glictGlobals.textboxTextColor.b,  &glictGlobals.textboxTextColor.a);
            fscanf(f, "%g %g %g %g", &glictGlobals.windowTitleColor.r, &glictGlobals.windowTitleColor.g, &glictGlobals.windowTitleColor.b, &glictGlobals.windowTitleColor.a);


            fclose(f);
        }
    }

    //return;
    printf("[Skin::loadSkin] Loading window\n");
    win.SetTL		(wintl = new skinImage(dirname + "window/tl.bmp"));
    win.SetTop		(wint  = new skinImage(dirname + "window/t.bmp" ));
    win.SetTR		(wintr = new skinImage(dirname + "window/tr.bmp"));
    win.SetLeft		(winl  = new skinImage(dirname + "window/l.bmp" ));
    win.SetCenter	(winc  = new skinImage(dirname + "window/c.bmp" ));
    win.SetRight	(winr  = new skinImage(dirname + "window/r.bmp" ));
    win.SetBL		(winbl = new skinImage(dirname + "window/bl.bmp"));
    win.SetBottom	(winb  = new skinImage(dirname + "window/b.bmp" ));
    win.SetBR		(winbr = new skinImage(dirname + "window/br.bmp"));

	glictGlobals.windowBodySkin = &win;

    printf("[Skin::loadSkin] Loading button\n");
    btn.SetTL		(btntl = new skinImage(dirname + "button/tl.bmp"));
    btn.SetTop		(btnt  = new skinImage(dirname + "button/t.bmp" ));
    btn.SetTR		(btntr = new skinImage(dirname + "button/tr.bmp"));
    btn.SetLeft		(btnl  = new skinImage(dirname + "button/l.bmp" ));
    btn.SetCenter	(btnc  = new skinImage(dirname + "button/c.bmp" ));
    btn.SetRight	(btnr  = new skinImage(dirname + "button/r.bmp" ));
    btn.SetBL		(btnbl = new skinImage(dirname + "button/bl.bmp"));
    btn.SetBottom	(btnb  = new skinImage(dirname + "button/b.bmp" ));
    btn.SetBR		(btnbr = new skinImage(dirname + "button/br.bmp"));

	glictGlobals.buttonSkin = &btn;

    printf("[Skin::loadSkin] Loading button highlight\n");
    bth.SetTL		(bthtl = new skinImage(dirname + "buttonhighlight/tl.bmp"));
    bth.SetTop		(btht  = new skinImage(dirname + "buttonhighlight/t.bmp" ));
    bth.SetTR		(bthtr = new skinImage(dirname + "buttonhighlight/tr.bmp"));
    bth.SetLeft		(bthl  = new skinImage(dirname + "buttonhighlight/l.bmp" ));
    bth.SetCenter	(bthc  = new skinImage(dirname + "buttonhighlight/c.bmp" ));
    bth.SetRight	(bthr  = new skinImage(dirname + "buttonhighlight/r.bmp" ));
    bth.SetBL		(bthbl = new skinImage(dirname + "buttonhighlight/bl.bmp"));
    bth.SetBottom	(bthb  = new skinImage(dirname + "buttonhighlight/b.bmp" ));
    bth.SetBR		(bthbr = new skinImage(dirname + "buttonhighlight/br.bmp"));

	glictGlobals.buttonHighlightSkin = &bth;

    printf("[Skin::loadSkin] Loading textbox\n");
    txt.SetTL		(txttl = new skinImage(dirname + "textbox/tl.bmp"));
    txt.SetTop		(txtt  = new skinImage(dirname + "textbox/t.bmp" ));
    txt.SetTR		(txttr = new skinImage(dirname + "textbox/tr.bmp"));
    txt.SetLeft		(txtl  = new skinImage(dirname + "textbox/l.bmp" ));
    txt.SetCenter	(txtc  = new skinImage(dirname + "textbox/c.bmp" ));
    txt.SetRight	(txtr  = new skinImage(dirname + "textbox/r.bmp" ));
    txt.SetBL		(txtbl = new skinImage(dirname + "textbox/bl.bmp"));
    txt.SetBottom	(txtb  = new skinImage(dirname + "textbox/b.bmp" ));
    txt.SetBR		(txtbr = new skinImage(dirname + "textbox/br.bmp"));


	glictGlobals.textboxSkin = &txt;


	printf("[Skin::loadSkin] Loading panelmainmenu\n");
    tmm.SetTL		(tmmtl = new skinImage(dirname + "panelmainmenu/tl.bmp"));
    tmm.SetTop		(tmmt  = new skinImage(dirname + "panelmainmenu/t.bmp" ));
    tmm.SetTR		(tmmtr = new skinImage(dirname + "panelmainmenu/tr.bmp"));
    tmm.SetLeft		(tmml  = new skinImage(dirname + "panelmainmenu/l.bmp" ));
    tmm.SetCenter	(tmmc  = new skinImage(dirname + "panelmainmenu/c.bmp" ));
    tmm.SetRight	(tmmr  = new skinImage(dirname + "panelmainmenu/r.bmp" ));
    tmm.SetBL		(tmmbl = new skinImage(dirname + "panelmainmenu/bl.bmp"));
    tmm.SetBottom	(tmmb  = new skinImage(dirname + "panelmainmenu/b.bmp" ));
    tmm.SetBR		(tmmbr = new skinImage(dirname + "panelmainmenu/br.bmp"));

}

void Skin::unloadSkin() {

    printf("[Skin::unloadSkin] Unloading window skin...\n");
    if (wintl) delete wintl; wintl = NULL;
    if (wint)  delete wint;  wint  = NULL;
    if (wintr) delete wintr; wintr = NULL;
    if (winl)  delete winl;  winl  = NULL;
    if (winc)  delete winc;  winc  = NULL;
    if (winr)  delete winr;  winr  = NULL;
    if (winbl) delete winbl; winbl = NULL;
    if (winb)  delete winb;  winb  = NULL;
    if (winbr) delete winbr; winbr = NULL;



    win.SetTL(NULL);
    win.SetTop(NULL);
    win.SetTR(NULL);
    win.SetLeft(NULL);
    win.SetCenter(NULL);
    win.SetRight(NULL);
    win.SetBL(NULL);
    win.SetBottom(NULL);
    win.SetBR(NULL);


    printf("[Skin::unloadSkin] Unloading button skin...\n");

    if (btntl) delete btntl; btntl = NULL;
    if (btnt)  delete btnt;  btnt  = NULL;
    if (btntr) delete btntr; btntr = NULL;
    if (btnl)  delete btnl;  btnl  = NULL;
    if (btnc)  delete btnc;  btnc  = NULL;
    if (btnr)  delete btnr;  btnr  = NULL;
    if (btnbl) delete btnbl; btnbl = NULL;
    if (btnb)  delete btnb;  btnb  = NULL;
    if (btnbr) delete btnbr; btnbr = NULL;

    btn.SetTL(NULL);
    btn.SetTop(NULL);
    btn.SetTR(NULL);
    btn.SetLeft(NULL);
    btn.SetCenter(NULL);
    btn.SetRight(NULL);
    btn.SetBL(NULL);
    btn.SetBottom(NULL);
    btn.SetBR(NULL);

    glictGlobals.buttonSkin = NULL;


    printf("[Skin::unloadSkin] Unloading highlighted button skin...\n");

    if (bthtl) delete bthtl; bthtl = NULL;
    if (btht)  delete btht;  btht  = NULL;
    if (bthtr) delete bthtr; bthtr = NULL;
    if (bthl)  delete bthl;  bthl  = NULL;
    if (bthc)  delete bthc;  bthc  = NULL;
    if (bthr)  delete bthr;  bthr  = NULL;
    if (bthbl) delete bthbl; bthbl = NULL;
    if (bthb)  delete bthb;  bthb  = NULL;
    if (bthbr) delete bthbr; bthbr = NULL;

    bth.SetTL(NULL);
    bth.SetTop(NULL);
    bth.SetTR(NULL);
    bth.SetLeft(NULL);
    bth.SetCenter(NULL);
    bth.SetRight(NULL);
    bth.SetBL(NULL);
    bth.SetBottom(NULL);
    bth.SetBR(NULL);

    glictGlobals.buttonHighlightSkin = NULL;

    printf("[Skin::unloadSkin] Unloading textbox skin...\n");

    if (txttl) delete txttl; txttl = NULL;
    if (txtt)  delete txtt;  txtt  = NULL;
    if (txttr) delete txttr; txttr = NULL;
    if (txtl)  delete txtl;  txtl  = NULL;
    if (txtc)  delete txtc;  txtc  = NULL;
    if (txtr)  delete txtr;  txtr  = NULL;
    if (txtbl) delete txtbl; txtbl = NULL;
    if (txtb)  delete txtb;  txtb  = NULL;
    if (txtbr) delete txtbr; txtbr = NULL;

    txt.SetTL(NULL);
    txt.SetTop(NULL);
    txt.SetTR(NULL);
    txt.SetLeft(NULL);
    txt.SetCenter(NULL);
    txt.SetRight(NULL);
    txt.SetBL(NULL);
    txt.SetBottom(NULL);
    txt.SetBR(NULL);

    glictGlobals.textboxSkin = NULL;

    printf("[Skin::unloadSkin] Unloading tibiamainmenu skin...\n");

    if (tmmtl) delete tmmtl; tmmtl = NULL;
    if (tmmt)  delete tmmt;  tmmt  = NULL;
    if (tmmtr) delete tmmtr; tmmtr = NULL;
    if (tmml)  delete tmml;  tmml  = NULL;
    if (tmmc)  delete tmmc;  tmmc  = NULL;
    if (tmmr)  delete tmmr;  tmmr  = NULL;
    if (tmmbl) delete tmmbl; tmmbl = NULL;
    if (tmmb)  delete tmmb;  tmmb  = NULL;
    if (tmmbr) delete tmmbr; tmmbr = NULL;

    tmm.SetTL(NULL);
    tmm.SetTop(NULL);
    tmm.SetTR(NULL);
    tmm.SetLeft(NULL);
    tmm.SetCenter(NULL);
    tmm.SetRight(NULL);
    tmm.SetBL(NULL);
    tmm.SetBottom(NULL);
    tmm.SetBR(NULL);

    printf("[Skin::unloadSkin] UNLOADED ALL!\n");
}


#endif // glict_apirev
