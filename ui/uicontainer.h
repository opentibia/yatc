//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __UI_CONTAINER_H
#define __UI_CONTAINER_H

#include <GLICT/container.h>
#include <GLICT/button.h>
#include <GLICT/window.h>
#include "../gamecontent/container.h"
#include <cmath>
#include "../defines.h"
#include "../popup.h"
#include "stackpanel.h"

class winContainer_t : public yatcStackPanelWindow {
public:
	winContainer_t(Container* _container, uint32_t cid);

	virtual ~winContainer_t();

	float GetDefaultHeight();
	virtual void OnClose();

	// we use a list of items in the container class, so I'm doing it here too
	typedef std::list<glictPanel*> PanelList;
	PanelList pnlItems;

	// container information
	Container* container;
	uint32_t containerId;

    glictPanel pnlIcon;

    Item* itemIcon;

    static void containerIconOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller);

};

#endif
