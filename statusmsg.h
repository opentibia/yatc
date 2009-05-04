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


// This file implements status message displayed temporarily on bottom
// of the screen.

#ifndef __STATUSMSG_H
#define __STATUSMSG_H

#include <string>
#include "gamecontent/enums.h"

enum VAlignment
{
    TOP, MIDDLE, BOTTOM
};
enum HAlignment
{
    LEFT, CENTER, RIGHT
};

class StatusMsg
{
    public:
        StatusMsg(TextColor_t color, const std::string& message, double timeout, double xoffset=0, double yoffset=0, HAlignment halign=CENTER, VAlignment valign=BOTTOM);

    private:
        double m_timeRemaining;
};

#endif
