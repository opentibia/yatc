#ifndef __UI_CONTAINER_H
#define __UI_CONTAINER_H

#include <GLICT/container.h>
#include <GLICT/button.h>
#include <GLICT/window.h>
#include "../gamecontent/container.h"
#include <cmath>
#include "../defines.h"

class winContainer_t {
public:
	winContainer_t(Container* _container, uint32_t cid);

	~winContainer_t() {
		for(PanelList::iterator it = pnlItems.begin(); it != pnlItems.end(); ++it)
		{
			delete (*it);
		}

		pnlItems.clear();
	}

	inline uint32_t getSlotId(glictContainer* pnl)
	{
		uint32_t slot_id = 0;
		for(PanelList::iterator it = pnlItems.begin(); it != pnlItems.end(); ++it,
			++slot_id)
		{
			if((*it) == pnl) break;
		}
		return slot_id;
	}

    static void OnClose(glictPos* pos, glictContainer *caller);


	glictWindow window;
	glictPanel winpanel;
	// we use a list of items in the container class, so I'm doing it here too
	typedef std::list<glictPanel*> PanelList;
	PanelList pnlItems;

	// container information
	Container* container;
	uint32_t containerId;

	static void containerItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void containerItemOnClick(glictPos *relmousepos, glictContainer* callerclass);
    static void containerItemOnMouseDown(glictPos *relmousepos, glictContainer* callerclass);
	static void containerItemOnMouseUp(glictPos *relmousepos, glictContainer* callerclass);

#if (GLICT_APIREV >= 76)
    glictButton closebtn;
#endif

};

#endif
