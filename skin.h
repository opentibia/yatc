#ifndef __SKIN_H
#define __SKIN_H

#include <GLICT/globals.h>
#include <GLICT/image.h>
#include "sprite.h"
#include "engine.h"



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


private:

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

	skinImage *backgroundtl, *backgroundt, *backgroundtr, *backgroundl, *backgroundc, *backgroundr, *backgroundbl, *backgroundb, *backgroundbr; // general background

public:
	glictSkinner chk, tmm, inv, consoletabbg, consoletabactive, consoletabpassive, compass, background;
	glictSkinner txt;

	friend class skinImage;
};

extern Skin g_skin;

#endif

