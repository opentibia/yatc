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
enum ConsoleColor {
	CONRED = 0,
	CONBLUE = 1,
	CONYELLOW = 2,
	CONORANGE = 3,
	CONLTBLUE = 4,
	CONGREEN = 5,
	CONWHITE = 6
};

class ConsoleEntry {
	public:
		ConsoleEntry(std::string text, ConsoleColor c=CONWHITE)
		{
			m_text = text;
			m_speaker = "";
			m_color = c;
			m_timestamp = time(NULL);
		}
		ConsoleEntry(std::string text, std::string speaker, ConsoleColor c=CONYELLOW)
		{
			m_text = text;
			m_speaker = speaker;
			m_color = c;
			m_timestamp = time(NULL);
		}
		int Paint(float x, float y);
	private:
		std::string m_text, m_speaker;
		ConsoleColor m_color;
		uint32_t m_timestamp;
};
class Console {
	public:
		Console();
		virtual ~Console();

		void Paint(float left, float top, float right, float bottom);
		void Insert(ConsoleEntry ce);
	private:
		std::vector <ConsoleEntry> m_content;
		SDL_Surface *m_surface;
};
#endif
