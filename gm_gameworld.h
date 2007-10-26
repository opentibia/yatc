#ifndef __GM_GAMEWORLD_H
#define __GM_GAMEWORLD_H

#include "gamemode.h"
#include "sprite.h"
class GM_Gameworld : public GameMode {
public:
	GM_Gameworld();
	~GM_Gameworld();

	void renderScene();
private:
	Sprite* ui;
};

#endif

