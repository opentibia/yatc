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


#ifndef __UI_INVENTORY_H
#define __UI_INVENTORY_H

#include <GLICT/container.h>
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include <GLICT/window.h>
#include <GLICT/globals.h>

#include "../skin.h"
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include "../popup.h"
#include "itempanel.h"

class pnlInventory_t {
public:
	pnlInventory_t();
	~pnlInventory_t();

#if (GLICT_APIREV >= 98)
    glictPanel panel; // 170x34
#elif _MSC_VER
	__declspec(deprecated) glictWindow panel;
#else
	glictWindow panel __attribute__((deprecated));
#endif
	glictButton btnLogout;
	glictButton btnOptions;

	ItemPanel* pnlItem[10];

	static void onClick_Logout(glictPos* relmousepos, glictContainer* callerclass);
	static void onClick_Options(glictPos* relmousepos, glictContainer* callerclass);
};


#endif
