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

#if defined(HAVE_CONFIG_H)
    #include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

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
	glictPanel pnlCompass;
	glictButton btnZoomOut, btnZoomIn;
	glictButton btnLevelUp, btnLevelDown;
    glictButton btnCenter;

	pnlMap_t(Automap* map) {
		panel.SetWidth(172);
		panel.SetHeight(115);

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

		#endif

		panel.AddObject(&pnlMap);
		pnlMap.SetPos(9, 5);
		pnlMap.SetWidth(108);
		pnlMap.SetHeight(108);
		pnlMap.SetCaption("");
		pnlMap.SetBGColor(.1,.1,.1,1);
		pnlMap.SetSkin(&g_skin.txt);
		pnlMap.SetCustomData(map);
		pnlMap.SetOnPaint(&pnlMap_t::paintMap);

		panel.AddObject(&pnlCompass);
		pnlCompass.SetPos(125,6);
		pnlCompass.SetWidth(43);
		pnlCompass.SetHeight(43);
		pnlCompass.SetCaption("");
		pnlCompass.SetBGColor(.1,.1,.1,1.);
		pnlCompass.SetSkin(&g_skin.compass);

		panel.AddObject(&btnZoomOut);
		btnZoomOut.SetPos(125,51);
		btnZoomOut.SetWidth(20);
		btnZoomOut.SetHeight(20);
		btnZoomOut.SetCaption("");
		btnZoomOut.SetBGColor(.1,.1,.1,1.);
		btnZoomOut.SetSkin(&g_skin.graphicbtn[SKINGB_ZOOMOUT]);
		btnZoomOut.SetHighlightSkin(&g_skin.graphicbth[SKINGB_ZOOMOUT]);
		btnZoomOut.SetCustomData(map);
        btnZoomOut.SetOnClick(zoomOut_OnClick);

		panel.AddObject(&btnZoomIn);
		btnZoomIn.SetPos(125,72);
		btnZoomIn.SetWidth(20);
		btnZoomIn.SetHeight(20);
		btnZoomIn.SetCaption("");
		btnZoomIn.SetBGColor(.1,.1,.1,1.);
		btnZoomIn.SetSkin(&g_skin.graphicbtn[SKINGB_ZOOMIN]);
		btnZoomIn.SetHighlightSkin(&g_skin.graphicbth[SKINGB_ZOOMIN]);
        btnZoomIn.SetCustomData(map);
        btnZoomIn.SetOnClick(zoomIn_OnClick);

		panel.AddObject(&btnLevelUp);
		btnLevelUp.SetPos(148,51);
		btnLevelUp.SetWidth(20);
		btnLevelUp.SetHeight(20);
		btnLevelUp.SetCaption("");
		btnLevelUp.SetBGColor(.1,.1,.1,1.);
		btnLevelUp.SetSkin(&g_skin.graphicbtn[SKINGB_LEVELUP]);
		btnLevelUp.SetHighlightSkin(&g_skin.graphicbth[SKINGB_LEVELUP]);
		btnLevelUp.SetCustomData(map);

		panel.AddObject(&btnLevelDown);
		btnLevelDown.SetPos(148,72);
		btnLevelDown.SetWidth(20);
		btnLevelDown.SetHeight(20);
		btnLevelDown.SetCaption("");
		btnLevelDown.SetBGColor(.1,.1,.1,1.);
		btnLevelDown.SetSkin(&g_skin.graphicbtn[SKINGB_LEVELDOWN]);
		btnLevelDown.SetHighlightSkin(&g_skin.graphicbth[SKINGB_LEVELDOWN]);
        btnLevelDown.SetCustomData(map);



		panel.AddObject(&btnCenter);
		btnCenter.SetPos(125,93);
		btnCenter.SetWidth(43);
		btnCenter.SetHeight(20);
		btnCenter.SetCaption(gettext("Centre")); // Centre [sic!]
		btnCenter.SetBGColor(.1,.1,.1,1.);
		btnCenter.SetFont("minifont");

	}

	~pnlMap_t() {}

	static void paintMap(glictRect *real, glictRect *clipped, glictContainer *caller)
	{
		Automap* map = (Automap*)caller->GetCustomData();
		//g_engine->drawRectangleLines(clipped->left, clipped->top, clipped->right-clipped->left, clipped->bottom-clipped->top, oRGBA(255,255,255,255));
		map->renderSelf((int)real->left+1, (int)real->top+1, 106, 106, GlobalVariables::getPlayerPosition());
	}

    static void zoomIn_OnClick(glictPos* relmousepos, glictContainer* caller)
    {
        Automap* map = (Automap*)caller->GetCustomData();
        map->zoomIn();
    }
    static void zoomOut_OnClick(glictPos* relmousepos, glictContainer* caller)
    {
        Automap* map = (Automap*)caller->GetCustomData();
        map->zoomOut();
    }

};

#endif //__UI_MINIMAP_H
