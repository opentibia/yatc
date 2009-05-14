#ifndef __SKIN_H
#define __SKIN_H

#include <GLICT/globals.h>
#include <GLICT/image.h>
#include "sprite.h"
#include "engine.h"

// NOTE (nfries88): Since khaos is loading all buttons into an array, let's give those buttons an ID.
#define BUTTON_IND_INV 0

#define BUTTON_IND_CONSOLE BUTTON_OPTIONSMINI
enum button_id_t
{
// 20x20 buttons
	BUTTON_ZOOMIN = BUTTON_IND_INV+0,
	BUTTON_ZOOMOUT = BUTTON_IND_INV+1,
	BUTTON_ARROWUP = BUTTON_IND_INV+2,
	BUTTON_ARROWDOWN = BUTTON_IND_INV+3,
	BUTTON_FULLATK = BUTTON_IND_INV+4,
	BUTTON_BALANCED = BUTTON_IND_INV+5,
	BUTTON_FULLDEF = BUTTON_IND_INV+6,
	BUTTON_NOCHASE = BUTTON_IND_INV+7,
	BUTTON_CHASE = BUTTON_IND_INV+8,
	BUTTON_SAFEMODE = BUTTON_IND_INV+9,
	BUTTON_HELPMINI = BUTTON_IND_INV+10,
	BUTTON_OPTIONSMINI = BUTTON_IND_INV+11,
// 16x16 buttons
	BUTTON_CONSOLE_CLOSE = BUTTON_IND_CONSOLE+0,
	BUTTON_CONSOLE_CHANNELS = BUTTON_IND_CONSOLE+1,
	BUTTON_QUESTIONMARK = BUTTON_IND_CONSOLE+2,
	BUTTON_CONSOLE_M = BUTTON_IND_CONSOLE+3, // note(nfries88): no clue what this does. :|
	BUTTON_BLANK = BUTTON_IND_CONSOLE+4,
	BUTTON_CONSOLE_IGNORELIST = BUTTON_IND_CONSOLE+5,
	BUTTON_CONSOLE_YELL = BUTTON_IND_CONSOLE+6,
	BUTTON_CONSOLE_SPEAK = BUTTON_IND_CONSOLE+7,
	BUTTON_CONSOLE_WHISPER = BUTTON_IND_CONSOLE+8,


	// last button, must always be last
	BUTTON_LASTID
};

class skinImage : public glictImage {
public:
	skinImage(float x=0, float y=0, float w=-1, float h=-1);
	~skinImage() {}
	void Paint(float destx, float desty, float w, float h);
	float GetW() { return m_width; }
	float GetH() { return m_height; }
private:
	float m_x, m_y, m_width, m_height;
};


class Skin {
public:
	Skin();
	~Skin();

	void loadSkin();
	void unloadSkin();

// NOTE (nfries88): I don't see a point in making some private and some public, so let's just make them all public.
//private:

	skinImage *windowtl, *windowt, *windowtr, *windowl, *windowc, *windowr, *windowbl, *windowb, *windowbr; // window
	glictSkinner window;
	skinImage *btnntl, *btnnt, *btnntr, *btnnl, *btnnc, *btnnr, *btnnbl, *btnnb, *btnnbr; // button
	glictSkinner btnn;
	skinImage *btnhtl, *btnht, *btnhtr, *btnhl, *btnhc, *btnhr, *btnhbl, *btnhb, *btnhbr; // button highlighted
	glictSkinner btnh;
	skinImage *scrollbaruptl, *scrollbarupt, *scrollbaruptr, *scrollbarupl, *scrollbarupc, *scrollbarupr, *scrollbarupbl, *scrollbarupb, *scrollbarupbr; // scrollbar up button
	glictSkinner scrollbarup;
	skinImage *scrollbaruphtl, *scrollbarupht, *scrollbaruphtr, *scrollbaruphl, *scrollbaruphc, *scrollbaruphr, *scrollbaruphbl, *scrollbaruphb, *scrollbaruphbr; // scrollbar up button, highlighted
	glictSkinner scrollbaruph;
	skinImage *scrollbardntl, *scrollbardnt, *scrollbardntr, *scrollbardnl, *scrollbardnc, *scrollbardnr, *scrollbardnbl, *scrollbardnb, *scrollbardnbr; // scrollbar dn button
	glictSkinner scrollbardn;
	skinImage *scrollbardnhtl, *scrollbardnht, *scrollbardnhtr, *scrollbardnhl, *scrollbardnhc, *scrollbardnhr, *scrollbardnhbl, *scrollbardnhb, *scrollbardnhbr; // scrollbar dn button, highlighted
	glictSkinner scrollbardnh;
	skinImage *scrollbarlttl, *scrollbarltt, *scrollbarlttr, *scrollbarltl, *scrollbarltc, *scrollbarltr, *scrollbarltbl, *scrollbarltb, *scrollbarltbr; // scrollbar lt button
	glictSkinner scrollbarlt;
	skinImage *scrollbarlthtl, *scrollbarltht, *scrollbarlthtr, *scrollbarlthl, *scrollbarlthc, *scrollbarlthr, *scrollbarlthbl, *scrollbarlthb, *scrollbarlthbr; // scrollbar lt button, highlighted
	glictSkinner scrollbarlth;
	skinImage *scrollbarrttl, *scrollbarrtt, *scrollbarrttr, *scrollbarrtl, *scrollbarrtc, *scrollbarrtr, *scrollbarrtbl, *scrollbarrtb, *scrollbarrtbr; // scrollbarrtollbar rt button
	glictSkinner scrollbarrt;
	skinImage *scrollbarrthtl, *scrollbarrtht, *scrollbarrthtr, *scrollbarrthl, *scrollbarrthc, *scrollbarrthr, *scrollbarrthbl, *scrollbarrthb, *scrollbarrthbr; // scrollbar rt button, highlighted
	glictSkinner scrollbarrth;
	skinImage *scrollbarbtntl, *scrollbarbtnt, *scrollbarbtntr, *scrollbarbtnl, *scrollbarbtnc, *scrollbarbtnr, *scrollbarbtnbl, *scrollbarbtnb, *scrollbarbtnbr; // scrollbar drag button
	glictSkinner scrollbarbtn;
	skinImage *scrollbarbgtl, *scrollbarbgt, *scrollbarbgtr, *scrollbarbgl, *scrollbarbgc, *scrollbarbgr, *scrollbarbgbl, *scrollbarbgb, *scrollbarbgbr; // scrollbar panel bg
	glictSkinner scrollbarbg;


	skinImage *txttl, *txtt, *txttr, *txtl, *txtc, *txtr, *txtbl, *txtb, *txtbr; // textbox
	skinImage *chktl, *chkt, *chktr, *chkl, *chkc, *chkr, *chkbl, *chkb, *chkbr; // checkbox
	skinImage *tmmtl, *tmmt, *tmmtr, *tmml, *tmmc, *tmmr, *tmmbl, *tmmb, *tmmbr; // tibia main menu panel
	skinImage *invtl, *invt, *invtr, *invl, *invc, *invr, *invbl, *invb, *invbr; // inventory item panel bg
	skinImage *consoletabbgtl, *consoletabbgt, *consoletabbgtr, *consoletabbgl, *consoletabbgc, *consoletabbgr, *consoletabbgbl, *consoletabbgb, *consoletabbgbr; // console button container bg
	skinImage *consoletabactivetl, *consoletabactivet, *consoletabactivetr, *consoletabactivel, *consoletabactivec, *consoletabactiver, *consoletabactivebl, *consoletabactiveb, *consoletabactivebr; // console button active bg
	skinImage *consoletabpassivetl, *consoletabpassivet, *consoletabpassivetr, *consoletabpassivel, *consoletabpassivec, *consoletabpassiver, *consoletabpassivebl, *consoletabpassiveb, *consoletabpassivebr; // console button passive bg
	skinImage *compasstl, *compasst, *compasstr, *compassl, *compassc, *compassr, *compassbl, *compassb, *compassbr; // minimap compass
	skinImage *graphicbtntl[BUTTON_LASTID], *graphicbtnt[BUTTON_LASTID], *graphicbtntr[BUTTON_LASTID], *graphicbtnl[BUTTON_LASTID], *graphicbtnc[BUTTON_LASTID], *graphicbtnr[BUTTON_LASTID], *graphicbtnbl[BUTTON_LASTID], *graphicbtnb[BUTTON_LASTID], *graphicbtnbr[BUTTON_LASTID]; // various graphic buttons
	skinImage *graphicbthtl[BUTTON_LASTID], *graphicbtht[BUTTON_LASTID], *graphicbthtr[BUTTON_LASTID], *graphicbthl[BUTTON_LASTID], *graphicbthc[BUTTON_LASTID], *graphicbthr[BUTTON_LASTID], *graphicbthbl[BUTTON_LASTID], *graphicbthb[BUTTON_LASTID], *graphicbthbr[BUTTON_LASTID]; // various graphic buttons, highlighted

	skinImage *backgroundtl, *backgroundt, *backgroundtr, *backgroundl, *backgroundc, *backgroundr, *backgroundbl, *backgroundb, *backgroundbr; // general background

public:
	glictSkinner chk, tmm, inv, consoletabbg, consoletabactive, consoletabpassive, compass, background, graphicbtn[BUTTON_LASTID], graphicbth[BUTTON_LASTID];
	glictSkinner txt;

	friend class skinImage;
};


enum skinGraphicButtons_t
{
    SKINGB_ZOOMIN,
    SKINGB_ZOOMOUT,
    SKINGB_LEVELUP,
    SKINGB_LEVELDOWN,
    SKINGB_AGRESSIVE,
    SKINGB_NEUTRAL,
    SKINGB_DEFENSIVE,
    SKINGB_NOFOLLOWING,
    SKINGB_FOLLOWING,
    SKINGB_ATTACKUNMARKED,
    SKINGB_QUESTIONMARK
};

extern Skin g_skin;

#endif

