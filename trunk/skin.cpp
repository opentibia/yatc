//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Skin loader and unloader
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
#include "skin.h"

Skin g_skin;

#include <GLICT/skinner.h>

#include "assert.h"
#include "defines.h" // for fileexists


skinImage::skinImage(float x, float y, float w, float h) {
	m_x = x;
	m_y = y;
	if (w == -1)
		if (g_skin.m_uisprite)
			m_width = g_skin.m_uisprite->getWidth();
		else
			m_width = 0;
	else
		m_width = w;

	if (h == -1)
		if (g_skin.m_uisprite)
			m_height = g_skin.m_uisprite->getHeight();
		else
			m_height = 0;
	else
		m_height = h;

}

void skinImage::Paint(float destx, float desty, float w, float h) {
	g_skin.m_uisprite->Blit(destx + glictGlobals.translation.x, desty + glictGlobals.translation.y, m_x, m_y, w, h);
}


/////////////////////////


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

	m_uisprite = g_engine->createSprite("Tibia.pic", 3);

    printf("[Skin::loadSkin] Loading window\n");
    win.SetTL		(wintl = new skinImage(106,	183,	4,		17));
    win.SetTop		(wint  = new skinImage(114,	183,	96,		17));
    win.SetTR		(wintr = new skinImage(110,	183,	4,		17));
    win.SetLeft		(winl  = new skinImage(256,	0,		4,		96));
    win.SetCenter	(winc  = new skinImage(0,	0,		96,		96));
    win.SetRight	(winr  = new skinImage(260,	0,		4,		96));
    win.SetBL		(winbl = new skinImage(2,	193,	4,		4)); // FIXME
    win.SetBottom	(winb  = new skinImage(2,	193,	96,		4));
    win.SetBR		(winbr = new skinImage(2,	193,	4,		4)); // FIXME

	glictGlobals.windowBodySkin = &win;

    printf("[Skin::loadSkin] Loading button\n");
    btn.SetTL		(btntl = new skinImage(174,	138,	1,		1));
    btn.SetTop		(btnt  = new skinImage(175,	138,	32,		1));
    btn.SetTR		(btntr = new skinImage(207,	138,	1,		1));
    btn.SetLeft		(btnl  = new skinImage(174,	139,	1,		18));
    btn.SetCenter	(btnc  = new skinImage(175,	139,	32,		18));
    btn.SetRight	(btnr  = new skinImage(207,	139,	1,		18));
    btn.SetBL		(btnbl = new skinImage(174,	157,	1,		1));
    btn.SetBottom	(btnb  = new skinImage(175,	157,	32,		1));
    btn.SetBR		(btnbr = new skinImage(207,	157,	1,		1));

	glictGlobals.buttonSkin = &btn;

    printf("[Skin::loadSkin] Loading button highlight\n");
    bth.SetTL		(bthtl = new skinImage(174,	158,	1,		1));
    bth.SetTop		(btht  = new skinImage(175,	158,	32,		1));
    bth.SetTR		(bthtr = new skinImage(207,	158,	1,		1));
    bth.SetLeft		(bthl  = new skinImage(174,	159,	1,		18));
    bth.SetCenter	(bthc  = new skinImage(175,	159,	32,		18));
    bth.SetRight	(bthr  = new skinImage(207,	159,	1,		18));
    bth.SetBL		(bthbl = new skinImage(174,	177,	1,		1));
    bth.SetBottom	(bthb  = new skinImage(175,	177,	32,		1));
    bth.SetBR		(bthbr = new skinImage(207,	177,	1,		1));

	glictGlobals.buttonHighlightSkin = &bth;

    printf("[Skin::loadSkin] Loading textbox\n"); // FIXME (ivucica#4#) looks like the "unchecked checkbox" is not the same as "textbox" -- see: "XPlike Tibia.pic" for more info
    txt.SetTL		(txttl = new skinImage(308,	96,		1,		1)); // in fact it looks like we have to HARDCODE this piece of skin.
    txt.SetTop		(txtt  = new skinImage(309,	96,		10,		1));
    txt.SetTR		(txttr = new skinImage(319,	96,		1,		1));
    txt.SetLeft		(txtl  = new skinImage(308,	97,		1,		10));
    txt.SetCenter	(txtc  = new skinImage(309,	97,		10,		10));
    txt.SetRight	(txtr  = new skinImage(319,	97,		1,		10));
    txt.SetBL		(txtbl = new skinImage(308,	107,	1,		1));
    txt.SetBottom	(txtb  = new skinImage(309,	107,	10,		1));
    txt.SetBR		(txtbr = new skinImage(319,	107,	1,		1));

	glictGlobals.textboxSkin = &txt;


    printf("[Skin::loadSkin] Loading checkbox\n");
    chk.SetTL		(chktl = new skinImage(308,	108,	1,		1));
    chk.SetTop		(chkt  = new skinImage(309,	108,	10,		1));
    chk.SetTR		(chktr = new skinImage(319,	108,	1,		1));
    chk.SetLeft		(chkl  = new skinImage(308,	109,	1,		10));
    chk.SetCenter	(chkc  = new skinImage(309,	109,	10,		10));
    chk.SetRight	(chkr  = new skinImage(319,	109,	1,		10));
    chk.SetBL		(chkbl = new skinImage(308,	119,	1,		1));
    chk.SetBottom	(chkb  = new skinImage(309,	119,	10,		1));
    chk.SetBR		(chkbr = new skinImage(319,	119,	1,		1));


	printf("[Skin::loadSkin] Loading panelmainmenu\n");
    tmm.SetTL		(tmmtl = new skinImage(44,		226,	5,		5));
    tmm.SetTop		(tmmt  = new skinImage(43,		214,	32,		5));
    tmm.SetTR		(tmmtr = new skinImage(49,		226,	5,		5));
    tmm.SetLeft		(tmml  = new skinImage(0,		214,	5,		32));
    tmm.SetCenter	(tmmc  = new skinImage(11,		214,	32,		32));
    tmm.SetRight	(tmmr  = new skinImage(6,		214,	5,		32));
    tmm.SetBL		(tmmbl = new skinImage(44,		231,	5,		5));
    tmm.SetBottom	(tmmb  = new skinImage(43,		219,	32,		5));
    tmm.SetBR		(tmmbr = new skinImage(49,		231,	5,		5));

}

void Skin::unloadSkin() {

//    printf("[Skin::unloadSkin] Unloading window skin...\n");
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


//    printf("[Skin::unloadSkin] Unloading button skin...\n");

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


//    printf("[Skin::unloadSkin] Unloading highlighted button skin...\n");

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

//    printf("[Skin::unloadSkin] Unloading textbox skin...\n");

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

//    printf("[Skin::unloadSkin] Unloading checkbox skin...\n");

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

//    printf("[Skin::unloadSkin] Unloading panelmainmenu skin...\n");

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

