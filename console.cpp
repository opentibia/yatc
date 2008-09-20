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

#include "console.h"
#include "engine.h"
int ConsoleEntry::paintEntry(float x, float y)
{
	// TODO (ivucica#3#) add word wrapping
	g_engine->drawText( (m_speaker + (m_speaker.size() ? ": " : "") + m_text).c_str(), "aafont", (int)x, (int)(y - glictFontNumberOfLines(m_text.c_str())*12), m_color);
	return glictFontNumberOfLines(m_text.c_str())*12;
}

static int consolecount=0;

Console::Console()
{
    m_channelid = 0;
    m_consoleid = (consolecount++);
}
Console::Console(uint32_t channelid)
{
    m_channelid = channelid;
    m_consoleid = (consolecount++);
}
Console::Console(std::string speakername)
{
    m_speakername = speakername;
    m_channelid = 0;
    m_consoleid = (consolecount++);
}

Console::~Console()
{
}
void Console::paintConsole(float left, float top, float right, float bottom)
{
	float x, y;
	x = left; y = bottom;
	for (std::vector<ConsoleEntry>::reverse_iterator it=m_content.rbegin(); it!=m_content.rend(); it++) {
		y -= it->paintEntry(x, y);
		if (y < top)
			break;
	}

}
void Console::insertEntry(ConsoleEntry ce) {
	m_content.insert(m_content.end(), ce);
}
