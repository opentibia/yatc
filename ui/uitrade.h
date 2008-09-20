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

class winTrade_t {
public:
	glictWindow window; // Width: 160 Height: 115 Caption: "Trade"
	#if (GLICT_APIREV >= 76)
	glictPanel pnlIcon; // Width: 12 Height: 12 SpriteID: ? (Bag)
	glictButton btnClose; // Width: 12 Height: 12 Caption: "x"
	glictButton btnCollapse; // Width: 12 Height: 12 Caption: "-"
	#endif

	glictPanel pnlContainer; // Pos: 0, 12 Width: 168 Height: 75 Virtual Height: as necessary

	glictPanel lblNameLeft; // Pos: 3, 2 Width: 70 Height: 8 Caption: Name of initator
	glictPanel lblNameRight; // Pos: 82, 2 Width: 70 Height: 8 Caption: Name of other trader

	glictPanel pnlSepTop; // Pos: 0, 10 Width: 168 Height: 2
	glictPanel pnlSepMiddle; // Pos: 78, 13 Width: 2 Height: Virtual Height of pnlContainer
	glictPanel pnlSepBottom; // Pos: 0, 88 Width: 168 Height: 2

	glictPanel lblWait; // Pos: 12, 92 Width: 60 Height: 20 Caption: "Wait for a counter offer."
	glictButton btnAccept; // Pos: 75, 92 Width: 43 Height: 20 Caption: "Accept"
	glictButton btnReject; // Pos: 122, 92 Width: 43 Height: 20 Caption: "Reject"

	typedef std::list<glictPanel*> PanelList;
	PanelList pnlItemsLeft;
	PanelList pnlItemsRight;

	Container* initator;
	Container* acceptor;

	winTrade_t(Container* _initatior);

	~winTrade_t();

	void onTradeAccepted(Container* _acceptor);

	static void tradeItemOnPaint(glictRect* real, glictRect* clipped, glictContainer* caller);
	static void tradeItemOnClick(glictPos* relmousepos, glictContainer* caller);

	static void tradeOnAccept(glictPos* relmousepos, glictContainer* caller);
	static void tradeOnReject(glictPos* relmousepos, glictContainer* caller);

	static void onClose(glictPos* relmousepos, glictContainer* caller);
	static void onCollapse(glictPos* relmousepos, glictContainer* caller);
	static void iconOnPaint(glictRect* real, glictRect* clipped, glictContainer* caller);
};

#endif
