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

#include "statusmsg.h"
#include "engine.h"
#include "gamecontent/globalvars.h"

StatusMsg::StatusMsg()
{
    m_timeRemaining = 0;
    m_textColor = TEXTCOLOR_WHITE;
    m_messageText = "";

    m_msgPos = BOTTOM;
    m_linecount = 1;
    linebreaker();
}
StatusMsg::StatusMsg(TextColor_t color, const std::string& message, double timeout, MSG_POS msgPos)
{
    m_timeRemaining = timeout;
    m_textColor = color;
    m_messageText = message;

    m_msgPos = msgPos;
    m_linecount = 1;
    linebreaker();

    m_timeRemaining += (m_linecount - 1) * 0.5;
}

void StatusMsg::paintSelf()
{
    double x, y;
    float scale = GlobalVariables::getScale();

    x = ((scale * 32) * 15) / 2;
    y = (scale * 32) * 11;

    if (m_timeRemaining <= 0)
        return;

    switch (m_msgPos)
    {
        case CENTER_INFO:
            y = (y / 2) - 12;
            y -= (12 * (m_linecount - 1));
            break;
        case CENTER_EVENT:
            y = (y / 2) + 12;
            break;
        case BOTTOM:
            y -= 12;
            break;
    }

    g_engine->drawTextGW(m_messageText.c_str(),"gamefont",x,y,scale,m_textColor);
}

void StatusMsg::linebreaker()
{
    if (BOTTOM)
        return;

    std::string new_line_text, old_line_text;
    std::stringstream final_text;

    new_line_text = old_line_text = m_messageText;
    m_linecount = 1;
    int line_size = m_messageText.length();
    size_t iter_pos;

    // NOTE (Kilouco): Here we make linebreaks for public messages.
    // TODO (Kilouco): Generic Linebreaker.
    //39 characters per line, despite their size.
    if (line_size > 39) {
        while (line_size > 39) {
            old_line_text.erase(old_line_text.end()-1);
            line_size = old_line_text.length();

            if (line_size <= 39) {
                iter_pos = old_line_text.find_last_of(" ");

                if(iter_pos == std::string::npos || !iter_pos) {
                    iter_pos = old_line_text.length();
                    new_line_text = new_line_text.substr(iter_pos-1);
                    old_line_text.resize(iter_pos);

                    old_line_text.erase(old_line_text.end()-1);
                    old_line_text = old_line_text + "-";
                    iter_pos = old_line_text.length();
                }
                else {
                    new_line_text = new_line_text.substr(iter_pos);
                    old_line_text.resize(iter_pos);
                }

                final_text << old_line_text << "\n";

                m_linecount++;

                old_line_text = new_line_text;
                line_size = old_line_text.length();

                if (line_size <= 39)
                    final_text << new_line_text;
            }
        }
        m_messageText = final_text.str();
        return;
    }
}

void StatusMsg::updateSelf(double k)
{
    m_timeRemaining -= k;
}
