#ifndef __UI_MINIMAP_H
#define __UI_MINIMAP_H

#include <GLICT/container.h>
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include <GLICT/window.h>
#include <math.h>
#include "engine.h"
#include "skin.h"
#include "../automap.h"
#include "../gamecontent/globalvars.h"

class pnlMap_t{
public:
#if (GLICT_APIREV >= 98)
    glictPanel panel; // 170x34
#elif _MSC_VER
	 __declspec(deprecated) glictWindow panel;
#else
    glictWindow panel __attribute__ ((deprecated));
#endif

	glictPanel pnlMap;

	pnlMap_t(Automap* map) {
		panel.SetWidth(150);
		panel.SetHeight(150);

		#if (GLICT_APIREV >= 98)
		panel.SetDraggable(true);
		panel.SetSkin(&g_skin.background);
		panel.SetFocusable(true);
		#else
			#ifdef _MSC_VER
				#pragma warning(Minimap is not properly drawn until you upgrade to GLICT 98+)
			#else
				#warning Minimap is not properly drawn until you upgrade to GLICT 98+
			#endif
		//panel.SetCaption(gettext("Map"));
		panel.SetCaption("Map");
		#endif

		panel.AddObject(&pnlMap);

		pnlMap.SetPos(2,2);
		pnlMap.SetWidth(130);
		pnlMap.SetHeight(130);
		pnlMap.SetCaption("");
		pnlMap.SetBGColor(.1,.1,.1,1);
		pnlMap.SetSkin(&g_skin.inv);
		pnlMap.SetCustomData(map);
		pnlMap.SetOnPaint(&pnlMap_t::paintMap);
	}

	~pnlMap_t() {}

	static void paintMap(glictRect *real, glictRect *clipped, glictContainer *caller){
		Automap* map = (Automap*)caller->GetCustomData();
		//g_engine->drawRectangleLines(clipped->left, clipped->top, clipped->right-clipped->left, clipped->bottom-clipped->top, oRGBA(255,255,255,255));
		map->renderSelf((int)real->left+1, (int)real->top+1, 128, 128, GlobalVariables::getPlayerPosition());
	}
};

#endif //__UI_MINIMAP_H
