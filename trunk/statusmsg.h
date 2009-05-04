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
    ALIGN_TOP, ALIGN_MIDDLE, ALIGN_BOTTOM
};
enum HAlignment
{
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
};

class StatusMsg
{
    public:
        StatusMsg();
        StatusMsg(TextColor_t color, const std::string& message, double timeout, double xoffset=0, double yoffset=0, HAlignment halign=ALIGN_CENTER, VAlignment valign=ALIGN_BOTTOM);

        void paintSelf(int windowX, int windowY, int windowW, int windowH);
        void updateSelf(double k);
    private:
        double m_timeRemaining;
        TextColor_t m_textColor;
        std::string m_messageText;

        double m_xOffset, m_yOffset;
        HAlignment m_hAlign; VAlignment m_vAlign;
};

#endif
