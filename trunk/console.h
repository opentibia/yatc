//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Console
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

#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <time.h>
#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "stdinttypes.h"
#include "gamecontent/enums.h"

class glictPanel;

class ConsoleEntry {
	public:
		ConsoleEntry(std::string text, TextColor_t c=TEXTCOLOR_WHITE)
		{
			m_text = text;
			m_speaker = "";
			m_color = c;
			m_timestamp = time(NULL);
		}
		ConsoleEntry(std::string text, std::string speaker, TextColor_t c=TEXTCOLOR_WHITE)
		{
			m_text = text;
			m_speaker = speaker;
			m_color = c;
			m_timestamp = time(NULL);
		}
		int paintEntry(float x, float y);
	private:
		std::string m_text, m_speaker;
		TextColor_t m_color;
		uint32_t m_timestamp;
};
class Console {
	public:
		Console();
		Console(uint32_t channelid);
		Console(std::string speakername);
		virtual ~Console();

		void paintConsole(float left, float top, float right, float bottom);
		void insertEntry(ConsoleEntry ce);

        const std::string& getSpeakerName() const { return m_speakername; }
        uint32_t getChannelId()  const { return m_channelid; }
        int getConsoleId() const {return m_consoleid;}

        void setAssignedButton(glictPanel *pnl) { m_assignedButton = pnl; }
        glictPanel *getAssignedButton() const { return m_assignedButton; }
	private:
		std::vector <ConsoleEntry> m_content;
		SDL_Surface *m_surface;
		std::string m_speakername;
		uint32_t m_channelid;
		int m_consoleid;
		glictPanel* m_assignedButton;
};

#endif
