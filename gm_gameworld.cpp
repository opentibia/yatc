
#include "gm_gameworld.h"
#include "engine.h"
#include "options.h"
GM_Gameworld::GM_Gameworld () {
	ui = g_engine->createSprite("Tibia.pic", 3);
}

GM_Gameworld::~GM_Gameworld () {
}

void GM_Gameworld::renderScene() {
	for (int i=0;i<options.w;i+=96)
		for (int j=0;j<options.h;j+=96)
			ui->Blit(i,j,0,0,96,96);
}
