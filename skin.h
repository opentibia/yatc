#ifndef __SKIN_H
#define __SKIN_H

#include <GLICT/globals.h>

#if (GLICT_APIREV < 49)
#error You must update GLICT to at least revision 49 for skins to compile correctly.
#endif

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

	skinImage *wintl, *wint, *wintr, *winl, *winc, *winr, *winbl, *winb, *winbr; // window
	glictSkinner win;
	skinImage *btntl, *btnt, *btntr, *btnl, *btnc, *btnr, *btnbl, *btnb, *btnbr; // button
	glictSkinner btn;
	skinImage *bthtl, *btht, *bthtr, *bthl, *bthc, *bthr, *bthbl, *bthb, *bthbr; // button highlighted
	glictSkinner bth;
	skinImage *txttl, *txtt, *txttr, *txtl, *txtc, *txtr, *txtbl, *txtb, *txtbr; // textbox
	skinImage *chktl, *chkt, *chktr, *chkl, *chkc, *chkr, *chkbl, *chkb, *chkbr; // checkbox
	skinImage *tmmtl, *tmmt, *tmmtr, *tmml, *tmmc, *tmmr, *tmmbl, *tmmb, *tmmbr; // tibia main menu panel
	skinImage *invtl, *invt, *invtr, *invl, *invc, *invr, *invbl, *invb, *invbr; // inventory item panel bg
	skinImage *scutl, *scut, *scutr, *scul, *scuc, *scur, *scubl, *scub, *scubr; // scrollbar up button
	glictSkinner scu;
	skinImage *shutl, *shut, *shutr, *shul, *shuc, *shur, *shubl, *shub, *shubr; // scrollbar up button, highlighted
	glictSkinner shu;
	skinImage *scdtl, *scdt, *scdtr, *scdl, *scdc, *scdr, *scdbl, *scdb, *scdbr; // scrollbar dn button
	glictSkinner scd;
	skinImage *shdtl, *shdt, *shdtr, *shdl, *shdc, *shdr, *shdbl, *shdb, *shdbr; // scrollbar dn button, highlighted
	glictSkinner shd;
	skinImage *scbtl, *scbt, *scbtr, *scbl, *scbc, *scbr, *scbbl, *scbb, *scbbr; // scrollbar drag button
	glictSkinner scb;
	skinImage *scptl, *scpt, *scptr, *scpl, *scpc, *scpr, *scpbl, *scpb, *scpbr; // scrollbar panel bg
	glictSkinner scp;
public:
	glictSkinner chk, tmm, inv;
	glictSkinner txt;

	friend class skinImage;
};

extern Skin g_skin;

#endif

