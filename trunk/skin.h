#ifndef __SKIN_H
#define __SKIN_H

#include <GLICT/globals.h>
#if (GLICT_APIREV >= 49)

#include <GLICT/image.h>
#include "sprite.h"
#include "engine.h"

class skinImage : public glictImage {

public:
	skinImage(const std::string& filename, int index = 0, float x=0, float y=0, float w=-1, float h=-1) {
		m_spr = g_engine->createSprite(filename, index);
		m_x = x;
		m_y = y;
		if (w == -1)
			if (m_spr)
				m_width = m_spr->getWidth();
			else
				m_width = 0;
		else
			m_width = w;

		if (h == -1)
			if (m_spr)
				m_height = m_spr->getHeight();
			else
				m_height = 0;
		else
			m_height = h;

		m_filename = filename;

	}
	~skinImage() {
		delete m_spr;
	}
	void Paint(float destx, float desty, float w, float h) {
		m_spr->Blit(destx + glictGlobals.translation.x, desty + glictGlobals.translation.y, m_x, m_y, w, h);

	}
	float GetW() {
		return m_width;
	}
	float GetH() {
		return m_height;
	}

private:
	Sprite* m_spr;
	float m_x, m_y, m_width, m_height;
	std::string m_filename;
};

class Skin {
public:
	Skin();
	~Skin();

	void loadSkin();
	void unloadSkin();


private:
	skinImage *wintl, *wint, *wintr, *winl, *winc, *winr, *winbl, *winb, *winbr;
	glictSkinner win;
	skinImage *btntl, *btnt, *btntr, *btnl, *btnc, *btnr, *btnbl, *btnb, *btnbr;
	glictSkinner btn;
	skinImage *bthtl, *btht, *bthtr, *bthl, *bthc, *bthr, *bthbl, *bthb, *bthbr;
	glictSkinner bth;
	skinImage *txttl, *txtt, *txttr, *txtl, *txtc, *txtr, *txtbl, *txtb, *txtbr;
	glictSkinner txt;
	skinImage *chktl, *chkt, *chktr, *chkl, *chkc, *chkr, *chkbl, *chkb, *chkbr;
	skinImage *tmmtl, *tmmt, *tmmtr, *tmml, *tmmc, *tmmr, *tmmbl, *tmmb, *tmmbr;
public:
	glictSkinner chk, tmm;
};
extern Skin skin;

#else
#warning You need GLICT APIREV 49 or newer to compile YATC with skins. Update your YATC.
class Skin {
public:
	void loadSkin() {}
	void unloadSkin() {}
};
#endif



#endif

