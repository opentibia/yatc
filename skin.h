#ifndef __SKIN_H
#define __SKIN_H

#include <GLICT/globals.h>
#if (GLICT_APIREV >= 49)

#include <GLICT/image.h>
#include "sprite.h"
#include "engine.h"

class skinImage : public glictImage {
public:
	skinImage(const std::string& filename, int index = 0) {
		spr = g_engine->createSprite(filename, index);
	}
	void Paint(float destx, float desty, float w, float h) {
		spr->Blit(destx + glictGlobals.translation.x, desty + glictGlobals.translation.y, 0, 0, w, h);

	}
	float GetW() {
		if (spr)
			return spr->getWidth();
		else
			return 0;
	}
	float GetH() {
		if (spr)
			return spr->getHeight();
		else
			return 0;
	}
private:
	Sprite* spr;
};

class Skin {
public:
	Skin();
	~Skin();

	void loadSkin(const char *what);
	void unloadSkin();


	glictSkinner tmm;
private:
	skinImage *wintl, *wint, *wintr, *winl, *winc, *winr, *winbl, *winb, *winbr;
	glictSkinner win;
	skinImage *btntl, *btnt, *btntr, *btnl, *btnc, *btnr, *btnbl, *btnb, *btnbr;
	glictSkinner btn;
	skinImage *bthtl, *btht, *bthtr, *bthl, *bthc, *bthr, *bthbl, *bthb, *bthbr;
	glictSkinner bth;
	skinImage *txttl, *txtt, *txttr, *txtl, *txtc, *txtr, *txtbl, *txtb, *txtbr;
	glictSkinner txt;
	skinImage *tmmtl, *tmmt, *tmmtr, *tmml, *tmmc, *tmmr, *tmmbl, *tmmb, *tmmbr;
};
extern Skin skin;

#else
#warning You need GLICT APIREV 49 or newer to compile YATC with skins. Update your YATC.
class Skin {
public:
	void loadSkin(const char *what) {}
	void unloadSkin() {}
};
#endif



#endif

