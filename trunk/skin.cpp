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
		if (g_engine->getUISprite())
			m_width = g_engine->getUISprite()->getWidth();
		else
			m_width = 0;
	else
		m_width = w;

	if (h == -1)
		if (g_engine->getUISprite())
			m_height = g_engine->getUISprite()->getHeight();
		else
			m_height = 0;
	else
		m_height = h;

}

void skinImage::Paint(float destx, float desty, float w, float h) {
	g_engine->getUISprite()->Blit(destx + glictGlobals.translation.x, desty + glictGlobals.translation.y, m_x, m_y, w, h);
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

    invtl = NULL;
    invt  = NULL;
    invtr = NULL;
    invl  = NULL;
    invc  = NULL;
    invr  = NULL;
    invbl = NULL;
    invb  = NULL;
    invbr = NULL;

    scutl = NULL;
    scut  = NULL;
    scutr = NULL;
    scul  = NULL;
    scuc  = NULL;
    scur  = NULL;
    scubl = NULL;
    scub  = NULL;
    scubr = NULL;

    shutl = NULL;
    shut  = NULL;
    shutr = NULL;
    shul  = NULL;
    shuc  = NULL;
    shur  = NULL;
    shubl = NULL;
    shub  = NULL;
    shubr = NULL;

    scdtl = NULL;
    scdt  = NULL;
    scdtr = NULL;
    scdl  = NULL;
    scdc  = NULL;
    scdr  = NULL;
    scdbl = NULL;
    scdb  = NULL;
    scdbr = NULL;

    shdtl = NULL;
    shdt  = NULL;
    shdtr = NULL;
    shdl  = NULL;
    shdc  = NULL;
    shdr  = NULL;
    shdbl = NULL;
    shdb  = NULL;
    shdbr = NULL;

    scbtl = NULL;
    scbt  = NULL;
    scbtr = NULL;
    scbl  = NULL;
    scbc  = NULL;
    scbr  = NULL;
    scbbl = NULL;
    scbb  = NULL;
    scbbr = NULL;

    scptl = NULL;
    scpt  = NULL;
    scptr = NULL;
    scpl  = NULL;
    scpc  = NULL;
    scpr  = NULL;
    scpbl = NULL;
    scpb  = NULL;
    scpbr = NULL;


}
Skin::~Skin() {
    unloadSkin();

}
void Skin::loadSkin() {
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

    // FIXME (ivucica#4#) looks like the "unchecked checkbox" is not the same as "textbox" -- see: "XPlike Tibia.pic" for more info
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

    chk.SetTL		(chktl = new skinImage(308,	108,	1,		1));
    chk.SetTop		(chkt  = new skinImage(309,	108,	10,		1));
    chk.SetTR		(chktr = new skinImage(319,	108,	1,		1));
    chk.SetLeft		(chkl  = new skinImage(308,	109,	1,		10));
    chk.SetCenter	(chkc  = new skinImage(309,	109,	10,		10));
    chk.SetRight	(chkr  = new skinImage(319,	109,	1,		10));
    chk.SetBL		(chkbl = new skinImage(308,	119,	1,		1));
    chk.SetBottom	(chkb  = new skinImage(309,	119,	10,		1));
    chk.SetBR		(chkbr = new skinImage(319,	119,	1,		1));


    tmm.SetTL		(tmmtl = new skinImage(44,		226,	5,		5));
    tmm.SetTop		(tmmt  = new skinImage(43,		214,	32,		5));
    tmm.SetTR		(tmmtr = new skinImage(49,		226,	5,		5));
    tmm.SetLeft		(tmml  = new skinImage(0,		214,	5,		32));
    tmm.SetCenter	(tmmc  = new skinImage(11,		214,	32,		32));
    tmm.SetRight	(tmmr  = new skinImage(6,		214,	5,		32));
    tmm.SetBL		(tmmbl = new skinImage(44,		231,	5,		5));
    tmm.SetBottom	(tmmb  = new skinImage(43,		219,	32,		5));
    tmm.SetBR		(tmmbr = new skinImage(49,		231,	5,		5));



    inv.SetTL		(invtl = new skinImage(186,		64, 	1,		1));
    inv.SetTop		(invt  = new skinImage(187,		64, 	31,		1));
    inv.SetTR		(invtr = new skinImage(219,		64, 	1,		1));
    inv.SetLeft		(invl  = new skinImage(186,		65,	    1,		31));
    inv.SetCenter	(invc  = new skinImage(187,		65,	    31,		31));
    inv.SetRight	(invr  = new skinImage(219,		65,	    1,		31));
    inv.SetBL		(invbl = new skinImage(186,		97, 	1,		1));
    inv.SetBottom	(invb  = new skinImage(187,		97,	    31,		1));
    inv.SetBR		(invbr = new skinImage(219,		97,	    1,		1));
#if GLICT_APIREV < 68
#warning Skinned scrollbar buttons need at least GLICT apirev 68
#else
    scu.SetTL		(scutl = NULL);
    scu.SetTop		(scut  = NULL);
    scu.SetTR		(scutr = NULL);
    scu.SetLeft		(scul  = NULL);
    scu.SetCenter	(scuc  = new skinImage(232,		64,	    12,		12));
    scu.SetRight	(scur  = NULL);
    scu.SetBL		(scubl = NULL);
    scu.SetBottom	(scub  = NULL);
    scu.SetBR		(scubr = NULL);

    glictGlobals.scrollbarUpperSkin = &scu;

    shu.SetTL		(shutl = NULL);
    shu.SetTop		(shut  = NULL);
    shu.SetTR		(shutr = NULL);
    shu.SetLeft		(shul  = NULL);
    shu.SetCenter	(shuc  = new skinImage(234,		122,    12,		12));
    shu.SetRight	(shur  = NULL);
    shu.SetBL		(shubl = NULL);
    shu.SetBottom	(shub  = NULL);
    shu.SetBR		(shubr = NULL);

    glictGlobals.scrollbarUpperHighlightSkin = &shu;

    scd.SetTL		(scdtl = NULL);
    scd.SetTop		(scdt  = NULL);
    scd.SetTR		(scdtr = NULL);
    scd.SetLeft		(scdl  = NULL);
    scd.SetCenter	(scdc  = new skinImage(244,		64,	    12,		12));
    scd.SetRight	(scdr  = NULL);
    scd.SetBL		(scdbl = NULL);
    scd.SetBottom	(scdb  = NULL);
    scd.SetBR		(scdbr = NULL);

    glictGlobals.scrollbarLowerSkin = &scd;

    shd.SetTL		(shdtl = NULL);
    shd.SetTop		(shdt  = NULL);
    shd.SetTR		(shdtr = NULL);
    shd.SetLeft		(shdl  = NULL);
    shd.SetCenter	(shdc  = new skinImage(246,		122,    12,		12));
    shd.SetRight	(shdr  = NULL);
    shd.SetBL		(shdbl = NULL);
    shd.SetBottom	(shdb  = NULL);
    shd.SetBR		(shdbr = NULL);

    glictGlobals.scrollbarLowerHighlightSkin = &shd;

    scb.SetTL		(scbtl = NULL);
    scb.SetTop		(scbt  = NULL);
    scb.SetTR		(scbtr = NULL);
    scb.SetLeft		(scbl  = NULL);
    scb.SetCenter	(scbc  = new skinImage(220,		64,    12,		12));
    scb.SetRight	(scbr  = NULL);
    scb.SetBL		(scbbl = NULL);
    scb.SetBottom	(scbb  = NULL);
    scb.SetBR		(scbbr = NULL);

    glictGlobals.scrollbarDragSkin = &scb;

    scp.SetTL		(scptl = NULL);
    scp.SetTop		(scpt  = NULL);
    scp.SetTR		(scptr = NULL);
    scp.SetLeft		(scpl  = new skinImage(264,		0,     1,		96));
    scp.SetCenter	(scpc  = new skinImage(265,		0,     10,		96));
    scp.SetRight	(scpr  = new skinImage(275,		0,     1,		96));
    scp.SetBL		(scpbl = NULL);
    scp.SetBottom	(scpb  = NULL);
    scp.SetBR		(scpbr = NULL);

    glictGlobals.scrollbarPanelSkin = &scp;
#endif
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

    glictGlobals.windowBodySkin = NULL;



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

    glictGlobals.textboxSkin = NULL;


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

//    printf("[Skin::unloadSkin] Unloading inventory element skin...\n");

    if (invtl) delete invtl; invtl = NULL;
    if (invt)  delete invt;  invt  = NULL;
    if (invtr) delete invtr; invtr = NULL;
    if (invl)  delete invl;  invl  = NULL;
    if (invc)  delete invc;  invc  = NULL;
    if (invr)  delete invr;  invr  = NULL;
    if (invbl) delete invbl; invbl = NULL;
    if (invb)  delete invb;  invb  = NULL;
    if (invbr) delete invbr; invbr = NULL;

    inv.SetTL(NULL);
    inv.SetTop(NULL);
    inv.SetTR(NULL);
    inv.SetLeft(NULL);
    inv.SetCenter(NULL);
    inv.SetRight(NULL);
    inv.SetBL(NULL);
    inv.SetBottom(NULL);
    inv.SetBR(NULL);

#if GLICT_APIREV < 68
#warning Skinned scrollbar buttons need at least GLICT apirev 68
#else
    if (scutl) delete scutl; scutl = NULL;
    if (scut)  delete scut;  scut  = NULL;
    if (scutr) delete scutr; scutr = NULL;
    if (scul)  delete scul;  scul  = NULL;
    if (scuc)  delete scuc;  scuc  = NULL;
    if (scur)  delete scur;  scur  = NULL;
    if (scubl) delete scubl; scubl = NULL;
    if (scub)  delete scub;  scub  = NULL;
    if (scubr) delete scubr; scubr = NULL;

    scu.SetTL(NULL);
    scu.SetTop(NULL);
    scu.SetTR(NULL);
    scu.SetLeft(NULL);
    scu.SetCenter(NULL);
    scu.SetRight(NULL);
    scu.SetBL(NULL);
    scu.SetBottom(NULL);
    scu.SetBR(NULL);

    glictGlobals.scrollbarUpperSkin = NULL;

    if (shutl) delete shutl; shutl = NULL;
    if (shut)  delete shut;  shut  = NULL;
    if (shutr) delete shutr; shutr = NULL;
    if (shul)  delete shul;  shul  = NULL;
    if (shuc)  delete shuc;  shuc  = NULL;
    if (shur)  delete shur;  shur  = NULL;
    if (shubl) delete shubl; shubl = NULL;
    if (shub)  delete shub;  shub  = NULL;
    if (shubr) delete shubr; shubr = NULL;

    shu.SetTL(NULL);
    shu.SetTop(NULL);
    shu.SetTR(NULL);
    shu.SetLeft(NULL);
    shu.SetCenter(NULL);
    shu.SetRight(NULL);
    shu.SetBL(NULL);
    shu.SetBottom(NULL);
    shu.SetBR(NULL);

    glictGlobals.scrollbarUpperHighlightSkin = NULL;


    if (scdtl) delete scdtl; scdtl = NULL;
    if (scdt)  delete scdt;  scdt  = NULL;
    if (scdtr) delete scdtr; scdtr = NULL;
    if (scdl)  delete scdl;  scdl  = NULL;
    if (scdc)  delete scdc;  scdc  = NULL;
    if (scdr)  delete scdr;  scdr  = NULL;
    if (scdbl) delete scdbl; scdbl = NULL;
    if (scdb)  delete scdb;  scdb  = NULL;
    if (scdbr) delete scdbr; scdbr = NULL;

    scd.SetTL(NULL);
    scd.SetTop(NULL);
    scd.SetTR(NULL);
    scd.SetLeft(NULL);
    scd.SetCenter(NULL);
    scd.SetRight(NULL);
    scd.SetBL(NULL);
    scd.SetBottom(NULL);
    scd.SetBR(NULL);

    glictGlobals.scrollbarLowerSkin = NULL;

    if (shdtl) delete shdtl; shdtl = NULL;
    if (shdt)  delete shdt;  shdt  = NULL;
    if (shdtr) delete shdtr; shdtr = NULL;
    if (shdl)  delete shdl;  shdl  = NULL;
    if (shdc)  delete shdc;  shdc  = NULL;
    if (shdr)  delete shdr;  shdr  = NULL;
    if (shdbl) delete shdbl; shdbl = NULL;
    if (shdb)  delete shdb;  shdb  = NULL;
    if (shdbr) delete shdbr; shdbr = NULL;

    shd.SetTL(NULL);
    shd.SetTop(NULL);
    shd.SetTR(NULL);
    shd.SetLeft(NULL);
    shd.SetCenter(NULL);
    shd.SetRight(NULL);
    shd.SetBL(NULL);
    shd.SetBottom(NULL);
    shd.SetBR(NULL);

    glictGlobals.scrollbarLowerHighlightSkin = NULL;

    if (scbtl) delete scbtl; scbtl = NULL;
    if (scbt)  delete scbt;  scbt  = NULL;
    if (scbtr) delete scbtr; scbtr = NULL;
    if (scbl)  delete scbl;  scbl  = NULL;
    if (scbc)  delete scbc;  scbc  = NULL;
    if (scbr)  delete scbr;  scbr  = NULL;
    if (scbbl) delete scbbl; scbbl = NULL;
    if (scbb)  delete scbb;  scbb  = NULL;
    if (scbbr) delete scbbr; scbbr = NULL;

    scb.SetTL(NULL);
    scb.SetTop(NULL);
    scb.SetTR(NULL);
    scb.SetLeft(NULL);
    scb.SetCenter(NULL);
    scb.SetRight(NULL);
    scb.SetBL(NULL);
    scb.SetBottom(NULL);
    scb.SetBR(NULL);

    glictGlobals.scrollbarDragSkin = NULL;

    if (scptl) delete scptl; scptl = NULL;
    if (scpt)  delete scpt;  scpt  = NULL;
    if (scptr) delete scptr; scptr = NULL;
    if (scpl)  delete scpl;  scpl  = NULL;
    if (scpc)  delete scpc;  scpc  = NULL;
    if (scpr)  delete scpr;  scpr  = NULL;
    if (scpbl) delete scpbl; scpbl = NULL;
    if (scpb)  delete scpb;  scpb  = NULL;
    if (scpbr) delete scpbr; scpbr = NULL;

    scp.SetTL(NULL);
    scp.SetTop(NULL);
    scp.SetTR(NULL);
    scp.SetLeft(NULL);
    scp.SetCenter(NULL);
    scp.SetRight(NULL);
    scp.SetBL(NULL);
    scp.SetBottom(NULL);
    scp.SetBR(NULL);

    glictGlobals.scrollbarPanelSkin = NULL;
#endif
}

