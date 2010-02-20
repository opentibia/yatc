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

#include <sstream>
#include "gamecontent/enums.h"

enum MSG_POS
{
    CENTER, BOTTOM
};

class StatusMsg
{
    public:
        StatusMsg();
        StatusMsg(TextColor_t color, const std::string& message, double timeout, MSG_POS msgPos);

        void paintSelf();
        void updateSelf(double k);
    private:
        void linebreaker();

        double m_timeRemaining;
        TextColor_t m_textColor;
        std::string m_messageText;

        MSG_POS m_msgPos;
};

#endif
