#ifndef __UI_CONTAINER_H
#define __UI_CONTAINER_H

#include "../gamecontent/container.h" // @Nate: please don't use backslashes; windows does tolerate slash, but unix does not tolerate backslash!
#include <cmath>

class winContainer_t {
public:
	winContainer_t(Container* _container, uint32_t cid) {
		container = _container;
		containerId = cid;

		window.SetCaption(container->getName());
		// wide enough to display 4 items per row, tall enough to display all rows or three rows, whichever is smalle
		window.SetWidth(150 + 10); // 10 == for scrollbar
		window.SetHeight(MIN(4+36*3, 4 + (36*ceil(container->getCapacity()/4.))));

        // we're adding a winpanel object.
        // sadly, glictWindow does not have virtualsize handling implemented.
        // glictPanel does have virtual size; we'll add subpanels into glictPanel
        // which'll be of the same size as the window itself, and the only object
        // on the window.
		window.AddObject(&winpanel);
		winpanel.SetBGActiveness(false);
		winpanel.SetPos(0,0);
		winpanel.SetWidth(150 + 10); // same size as for window
		winpanel.SetHeight(MIN(4+36*3, 4 + (36*ceil(container->getCapacity()/4.))));
		// virtual size is always to display all rows
		winpanel.SetVirtualSize(160, 4 + (36*ceil(container->getCapacity()/4.)));

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
			panel->SetOnClick(winContainer_t::containerItemOnClick);
			panel->SetOnMouseDown(winContainer_t::containerItemOnMouseDown);
			panel->SetOnMouseUp(winContainer_t::containerItemOnMouseUp);

			panel->SetSkin(&g_skin.inv);

			winpanel.AddObject(panel);
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
		uint32_t slot_id = 0;
		for(PanelList::iterator it = pnlItems.begin(); it != pnlItems.end(); ++it,
			++slot_id)
		{
			if((*it) == pnl) break;
		}
		return slot_id;
	}

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

};

#endif
