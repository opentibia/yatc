#ifndef __UI_CONTAINER_H
#define __UI_CONTAINER_H

#include "..\gamecontent\container.h"
#include <cmath>

class winContainer_t {
public:
	winContainer_t(Container* _container, uint32_t cid) {
		container = _container;
		containerId = cid;

		window.SetCaption(container->getName());
		// wide enough to display 4 items per row, tall enough to display all rows
		window.SetWidth(150);
		window.SetHeight(4 + (36*(container->getCapacity()/4)));

		for(uint32_t i = 0; i != container->getCapacity(); ++i)
		{
			glictPanel* panel = new glictPanel;
			panel->SetPos(5 + ((i % 4) * 36), 4 + (std::floor(i / 4) * 36));
			panel->SetWidth(32);
			panel->SetHeight(32);
			panel->SetBGColor(.1,.1,.1,1);
			panel->SetCaption("");
			panel->SetCustomData(this);
			panel->SetOnPaint(winContainer_t::containerItemOnPaint);
			panel->SetOnClick(winContainer_t::containersItemOnClick);

			window.AddObject(panel);
			pnlItems.push_back(panel);
		}
	}

	~winContainer_t() {
		for(PanelList::iterator it = pnlItems.begin(); it != pnlItems.end(); ++it)
		{
			delete (*it);
		}

		pnlItems.clear();
	}

	inline uint32_t getSlotId(glictContainer* pnl)
	{
		uint32_t slot_id = 1;
		for(PanelList::iterator it = pnlItems.begin(); it != pnlItems.end(); ++it,
			++slot_id)
		{
			if((*it) == pnl) break;
		}
		return slot_id;
	}

	glictWindow window;
	// we use a list of items in the container class, so I'm doing it here too
	typedef std::list<glictPanel*> PanelList;
	PanelList pnlItems;

	// container information
	Container* container;
	uint32_t containerId;

	static void containerItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void containersItemOnClick(glictPos *relmousepos, glictContainer* callerclass);
};

#endif
