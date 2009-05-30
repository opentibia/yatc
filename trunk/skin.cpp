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
    windowtl = NULL;
    windowt  = NULL;
    windowtr = NULL;
    windowl  = NULL;
    windowc  = NULL;
    windowr  = NULL;
    windowbl = NULL;
    windowb  = NULL;
    windowbr = NULL;

    btnntl = NULL;
    btnnt  = NULL;
    btnntr = NULL;
    btnnl  = NULL;
    btnnc  = NULL;
    btnnr  = NULL;
    btnnbl = NULL;
    btnnb  = NULL;
    btnnbr = NULL;

    btnhtl = NULL;
    btnht  = NULL;
    btnhtr = NULL;
    btnhl  = NULL;
    btnhc  = NULL;
    btnhr  = NULL;
    btnhbl = NULL;
    btnhb  = NULL;
    btnhbr = NULL;

    txttl = NULL;
    txtt  = NULL;
    txttr = NULL;
    txtl  = NULL;
    txtc  = NULL;
    txtr  = NULL;
    txtbl = NULL;
    txtb  = NULL;
    txtbr = NULL;

    scrollbaruptl = NULL;
    scrollbarupt  = NULL;
    scrollbaruptr = NULL;
    scrollbarupl  = NULL;
    scrollbarupc  = NULL;
    scrollbarupr  = NULL;
    scrollbarupbl = NULL;
    scrollbarupb  = NULL;
    scrollbarupbr = NULL;

    scrollbaruphtl = NULL;
    scrollbarupht  = NULL;
    scrollbaruphtr = NULL;
    scrollbaruphl  = NULL;
    scrollbaruphc  = NULL;
    scrollbaruphr  = NULL;
    scrollbaruphbl = NULL;
    scrollbaruphb  = NULL;
    scrollbaruphbr = NULL;

    scrollbardntl = NULL;
    scrollbardnt  = NULL;
    scrollbardntr = NULL;
    scrollbardnl  = NULL;
    scrollbardnc  = NULL;
    scrollbardnr  = NULL;
    scrollbardnbl = NULL;
    scrollbardnb  = NULL;
    scrollbardnbr = NULL;

    scrollbardnhtl = NULL;
    scrollbardnht  = NULL;
    scrollbardnhtr = NULL;
    scrollbardnhl  = NULL;
    scrollbardnhc  = NULL;
    scrollbardnhr  = NULL;
    scrollbardnhbl = NULL;
    scrollbardnhb  = NULL;
    scrollbardnhbr = NULL;

    scrollbarlttl = NULL;
    scrollbarltt  = NULL;
    scrollbarlttr = NULL;
    scrollbarltl  = NULL;
    scrollbarltc  = NULL;
    scrollbarltr  = NULL;
    scrollbarltbl = NULL;
    scrollbarltb  = NULL;
    scrollbarltbr = NULL;

    scrollbarlthtl = NULL;
    scrollbarltht  = NULL;
    scrollbarlthtr = NULL;
    scrollbarlthl  = NULL;
    scrollbarlthc  = NULL;
    scrollbarlthr  = NULL;
    scrollbarlthbl = NULL;
    scrollbarlthb  = NULL;
    scrollbarlthbr = NULL;

    scrollbarrttl = NULL;
    scrollbarrtt  = NULL;
    scrollbarrttr = NULL;
    scrollbarrtl  = NULL;
    scrollbarrtc  = NULL;
    scrollbarrtr  = NULL;
    scrollbarrtbl = NULL;
    scrollbarrtb  = NULL;
    scrollbarrtbr = NULL;

    scrollbarrthtl = NULL;
    scrollbarrtht  = NULL;
    scrollbarrthtr = NULL;
    scrollbarrthl  = NULL;
    scrollbarrthc  = NULL;
    scrollbarrthr  = NULL;
    scrollbarrthbl = NULL;
    scrollbarrthb  = NULL;
    scrollbarrthbr = NULL;

    scrollbarbtntl = NULL;
    scrollbarbtnt  = NULL;
    scrollbarbtntr = NULL;
    scrollbarbtnl  = NULL;
    scrollbarbtnc  = NULL;
    scrollbarbtnr  = NULL;
    scrollbarbtnbl = NULL;
    scrollbarbtnb  = NULL;
    scrollbarbtnbr = NULL;

    scrollbarbgtl = NULL;
    scrollbarbgt  = NULL;
    scrollbarbgtr = NULL;
    scrollbarbgl  = NULL;
    scrollbarbgc  = NULL;
    scrollbarbgr  = NULL;
    scrollbarbgbl = NULL;
    scrollbarbgb  = NULL;
    scrollbarbgbr = NULL;



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

    consoletabbgtl = NULL;
    consoletabbgt  = NULL;
    consoletabbgtr = NULL;
    consoletabbgl  = NULL;
    consoletabbgc  = NULL;
    consoletabbgr  = NULL;
    consoletabbgbl = NULL;
    consoletabbgb  = NULL;
    consoletabbgbr = NULL;

    consoletabactivetl = NULL;
    consoletabactivet  = NULL;
    consoletabactivetr = NULL;
    consoletabactivel  = NULL;
    consoletabactivec  = NULL;
    consoletabactiver  = NULL;
    consoletabactivebl = NULL;
    consoletabactiveb  = NULL;
    consoletabactivebr = NULL;

    consoletabpassivetl = NULL;
    consoletabpassivet  = NULL;
    consoletabpassivetr = NULL;
    consoletabpassivel  = NULL;
    consoletabpassivec  = NULL;
    consoletabpassiver  = NULL;
    consoletabpassivebl = NULL;
    consoletabpassiveb  = NULL;
    consoletabpassivebr = NULL;

    compasstl = NULL;
    compasst  = NULL;
    compasstr = NULL;
    compassl  = NULL;
    compassc  = NULL;
    compassr  = NULL;
    compassbl = NULL;
    compassb  = NULL;
    compassbr = NULL;

    for (int i = 0; i < 11; i++)
    {
        graphicbtntl[i] = NULL;
        graphicbtnt[i]  = NULL;
        graphicbtntr[i] = NULL;
        graphicbtnl[i]  = NULL;
        graphicbtnc[i]  = NULL;
        graphicbtnr[i]  = NULL;
        graphicbtnbl[i] = NULL;
        graphicbtnb[i]  = NULL;
        graphicbtnbr[i] = NULL;

        graphicbthtl[i] = NULL;
        graphicbtht[i]  = NULL;
        graphicbthtr[i] = NULL;
        graphicbthl[i]  = NULL;
        graphicbthc[i]  = NULL;
        graphicbthr[i]  = NULL;
        graphicbthbl[i] = NULL;
        graphicbthb[i]  = NULL;
        graphicbthbr[i] = NULL;


    }

    backgroundtl = NULL;
    backgroundt  = NULL;
    backgroundtr = NULL;
    backgroundl  = NULL;
    backgroundc  = NULL;
    backgroundr  = NULL;
    backgroundbl = NULL;
    backgroundb  = NULL;
    backgroundbr = NULL;

}
Skin::~Skin() {
    unloadSkin();

}
void Skin::loadSkin() {
    window.SetTL		(windowtl = new skinImage(106,	183,	4,		17));
    window.SetTop		(windowt  = new skinImage(114,	183,	96,		17));
    window.SetTR		(windowtr = new skinImage(110,	183,	4,		17));
    window.SetLeft		(windowl  = new skinImage(256,	0,		4,		96));
    window.SetCenter	(windowc  = new skinImage(0,	0,		96,		96));
    window.SetRight	(windowr  = new skinImage(260,	0,		4,		96));
    window.SetBL		(windowbl = new skinImage(98, 	193, 	4, 		4));
    window.SetBottom	(windowb  = new skinImage(2,	193,	96,		4));
    window.SetBR		(windowbr = new skinImage(102,	193,	4,		4));

	glictGlobals.windowBodySkin = &window;

    btnn.SetTL		(btnntl = new skinImage(174,	138,	1,		1));
    btnn.SetTop		(btnnt  = new skinImage(175,	138,	32,		1));
    btnn.SetTR		(btnntr = new skinImage(207,	138,	1,		1));
    btnn.SetLeft	(btnnl  = new skinImage(174,	139,	1,		18));
    btnn.SetCenter	(btnnc  = new skinImage(175,	139,	32,		18));
    btnn.SetRight	(btnnr  = new skinImage(207,	139,	1,		18));
    btnn.SetBL		(btnnbl = new skinImage(174,	157,	1,		1));
    btnn.SetBottom	(btnnb  = new skinImage(175,	157,	32,		1));
    btnn.SetBR		(btnnbr = new skinImage(207,	157,	1,		1));

	glictGlobals.buttonSkin = &btnn;

    btnh.SetTL		(btnhtl = new skinImage(174,	158,	1,		1));
    btnh.SetTop		(btnht  = new skinImage(175,	158,	32,		1));
    btnh.SetTR		(btnhtr = new skinImage(207,	158,	1,		1));
    btnh.SetLeft	(btnhl  = new skinImage(174,	159,	1,		18));
    btnh.SetCenter	(btnhc  = new skinImage(175,	159,	32,		18));
    btnh.SetRight	(btnhr  = new skinImage(207,	159,	1,		18));
    btnh.SetBL		(btnhbl = new skinImage(174,	177,	1,		1));
    btnh.SetBottom	(btnhb  = new skinImage(175,	177,	32,		1));
    btnh.SetBR		(btnhbr = new skinImage(207,	177,	1,		1));

	glictGlobals.buttonHighlightSkin = &btnh;

    // FIXME (ivucica#4#) looks like the "unchecked checkbox" is not the same as "textbox" -- see: "XPlike Tibia.pic" for more info
    txt.SetTL		(txttl = new skinImage(308,	96,		1,		1)); // in fact it looks like we have to HARDCODE this piece of skin.
    txt.SetTop		(txtt  = new skinImage(309,	96,		10,		1));
    txt.SetTR		(txttr = new skinImage(319,	96,		1,		1));
    txt.SetLeft		(txtl  = new skinImage(308,	97,		1,		10));
    txt.SetCenter	(txtc  = new skinImage(309,	97,		10,		10));
    txt.SetRight	    (txtr  = new skinImage(319,	97,		1,		10));
    txt.SetBL		(txtbl = new skinImage(308,	107,	1,		1));
    txt.SetBottom	(txtb  = new skinImage(309,	107,	10,		1));
    txt.SetBR		(txtbr = new skinImage(319,	107,	1,		1));

	glictGlobals.textboxSkin = &txt;

#if GLICT_APIREV < 68
#warning Skinned scrollbar buttons need at least GLICT apirev 68
#else
    scrollbarup.SetTL		(scrollbaruptl = NULL);
    scrollbarup.SetTop		(scrollbarupt  = NULL);
    scrollbarup.SetTR		(scrollbaruptr = NULL);
    scrollbarup.SetLeft		(scrollbarupl  = NULL);
    scrollbarup.SetCenter	(scrollbarupc  = new skinImage(232,		64,	    12,		12));
    scrollbarup.SetRight	(scrollbarupr  = NULL);
    scrollbarup.SetBL		(scrollbarupbl = NULL);
    scrollbarup.SetBottom	(scrollbarupb  = NULL);
    scrollbarup.SetBR		(scrollbarupbr = NULL);

    glictGlobals.scrollbarUpperSkin = &scrollbarup;

    scrollbaruph.SetTL		(scrollbaruphtl = NULL);
    scrollbaruph.SetTop		(scrollbarupht  = NULL);
    scrollbaruph.SetTR		(scrollbaruphtr = NULL);
    scrollbaruph.SetLeft	(scrollbaruphl  = NULL);
    scrollbaruph.SetCenter	(scrollbaruphc  = new skinImage(234,		122,    12,		12));
    scrollbaruph.SetRight	(scrollbaruphr  = NULL);
    scrollbaruph.SetBL		(scrollbaruphbl = NULL);
    scrollbaruph.SetBottom	(scrollbaruphb  = NULL);
    scrollbaruph.SetBR		(scrollbaruphbr = NULL);

    glictGlobals.scrollbarUpperHighlightSkin = &scrollbaruph;

    scrollbardn.SetTL		(scrollbardntl = NULL);
    scrollbardn.SetTop		(scrollbardnt  = NULL);
    scrollbardn.SetTR		(scrollbardntr = NULL);
    scrollbardn.SetLeft		(scrollbardnl  = NULL);
    scrollbardn.SetCenter	(scrollbardnc  = new skinImage(244,		64,	    12,		12));
    scrollbardn.SetRight	(scrollbardnr  = NULL);
    scrollbardn.SetBL		(scrollbardnbl = NULL);
    scrollbardn.SetBottom	(scrollbardnb  = NULL);
    scrollbardn.SetBR		(scrollbardnbr = NULL);

    glictGlobals.scrollbarLowerSkin = &scrollbardn;

    scrollbardnh.SetTL		(scrollbardnhtl = NULL);
    scrollbardnh.SetTop		(scrollbardnht  = NULL);
    scrollbardnh.SetTR		(scrollbardnhtr = NULL);
    scrollbardnh.SetLeft	(scrollbardnhl  = NULL);
    scrollbardnh.SetCenter	(scrollbardnhc  = new skinImage(246,		122,    12,		12));
    scrollbardnh.SetRight	(scrollbardnhr  = NULL);
    scrollbardnh.SetBL		(scrollbardnhbl = NULL);
    scrollbardnh.SetBottom	(scrollbardnhb  = NULL);
    scrollbardnh.SetBR		(scrollbardnhbr = NULL);

    glictGlobals.scrollbarLowerHighlightSkin = &scrollbardnh;

    scrollbarlt.SetTL		(scrollbarlttl = NULL);
    scrollbarlt.SetTop		(scrollbarltt  = NULL);
    scrollbarlt.SetTR		(scrollbarlttr = NULL);
    scrollbarlt.SetLeft		(scrollbarltl  = NULL);
    scrollbarlt.SetCenter	(scrollbarltc  = new skinImage(232,		76,	    12,		12));
    scrollbarlt.SetRight	(scrollbarltr  = NULL);
    scrollbarlt.SetBL		(scrollbarltbl = NULL);
    scrollbarlt.SetBottom	(scrollbarltb  = NULL);
    scrollbarlt.SetBR		(scrollbarltbr = NULL);

    glictGlobals.scrollbarLeftSkin = &scrollbarlt;

    scrollbarlth.SetTL		(scrollbarlthtl = NULL);
    scrollbarlth.SetTop		(scrollbarltht  = NULL);
    scrollbarlth.SetTR		(scrollbarlthtr = NULL);
    scrollbarlth.SetLeft	(scrollbarlthl  = NULL);
    scrollbarlth.SetCenter	(scrollbarlthc  = new skinImage(234,		134,    12,		12));
    scrollbarlth.SetRight	(scrollbarlthr  = NULL);
    scrollbarlth.SetBL		(scrollbarlthbl = NULL);
    scrollbarlth.SetBottom	(scrollbarlthb  = NULL);
    scrollbarlth.SetBR		(scrollbarlthbr = NULL);

    glictGlobals.scrollbarLeftHighlightSkin = &scrollbarlth;

    scrollbarrt.SetTL		(scrollbarrttl = NULL);
    scrollbarrt.SetTop		(scrollbarrtt  = NULL);
    scrollbarrt.SetTR		(scrollbarrttr = NULL);
    scrollbarrt.SetLeft		(scrollbarrtl  = NULL);
    scrollbarrt.SetCenter	(scrollbarrtc  = new skinImage(244,		76,	    12,		12));
    scrollbarrt.SetRight	(scrollbarrtr  = NULL);
    scrollbarrt.SetBL		(scrollbarrtbl = NULL);
    scrollbarrt.SetBottom	(scrollbarrtb  = NULL);
    scrollbarrt.SetBR		(scrollbarrtbr = NULL);

    glictGlobals.scrollbarRightSkin = &scrollbarrt;

    scrollbarrth.SetTL		(scrollbarrthtl = NULL);
    scrollbarrth.SetTop		(scrollbarrtht  = NULL);
    scrollbarrth.SetTR		(scrollbarrthtr = NULL);
    scrollbarrth.SetLeft	(scrollbarrthl  = NULL);
    scrollbarrth.SetCenter	(scrollbarrthc  = new skinImage(246,		134,    12,		12));
    scrollbarrth.SetRight	(scrollbarrthr  = NULL);
    scrollbarrth.SetBL		(scrollbarrthbl = NULL);
    scrollbarrth.SetBottom	(scrollbarrthb  = NULL);
    scrollbarrth.SetBR		(scrollbarrthbr = NULL);

    glictGlobals.scrollbarRightHighlightSkin = &scrollbarrth;

    scrollbarbtn.SetTL		(scrollbarbtntl = NULL);
    scrollbarbtn.SetTop		(scrollbarbtnt  = NULL);
    scrollbarbtn.SetTR		(scrollbarbtntr = NULL);
    scrollbarbtn.SetLeft	(scrollbarbtnl  = NULL);
    scrollbarbtn.SetCenter	(scrollbarbtnc  = new skinImage(220,		64,    12,		12));
    scrollbarbtn.SetRight	(scrollbarbtnr  = NULL);
    scrollbarbtn.SetBL		(scrollbarbtnbl = NULL);
    scrollbarbtn.SetBottom	(scrollbarbtnb  = NULL);
    scrollbarbtn.SetBR		(scrollbarbtnbr = NULL);

    glictGlobals.scrollbarDragSkin = &scrollbarbtn;

    scrollbarbg.SetTL		(scrollbarbgtl = NULL);
    scrollbarbg.SetTop		(scrollbarbgt  = NULL);
    scrollbarbg.SetTR		(scrollbarbgtr = NULL);
    scrollbarbg.SetLeft		(scrollbarbgl  = new skinImage(264,		0,     1,		96));
    scrollbarbg.SetCenter	(scrollbarbgc  = new skinImage(265,		0,     10,		96));
    scrollbarbg.SetRight	(scrollbarbgr  = new skinImage(275,		0,     1,		96));
    scrollbarbg.SetBL		(scrollbarbgbl = NULL);
    scrollbarbg.SetBottom	(scrollbarbgb  = NULL);
    scrollbarbg.SetBR		(scrollbarbgbr = NULL);

    glictGlobals.scrollbarPanelSkin = &scrollbarbg;
#endif


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

    consoletabbg.SetTL		(consoletabbgtl = new skinImage(210,     182,    2,      2));
    consoletabbg.SetTop		(consoletabbgt  = new skinImage(212,     182,    96,     2));
    consoletabbg.SetTR		(consoletabbgtr = new skinImage(308,     182,    2,      2));
    consoletabbg.SetLeft	(consoletabbgl  = new skinImage(210,     184,    2,      14));
    consoletabbg.SetCenter	(consoletabbgc  = new skinImage(212,     184,    96,     14));
    consoletabbg.SetRight	(consoletabbgr  = new skinImage(308,     184,    2,      14));
    consoletabbg.SetBL		(consoletabbgbl = new skinImage(210,     198,    2,      2));
    consoletabbg.SetBottom	(consoletabbgb  = new skinImage(212,     198,    96,     2));
    consoletabbg.SetBR		(consoletabbgbr = new skinImage(308,     198,    2,      2));

    consoletabactive.SetTL		(consoletabactivetl = new skinImage(114,     200,    2,      2));
    consoletabactive.SetTop		(consoletabactivet  = new skinImage(116,     200,    92,     2));
    consoletabactive.SetTR		(consoletabactivetr = new skinImage(208,     200,    2,      2));
    consoletabactive.SetLeft	(consoletabactivel  = new skinImage(114,     202,    2,      14));
    consoletabactive.SetCenter	(consoletabactivec  = new skinImage(116,     202,    92,     14));
    consoletabactive.SetRight	(consoletabactiver  = new skinImage(208,     202,    2,      14));
    consoletabactive.SetBL		(consoletabactivebl = new skinImage(114,     216,    2,      2));
    consoletabactive.SetBottom	(consoletabactiveb  = new skinImage(116,     216,    92,     2));
    consoletabactive.SetBR		(consoletabactivebr = new skinImage(208,     216,    2,      2));

    consoletabpassive.SetTL		(consoletabpassivetl = new skinImage(114+96,  200,    2,      2));
    consoletabpassive.SetTop	(consoletabpassivet  = new skinImage(116+96,  200,    92,     2));
    consoletabpassive.SetTR		(consoletabpassivetr = new skinImage(208+96,  200,    2,      2));
    consoletabpassive.SetLeft	(consoletabpassivel  = new skinImage(114+96,  202,    2,      14));
    consoletabpassive.SetCenter	(consoletabpassivec  = new skinImage(116+96,  202,    92,     14));
    consoletabpassive.SetRight	(consoletabpassiver  = new skinImage(208+96,  202,    2,      14));
    consoletabpassive.SetBL		(consoletabpassivebl = new skinImage(114+96,  216,    2,      2));
    consoletabpassive.SetBottom	(consoletabpassiveb  = new skinImage(116+96,  216,    92,     2));
    consoletabpassive.SetBR		(consoletabpassivebr = new skinImage(208+96,  216,    2,      2));

    compass.SetTL		(compasstl = NULL);
    compass.SetTop	    (compasst  = NULL);
    compass.SetTR		(compasstr = NULL);
    compass.SetLeft	    (compassl  = NULL);
    compass.SetCenter	(compassc  = new skinImage(131, 138, 43, 43));
    compass.SetRight	(compassr  = NULL);
    compass.SetBL		(compassbl = NULL);
    compass.SetBottom	(compassb  = NULL);
    compass.SetBR		(compassbr = NULL);

	// some random buttons, many of them go on the inventory panel in-game
    for (int i = BUTTON_ZOOMIN; i <= BUTTON_HELPMINI; i++)
    {
        graphicbtn[i].SetCenter		(graphicbtnc [i] = new skinImage(2 + 20*i, 98, 20, 20));
        graphicbth[i].SetCenter		(graphicbthc [i] = new skinImage(2 + 20*i, 98+20, 20, 20));
    }
    // options button when inventory is collapsed
    {
		graphicbtn[BUTTON_OPTIONSMINI].SetCenter	(graphicbtnc [BUTTON_OPTIONSMINI] = new skinImage(312, 128, 20, 20));
		graphicbth[BUTTON_OPTIONSMINI].SetCenter	(graphicbtnc [BUTTON_OPTIONSMINI] = new skinImage(332, 128, 20, 20));
    }
    // buttons that go above the console
    for (int i = BUTTON_CONSOLE_CLOSE; i <= BUTTON_CONSOLE_IGNORELIST; i++)
    {
        graphicbtn[i].SetCenter	    (graphicbtnc [i] = new skinImage(208 + 16*(i-BUTTON_IND_CONSOLE), 146, 16, 16));
        graphicbth[i].SetCenter	    (graphicbthc [i] = new skinImage(208 + 16*(i-BUTTON_IND_CONSOLE), 146+16, 16, 16));
    }
    // buttons that go next to the console entry
    for (int i = BUTTON_CONSOLE_YELL; i <= BUTTON_CONSOLE_WHISPER; i++)
    {
        graphicbtn[i].SetCenter	    (graphicbtnc [i] = new skinImage(258 + 16*(i-BUTTON_CONSOLE_IGNORELIST), 144, 16, 16));
        graphicbth[i].SetCenter	    (graphicbthc [i] = new skinImage(258 + 16*(i-BUTTON_CONSOLE_IGNORELIST), 144+16, 16, 16));
    }
    // buttons that go in window headers
    for (int i = BUTTON_CLOSE_WINDOW; i <= BUTTON_CONTAINER_PARENT; i++)
    {
        graphicbtn[i].SetCenter	    (graphicbtnc [i] = new skinImage(222 + 12*(i-BUTTON_IND_WINDOW), 98, 12, 12));
        graphicbth[i].SetCenter	    (graphicbthc [i] = new skinImage(222 + 12*(i-BUTTON_IND_WINDOW), 98+12, 12, 12));
    }

	graphicbtn[BUTTON_EXPAND_WINDOW].SetCenter(graphicbtnc [BUTTON_EXPAND_WINDOW] = new skinImage(308, 158, 12, 12));
	graphicbth[BUTTON_EXPAND_WINDOW].SetCenter(graphicbthc [BUTTON_EXPAND_WINDOW] = new skinImage(308, 158+12, 12, 12));

    background.SetTL		(backgroundtl = NULL);
    background.SetTop	    (backgroundt  = NULL);
    background.SetTR		(backgroundtr = NULL);
    background.SetLeft	    (backgroundl  = NULL);
    background.SetCenter	(backgroundc  = new skinImage(0,  0,    96,     96));
    background.SetRight	    (backgroundr  = NULL);
    background.SetBL		(backgroundbl = NULL);
    background.SetBottom	(backgroundb  = NULL);
    background.SetBR		(backgroundbr = NULL);

    ptb.SetTL		(ptbtl = new skinImage(98, 201, 3, 3));
    ptb.SetTop		(ptbt = new skinImage(2, 211, 96, 3));
    ptb.SetTR		(ptbtr = new skinImage(101, 201, 3, 3));
    ptb.SetLeft		(ptbl = new skinImage(260, 0, 3, 96));
    ptb.SetCenter	(ptbc = backgroundc);
    ptb.SetRight	(ptbr = ptbl);
    ptb.SetBL		(ptbbl = new skinImage(98, 204, 3, 3));
    ptb.SetBottom	(ptbb = ptbt);
    ptb.SetBR		(ptbbr = new skinImage(101, 204, 3, 3));

    rsp.SetTL		(rstl = consoletabactivetl);
    rsp.SetTop		(rst = consoletabactivet);
    rsp.SetTR		(rstr = consoletabactivetr);
    rsp.SetLeft		(rsl = consoletabactivel);
    rsp.SetCenter	(rsc = backgroundc);
    rsp.SetRight	(rsr = new skinImage(0, 98, 2, 96));
    rsp.SetBL		(rsbl = new skinImage(98, 195, 2, 2));
    rsp.SetBottom	(rsb = new skinImage(0, 96, 96, 2));
    rsp.SetBR		(rsbr = new skinImage(104, 195, 2, 2));
}

void Skin::unloadSkin() {
    if (windowtl) delete windowtl; windowtl = NULL;
    if (windowt)  delete windowt;  windowt  = NULL;
    if (windowtr) delete windowtr; windowtr = NULL;
    if (windowl)  delete windowl;  windowl  = NULL;
    if (windowc)  delete windowc;  windowc  = NULL;
    if (windowr)  delete windowr;  windowr  = NULL;
    if (windowbl) delete windowbl; windowbl = NULL;
    if (windowb)  delete windowb;  windowb  = NULL;
    if (windowbr) delete windowbr; windowbr = NULL;



    window.SetTL(NULL);
    window.SetTop(NULL);
    window.SetTR(NULL);
    window.SetLeft(NULL);
    window.SetCenter(NULL);
    window.SetRight(NULL);
    window.SetBL(NULL);
    window.SetBottom(NULL);
    window.SetBR(NULL);

    glictGlobals.windowBodySkin = NULL;

    if (btnntl) delete btnntl; btnntl = NULL;
    if (btnnt)  delete btnnt;  btnnt  = NULL;
    if (btnntr) delete btnntr; btnntr = NULL;
    if (btnnl)  delete btnnl;  btnnl  = NULL;
    if (btnnc)  delete btnnc;  btnnc  = NULL;
    if (btnnr)  delete btnnr;  btnnr  = NULL;
    if (btnnbl) delete btnnbl; btnnbl = NULL;
    if (btnnb)  delete btnnb;  btnnb  = NULL;
    if (btnnbr) delete btnnbr; btnnbr = NULL;

    btnn.SetTL(NULL);
    btnn.SetTop(NULL);
    btnn.SetTR(NULL);
    btnn.SetLeft(NULL);
    btnn.SetCenter(NULL);
    btnn.SetRight(NULL);
    btnn.SetBL(NULL);
    btnn.SetBottom(NULL);
    btnn.SetBR(NULL);

    glictGlobals.buttonSkin = NULL;

    if (btnhtl) delete btnhtl; btnhtl = NULL;
    if (btnht)  delete btnht;  btnht  = NULL;
    if (btnhtr) delete btnhtr; btnhtr = NULL;
    if (btnhl)  delete btnhl;  btnhl  = NULL;
    if (btnhc)  delete btnhc;  btnhc  = NULL;
    if (btnhr)  delete btnhr;  btnhr  = NULL;
    if (btnhbl) delete btnhbl; btnhbl = NULL;
    if (btnhb)  delete btnhb;  btnhb  = NULL;
    if (btnhbr) delete btnhbr; btnhbr = NULL;

    btnh.SetTL(NULL);
    btnh.SetTop(NULL);
    btnh.SetTR(NULL);
    btnh.SetLeft(NULL);
    btnh.SetCenter(NULL);
    btnh.SetRight(NULL);
    btnh.SetBL(NULL);
    btnh.SetBottom(NULL);
    btnh.SetBR(NULL);

    glictGlobals.buttonHighlightSkin = NULL;

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


#if GLICT_APIREV < 68
#warning Skinned scrollbar buttons need at least GLICT apirev 68
#else
    if (scrollbaruptl) delete scrollbaruptl; scrollbaruptl = NULL;
    if (scrollbarupt)  delete scrollbarupt;  scrollbarupt  = NULL;
    if (scrollbaruptr) delete scrollbaruptr; scrollbaruptr = NULL;
    if (scrollbarupl)  delete scrollbarupl;  scrollbarupl  = NULL;
    if (scrollbarupc)  delete scrollbarupc;  scrollbarupc  = NULL;
    if (scrollbarupr)  delete scrollbarupr;  scrollbarupr  = NULL;
    if (scrollbarupbl) delete scrollbarupbl; scrollbarupbl = NULL;
    if (scrollbarupb)  delete scrollbarupb;  scrollbarupb  = NULL;
    if (scrollbarupbr) delete scrollbarupbr; scrollbarupbr = NULL;

    scrollbarup.SetTL(NULL);
    scrollbarup.SetTop(NULL);
    scrollbarup.SetTR(NULL);
    scrollbarup.SetLeft(NULL);
    scrollbarup.SetCenter(NULL);
    scrollbarup.SetRight(NULL);
    scrollbarup.SetBL(NULL);
    scrollbarup.SetBottom(NULL);
    scrollbarup.SetBR(NULL);

    glictGlobals.scrollbarUpperSkin = NULL;

    if (scrollbaruphtl) delete scrollbaruphtl; scrollbaruphtl = NULL;
    if (scrollbarupht)  delete scrollbarupht;  scrollbarupht  = NULL;
    if (scrollbaruphtr) delete scrollbaruphtr; scrollbaruphtr = NULL;
    if (scrollbaruphl)  delete scrollbaruphl;  scrollbaruphl  = NULL;
    if (scrollbaruphc)  delete scrollbaruphc;  scrollbaruphc  = NULL;
    if (scrollbaruphr)  delete scrollbaruphr;  scrollbaruphr  = NULL;
    if (scrollbaruphbl) delete scrollbaruphbl; scrollbaruphbl = NULL;
    if (scrollbaruphb)  delete scrollbaruphb;  scrollbaruphb  = NULL;
    if (scrollbaruphbr) delete scrollbaruphbr; scrollbaruphbr = NULL;

    scrollbaruph.SetTL(NULL);
    scrollbaruph.SetTop(NULL);
    scrollbaruph.SetTR(NULL);
    scrollbaruph.SetLeft(NULL);
    scrollbaruph.SetCenter(NULL);
    scrollbaruph.SetRight(NULL);
    scrollbaruph.SetBL(NULL);
    scrollbaruph.SetBottom(NULL);
    scrollbaruph.SetBR(NULL);

    glictGlobals.scrollbarUpperHighlightSkin = NULL;


    if (scrollbardntl) delete scrollbardntl; scrollbardntl = NULL;
    if (scrollbardnt)  delete scrollbardnt;  scrollbardnt  = NULL;
    if (scrollbardntr) delete scrollbardntr; scrollbardntr = NULL;
    if (scrollbardnl)  delete scrollbardnl;  scrollbardnl  = NULL;
    if (scrollbardnc)  delete scrollbardnc;  scrollbardnc  = NULL;
    if (scrollbardnr)  delete scrollbardnr;  scrollbardnr  = NULL;
    if (scrollbardnbl) delete scrollbardnbl; scrollbardnbl = NULL;
    if (scrollbardnb)  delete scrollbardnb;  scrollbardnb  = NULL;
    if (scrollbardnbr) delete scrollbardnbr; scrollbardnbr = NULL;

    scrollbardn.SetTL(NULL);
    scrollbardn.SetTop(NULL);
    scrollbardn.SetTR(NULL);
    scrollbardn.SetLeft(NULL);
    scrollbardn.SetCenter(NULL);
    scrollbardn.SetRight(NULL);
    scrollbardn.SetBL(NULL);
    scrollbardn.SetBottom(NULL);
    scrollbardn.SetBR(NULL);

    glictGlobals.scrollbarLowerSkin = NULL;

    if (scrollbardnhtl) delete scrollbardnhtl; scrollbardnhtl = NULL;
    if (scrollbardnht)  delete scrollbardnht;  scrollbardnht  = NULL;
    if (scrollbardnhtr) delete scrollbardnhtr; scrollbardnhtr = NULL;
    if (scrollbardnhl)  delete scrollbardnhl;  scrollbardnhl  = NULL;
    if (scrollbardnhc)  delete scrollbardnhc;  scrollbardnhc  = NULL;
    if (scrollbardnhr)  delete scrollbardnhr;  scrollbardnhr  = NULL;
    if (scrollbardnhbl) delete scrollbardnhbl; scrollbardnhbl = NULL;
    if (scrollbardnhb)  delete scrollbardnhb;  scrollbardnhb  = NULL;
    if (scrollbardnhbr) delete scrollbardnhbr; scrollbardnhbr = NULL;

    scrollbardnh.SetTL(NULL);
    scrollbardnh.SetTop(NULL);
    scrollbardnh.SetTR(NULL);
    scrollbardnh.SetLeft(NULL);
    scrollbardnh.SetCenter(NULL);
    scrollbardnh.SetRight(NULL);
    scrollbardnh.SetBL(NULL);
    scrollbardnh.SetBottom(NULL);
    scrollbardnh.SetBR(NULL);

    glictGlobals.scrollbarLowerHighlightSkin = NULL;

    if (scrollbarlttl) delete scrollbarlttl; scrollbarlttl = NULL;
    if (scrollbarltt)  delete scrollbarltt;  scrollbarltt  = NULL;
    if (scrollbarlttr) delete scrollbarlttr; scrollbarlttr = NULL;
    if (scrollbarltl)  delete scrollbarltl;  scrollbarltl  = NULL;
    if (scrollbarltc)  delete scrollbarltc;  scrollbarltc  = NULL;
    if (scrollbarltr)  delete scrollbarltr;  scrollbarltr  = NULL;
    if (scrollbarltbl) delete scrollbarltbl; scrollbarltbl = NULL;
    if (scrollbarltb)  delete scrollbarltb;  scrollbarltb  = NULL;
    if (scrollbarltbr) delete scrollbarltbr; scrollbarltbr = NULL;

    scrollbarlt.SetTL(NULL);
    scrollbarlt.SetTop(NULL);
    scrollbarlt.SetTR(NULL);
    scrollbarlt.SetLeft(NULL);
    scrollbarlt.SetCenter(NULL);
    scrollbarlt.SetRight(NULL);
    scrollbarlt.SetBL(NULL);
    scrollbarlt.SetBottom(NULL);
    scrollbarlt.SetBR(NULL);

    glictGlobals.scrollbarLeftSkin = NULL;

    if (scrollbarlthtl) delete scrollbarlthtl; scrollbarlthtl = NULL;
    if (scrollbarltht)  delete scrollbarltht;  scrollbarltht  = NULL;
    if (scrollbarlthtr) delete scrollbarlthtr; scrollbarlthtr = NULL;
    if (scrollbarlthl)  delete scrollbarlthl;  scrollbarlthl  = NULL;
    if (scrollbarlthc)  delete scrollbarlthc;  scrollbarlthc  = NULL;
    if (scrollbarlthr)  delete scrollbarlthr;  scrollbarlthr  = NULL;
    if (scrollbarlthbl) delete scrollbarlthbl; scrollbarlthbl = NULL;
    if (scrollbarlthb)  delete scrollbarlthb;  scrollbarlthb  = NULL;
    if (scrollbarlthbr) delete scrollbarlthbr; scrollbarlthbr = NULL;

    scrollbarlth.SetTL(NULL);
    scrollbarlth.SetTop(NULL);
    scrollbarlth.SetTR(NULL);
    scrollbarlth.SetLeft(NULL);
    scrollbarlth.SetCenter(NULL);
    scrollbarlth.SetRight(NULL);
    scrollbarlth.SetBL(NULL);
    scrollbarlth.SetBottom(NULL);
    scrollbarlth.SetBR(NULL);

    glictGlobals.scrollbarLeftHighlightSkin = NULL;


    if (scrollbarrttl) delete scrollbarrttl; scrollbarrttl = NULL;
    if (scrollbarrtt)  delete scrollbarrtt;  scrollbarrtt  = NULL;
    if (scrollbarrttr) delete scrollbarrttr; scrollbarrttr = NULL;
    if (scrollbarrtl)  delete scrollbarrtl;  scrollbarrtl  = NULL;
    if (scrollbarrtc)  delete scrollbarrtc;  scrollbarrtc  = NULL;
    if (scrollbarrtr)  delete scrollbarrtr;  scrollbarrtr  = NULL;
    if (scrollbarrtbl) delete scrollbarrtbl; scrollbarrtbl = NULL;
    if (scrollbarrtb)  delete scrollbarrtb;  scrollbarrtb  = NULL;
    if (scrollbarrtbr) delete scrollbarrtbr; scrollbarrtbr = NULL;

    scrollbarrt.SetTL(NULL);
    scrollbarrt.SetTop(NULL);
    scrollbarrt.SetTR(NULL);
    scrollbarrt.SetLeft(NULL);
    scrollbarrt.SetCenter(NULL);
    scrollbarrt.SetRight(NULL);
    scrollbarrt.SetBL(NULL);
    scrollbarrt.SetBottom(NULL);
    scrollbarrt.SetBR(NULL);

    glictGlobals.scrollbarRightSkin = NULL;

    if (scrollbarrthtl) delete scrollbarrthtl; scrollbarrthtl = NULL;
    if (scrollbarrtht)  delete scrollbarrtht;  scrollbarrtht  = NULL;
    if (scrollbarrthtr) delete scrollbarrthtr; scrollbarrthtr = NULL;
    if (scrollbarrthl)  delete scrollbarrthl;  scrollbarrthl  = NULL;
    if (scrollbarrthc)  delete scrollbarrthc;  scrollbarrthc  = NULL;
    if (scrollbarrthr)  delete scrollbarrthr;  scrollbarrthr  = NULL;
    if (scrollbarrthbl) delete scrollbarrthbl; scrollbarrthbl = NULL;
    if (scrollbarrthb)  delete scrollbarrthb;  scrollbarrthb  = NULL;
    if (scrollbarrthbr) delete scrollbarrthbr; scrollbarrthbr = NULL;

    scrollbarrth.SetTL(NULL);
    scrollbarrth.SetTop(NULL);
    scrollbarrth.SetTR(NULL);
    scrollbarrth.SetLeft(NULL);
    scrollbarrth.SetCenter(NULL);
    scrollbarrth.SetRight(NULL);
    scrollbarrth.SetBL(NULL);
    scrollbarrth.SetBottom(NULL);
    scrollbarrth.SetBR(NULL);

    glictGlobals.scrollbarRightHighlightSkin = NULL;

    if (scrollbarbtntl) delete scrollbarbtntl; scrollbarbtntl = NULL;
    if (scrollbarbtnt)  delete scrollbarbtnt;  scrollbarbtnt  = NULL;
    if (scrollbarbtntr) delete scrollbarbtntr; scrollbarbtntr = NULL;
    if (scrollbarbtnl)  delete scrollbarbtnl;  scrollbarbtnl  = NULL;
    if (scrollbarbtnc)  delete scrollbarbtnc;  scrollbarbtnc  = NULL;
    if (scrollbarbtnr)  delete scrollbarbtnr;  scrollbarbtnr  = NULL;
    if (scrollbarbtnbl) delete scrollbarbtnbl; scrollbarbtnbl = NULL;
    if (scrollbarbtnb)  delete scrollbarbtnb;  scrollbarbtnb  = NULL;
    if (scrollbarbtnbr) delete scrollbarbtnbr; scrollbarbtnbr = NULL;

    scrollbarbtn.SetTL(NULL);
    scrollbarbtn.SetTop(NULL);
    scrollbarbtn.SetTR(NULL);
    scrollbarbtn.SetLeft(NULL);
    scrollbarbtn.SetCenter(NULL);
    scrollbarbtn.SetRight(NULL);
    scrollbarbtn.SetBL(NULL);
    scrollbarbtn.SetBottom(NULL);
    scrollbarbtn.SetBR(NULL);

    glictGlobals.scrollbarDragSkin = NULL;

    if (scrollbarbgtl) delete scrollbarbgtl; scrollbarbgtl = NULL;
    if (scrollbarbgt)  delete scrollbarbgt;  scrollbarbgt  = NULL;
    if (scrollbarbgtr) delete scrollbarbgtr; scrollbarbgtr = NULL;
    if (scrollbarbgl)  delete scrollbarbgl;  scrollbarbgl  = NULL;
    if (scrollbarbgc)  delete scrollbarbgc;  scrollbarbgc  = NULL;
    if (scrollbarbgr)  delete scrollbarbgr;  scrollbarbgr  = NULL;
    if (scrollbarbgbl) delete scrollbarbgbl; scrollbarbgbl = NULL;
    if (scrollbarbgb)  delete scrollbarbgb;  scrollbarbgb  = NULL;
    if (scrollbarbgbr) delete scrollbarbgbr; scrollbarbgbr = NULL;

    scrollbarbg.SetTL(NULL);
    scrollbarbg.SetTop(NULL);
    scrollbarbg.SetTR(NULL);
    scrollbarbg.SetLeft(NULL);
    scrollbarbg.SetCenter(NULL);
    scrollbarbg.SetRight(NULL);
    scrollbarbg.SetBL(NULL);
    scrollbarbg.SetBottom(NULL);
    scrollbarbg.SetBR(NULL);

    glictGlobals.scrollbarPanelSkin = NULL;
#endif



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




    if (consoletabbgtl) delete consoletabbgtl; consoletabbgtl = NULL;
    if (consoletabbgt)  delete consoletabbgt;  consoletabbgt  = NULL;
    if (consoletabbgtr) delete consoletabbgtr; consoletabbgtr = NULL;
    if (consoletabbgl)  delete consoletabbgl;  consoletabbgl  = NULL;
    if (consoletabbgc)  delete consoletabbgc;  consoletabbgc  = NULL;
    if (consoletabbgr)  delete consoletabbgr;  consoletabbgr  = NULL;
    if (consoletabbgbl) delete consoletabbgbl; consoletabbgbl = NULL;
    if (consoletabbgb)  delete consoletabbgb;  consoletabbgb  = NULL;
    if (consoletabbgbr) delete consoletabbgbr; consoletabbgbr = NULL;

    consoletabbg.SetTL(NULL);
    consoletabbg.SetTop(NULL);
    consoletabbg.SetTR(NULL);
    consoletabbg.SetLeft(NULL);
    consoletabbg.SetCenter(NULL);
    consoletabbg.SetRight(NULL);
    consoletabbg.SetBL(NULL);
    consoletabbg.SetBottom(NULL);
    consoletabbg.SetBR(NULL);

    if (consoletabactivetl) delete consoletabactivetl; consoletabactivetl = NULL;
    if (consoletabactivet)  delete consoletabactivet;  consoletabactivet  = NULL;
    if (consoletabactivetr) delete consoletabactivetr; consoletabactivetr = NULL;
    if (consoletabactivel)  delete consoletabactivel;  consoletabactivel  = NULL;
    if (consoletabactivec)  delete consoletabactivec;  consoletabactivec  = NULL;
    if (consoletabactiver)  delete consoletabactiver;  consoletabactiver  = NULL;
    if (consoletabactivebl) delete consoletabactivebl; consoletabactivebl = NULL;
    if (consoletabactiveb)  delete consoletabactiveb;  consoletabactiveb  = NULL;
    if (consoletabactivebr) delete consoletabactivebr; consoletabactivebr = NULL;

    consoletabactive.SetTL(NULL);
    consoletabactive.SetTop(NULL);
    consoletabactive.SetTR(NULL);
    consoletabactive.SetLeft(NULL);
    consoletabactive.SetCenter(NULL);
    consoletabactive.SetRight(NULL);
    consoletabactive.SetBL(NULL);
    consoletabactive.SetBottom(NULL);
    consoletabactive.SetBR(NULL);

    if (consoletabpassivetl) delete consoletabpassivetl; consoletabpassivetl = NULL;
    if (consoletabpassivet)  delete consoletabpassivet;  consoletabpassivet  = NULL;
    if (consoletabpassivetr) delete consoletabpassivetr; consoletabpassivetr = NULL;
    if (consoletabpassivel)  delete consoletabpassivel;  consoletabpassivel  = NULL;
    if (consoletabpassivec)  delete consoletabpassivec;  consoletabpassivec  = NULL;
    if (consoletabpassiver)  delete consoletabpassiver;  consoletabpassiver  = NULL;
    if (consoletabpassivebl) delete consoletabpassivebl; consoletabpassivebl = NULL;
    if (consoletabpassiveb)  delete consoletabpassiveb;  consoletabpassiveb  = NULL;
    if (consoletabpassivebr) delete consoletabpassivebr; consoletabpassivebr = NULL;

    consoletabpassive.SetTL(NULL);
    consoletabpassive.SetTop(NULL);
    consoletabpassive.SetTR(NULL);
    consoletabpassive.SetLeft(NULL);
    consoletabpassive.SetCenter(NULL);
    consoletabpassive.SetRight(NULL);
    consoletabpassive.SetBL(NULL);
    consoletabpassive.SetBottom(NULL);
    consoletabpassive.SetBR(NULL);

    if (compasstl) delete compasstl; compasstl = NULL;
    if (compasst)  delete compasst;  compasst  = NULL;
    if (compasstr) delete compasstr; compasstr = NULL;
    if (compassl)  delete compassl;  compassl  = NULL;
    if (compassc)  delete compassc;  compassc  = NULL;
    if (compassr)  delete compassr;  compassr  = NULL;
    if (compassbl) delete compassbl; compassbl = NULL;
    if (compassb)  delete compassb;  compassb  = NULL;
    if (compassbr) delete compassbr; compassbr = NULL;

    compass.SetTL(NULL);
    compass.SetTop(NULL);
    compass.SetTR(NULL);
    compass.SetLeft(NULL);
    compass.SetCenter(NULL);
    compass.SetRight(NULL);
    compass.SetBL(NULL);
    compass.SetBottom(NULL);
    compass.SetBR(NULL);

    for (int i = 0; i < BUTTON_LASTID; i++)
    {
        if (graphicbtntl[i] )delete graphicbtntl[i]; graphicbtntl[i] = NULL;
        if (graphicbtnt[i] ) delete graphicbtnt[i];  graphicbtnt [i] = NULL;
        if (graphicbtntr[i] )delete graphicbtntr[i]; graphicbtntr[i] = NULL;
        if (graphicbtnl[i] ) delete graphicbtnl[i];  graphicbtnl [i] = NULL;
        if (graphicbtnc[i] ) delete graphicbtnc[i];  graphicbtnc [i] = NULL;
        if (graphicbtnr[i] ) delete graphicbtnr[i];  graphicbtnr [i] = NULL;
        if (graphicbtnbl[i] )delete graphicbtnbl[i]; graphicbtnbl[i] = NULL;
        if (graphicbtnb[i] ) delete graphicbtnb[i];  graphicbtnb [i] = NULL;
        if (graphicbtnbr[i] )delete graphicbtnbr[i]; graphicbtnbr[i] = NULL;

        graphicbtn[i].SetTL(NULL);
        graphicbtn[i].SetTop(NULL);
        graphicbtn[i].SetTR(NULL);
        graphicbtn[i].SetLeft(NULL);
        graphicbtn[i].SetCenter(NULL);
        graphicbtn[i].SetRight(NULL);
        graphicbtn[i].SetBL(NULL);
        graphicbtn[i].SetBottom(NULL);
        graphicbtn[i].SetBR(NULL);

        if (graphicbthtl[i] )delete graphicbthtl[i]; graphicbthtl[i] = NULL;
        if (graphicbtht[i] ) delete graphicbtht[i];  graphicbtht [i] = NULL;
        if (graphicbthtr[i] )delete graphicbthtr[i]; graphicbthtr[i] = NULL;
        if (graphicbthl[i] ) delete graphicbthl[i];  graphicbthl [i] = NULL;
        if (graphicbthc[i] ) delete graphicbthc[i];  graphicbthc [i] = NULL;
        if (graphicbthr[i] ) delete graphicbthr[i];  graphicbthr [i] = NULL;
        if (graphicbthbl[i] )delete graphicbthbl[i]; graphicbthbl[i] = NULL;
        if (graphicbthb[i] ) delete graphicbthb[i];  graphicbthb [i] = NULL;
        if (graphicbthbr[i] )delete graphicbthbr[i]; graphicbthbr[i] = NULL;

        graphicbth[i].SetTL(NULL);
        graphicbth[i].SetTop(NULL);
        graphicbth[i].SetTR(NULL);
        graphicbth[i].SetLeft(NULL);
        graphicbth[i].SetCenter(NULL);
        graphicbth[i].SetRight(NULL);
        graphicbth[i].SetBL(NULL);
        graphicbth[i].SetBottom(NULL);
        graphicbth[i].SetBR(NULL);



    }


    if (backgroundtl) delete backgroundtl; backgroundtl = NULL;
    if (backgroundt)  delete backgroundt;  backgroundt  = NULL;
    if (backgroundtr) delete backgroundtr; backgroundtr = NULL;
    if (backgroundl)  delete backgroundl;  backgroundl  = NULL;
    if (backgroundc)  delete backgroundc;  backgroundc  = NULL;
    if (backgroundr)  delete backgroundr;  backgroundr  = NULL;
    if (backgroundbl) delete backgroundbl; backgroundbl = NULL;
    if (backgroundb)  delete backgroundb;  backgroundb  = NULL;
    if (backgroundbr) delete backgroundbr; backgroundbr = NULL;

    background.SetTL(NULL);
    background.SetTop(NULL);
    background.SetTR(NULL);
    background.SetLeft(NULL);
    background.SetCenter(NULL);
    background.SetRight(NULL);
    background.SetBL(NULL);
    background.SetBottom(NULL);
    background.SetBR(NULL);

}

