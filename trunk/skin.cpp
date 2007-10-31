

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

    chktl = NULL;
    chkt  = NULL;
    chktr = NULL;
    chkl  = NULL;
    chkc  = NULL;
    chkr  = NULL;
    chkbl = NULL;
    chkb  = NULL;
    chkbr = NULL;

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
void Skin::loadSkin() {

	printf("[Skin::loadSkin] Loading skin from Tibia.pic...\n");

    printf("[Skin::loadSkin] Loading window\n");
    win.SetTL		(wintl = new skinImage("Tibia.pic",	3,	106,	183,	4,		17));
    win.SetTop		(wint  = new skinImage("Tibia.pic",	3,	114,	183,	96,		17));
    win.SetTR		(wintr = new skinImage("Tibia.pic",	3,	110,	183,	4,		17));
    win.SetLeft		(winl  = new skinImage("Tibia.pic",	3,	256,	0,		4,		96));
    win.SetCenter	(winc  = new skinImage("Tibia.pic",	3,	0,		0,		96,		96));
    win.SetRight	(winr  = new skinImage("Tibia.pic",	3,	260,	0,		4,		96));
    win.SetBL		(winbl = new skinImage("Tibia.pic",	3,	2,		193,	4,		4)); // FIXME
    win.SetBottom	(winb  = new skinImage("Tibia.pic",	3,	2,		193,	96,		4));
    win.SetBR		(winbr = new skinImage("Tibia.pic",	3,	2,		193,	4,		4)); // FIXME

	glictGlobals.windowBodySkin = &win;

    printf("[Skin::loadSkin] Loading button\n");
    btn.SetTL		(btntl = new skinImage("Tibia.pic", 3,	174,	138,	1,		1));
    btn.SetTop		(btnt  = new skinImage("Tibia.pic", 3,	175,	138,	32,		1));
    btn.SetTR		(btntr = new skinImage("Tibia.pic", 3,	207,	138,	1,		1));
    btn.SetLeft		(btnl  = new skinImage("Tibia.pic", 3,	174,	139,	1,		18));
    btn.SetCenter	(btnc  = new skinImage("Tibia.pic", 3,	175,	139,	32,		18));
    btn.SetRight	(btnr  = new skinImage("Tibia.pic", 3,	207,	139,	1,		18));
    btn.SetBL		(btnbl = new skinImage("Tibia.pic", 3,	174,	157,	1,		1));
    btn.SetBottom	(btnb  = new skinImage("Tibia.pic", 3,	175,	157,	32,		1));
    btn.SetBR		(btnbr = new skinImage("Tibia.pic", 3,	207,	157,	1,		1));

	glictGlobals.buttonSkin = &btn;

    printf("[Skin::loadSkin] Loading button highlight\n");
    bth.SetTL		(bthtl = new skinImage("Tibia.pic", 3,	174,	158,	1,		1));
    bth.SetTop		(btht  = new skinImage("Tibia.pic", 3,	175,	158,	32,		1));
    bth.SetTR		(bthtr = new skinImage("Tibia.pic", 3,	207,	158,	1,		1));
    bth.SetLeft		(bthl  = new skinImage("Tibia.pic", 3,	174,	159,	1,		18));
    bth.SetCenter	(bthc  = new skinImage("Tibia.pic", 3,	175,	159,	32,		18));
    bth.SetRight	(bthr  = new skinImage("Tibia.pic", 3,	207,	159,	1,		18));
    bth.SetBL		(bthbl = new skinImage("Tibia.pic", 3,	174,	177,	1,		1));
    bth.SetBottom	(bthb  = new skinImage("Tibia.pic", 3,	175,	177,	32,		1));
    bth.SetBR		(bthbr = new skinImage("Tibia.pic", 3,	207,	177,	1,		1));

	glictGlobals.buttonHighlightSkin = &bth;

    printf("[Skin::loadSkin] Loading textbox\n");
    txt.SetTL		(txttl = new skinImage("Tibia.pic", 3,	308,	96,		1,		1));
    txt.SetTop		(txtt  = new skinImage("Tibia.pic", 3,	309,	96,		10,		1));
    txt.SetTR		(txttr = new skinImage("Tibia.pic", 3,	319,	96,		1,		1));
    txt.SetLeft		(txtl  = new skinImage("Tibia.pic", 3,	308,	97,		1,		10));
    txt.SetCenter	(txtc  = new skinImage("Tibia.pic", 3,	309,	97,		10,		10));
    txt.SetRight	(txtr  = new skinImage("Tibia.pic", 3,	319,	97,		1,		10));
    txt.SetBL		(txtbl = new skinImage("Tibia.pic", 3,	308,	107,	1,		1));
    txt.SetBottom	(txtb  = new skinImage("Tibia.pic", 3,	309,	107,	10,		1));
    txt.SetBR		(txtbr = new skinImage("Tibia.pic", 3,	319,	107,	1,		1));

	glictGlobals.textboxSkin = &txt;


    printf("[Skin::loadSkin] Loading checkbox\n");
    chk.SetTL		(chktl = new skinImage("Tibia.pic", 3,	308,	108,	1,		1));
    chk.SetTop		(chkt  = new skinImage("Tibia.pic", 3,	309,	108,	10,		1));
    chk.SetTR		(chktr = new skinImage("Tibia.pic", 3,	319,	108,	1,		1));
    chk.SetLeft		(chkl  = new skinImage("Tibia.pic", 3,	308,	109,	1,		10));
    chk.SetCenter	(chkc  = new skinImage("Tibia.pic", 3,	309,	109,	10,		10));
    chk.SetRight	(chkr  = new skinImage("Tibia.pic", 3,	319,	109,	1,		10));
    chk.SetBL		(chkbl = new skinImage("Tibia.pic", 3,	308,	119,	1,		1));
    chk.SetBottom	(chkb  = new skinImage("Tibia.pic", 3,	309,	119,	10,		1));
    chk.SetBR		(chkbr = new skinImage("Tibia.pic", 3,	319,	119,	1,		1));


	printf("[Skin::loadSkin] Loading panelmainmenu\n");
    tmm.SetTL		(tmmtl = new skinImage("Tibia.pic", 3,	308,	96,		1,		1));
    tmm.SetTop		(tmmt  = new skinImage("Tibia.pic", 3,	309,	96,		10,		1));
    tmm.SetTR		(tmmtr = new skinImage("Tibia.pic", 3,	319,	96,		1,		1));
    tmm.SetLeft		(tmml  = new skinImage("Tibia.pic", 3,	308,	97,		1,		10));
    tmm.SetCenter	(tmmc  = new skinImage("Tibia.pic", 3,	309,	97,		10,		10));
    tmm.SetRight	(tmmr  = new skinImage("Tibia.pic", 3,	319,	97,		1,		10));
    tmm.SetBL		(tmmbl = new skinImage("Tibia.pic", 3,	308,	107,	1,		1));
    tmm.SetBottom	(tmmb  = new skinImage("Tibia.pic", 3,	309,	107,	10,		1));
    tmm.SetBR		(tmmbr = new skinImage("Tibia.pic", 3,	319,	107,	1,		1));

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

    printf("[Skin::unloadSkin] Unloading checkbox skin...\n");

    if (chktl) delete chktl; chktl = NULL;
    if (chkt)  delete chkt;  chkt  = NULL;
    if (chktr) delete chktr; chktr = NULL;
    if (chkl)  delete chkl;  chkl  = NULL;
    if (chkc)  delete chkc;  chkc  = NULL;
    if (chkr)  delete chkr;  chkr  = NULL;
    if (chkbl) delete chkbl; chkbl = NULL;
    if (chkb)  delete chkb;  chkb  = NULL;
    if (chkbr) delete chkbr; chkbr = NULL;

    chk.SetTL(NULL);
    chk.SetTop(NULL);
    chk.SetTR(NULL);
    chk.SetLeft(NULL);
    chk.SetCenter(NULL);
    chk.SetRight(NULL);
    chk.SetBL(NULL);
    chk.SetBottom(NULL);
    chk.SetBR(NULL);

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

}


#endif // glict_apirev
