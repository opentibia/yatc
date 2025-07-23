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
#ifndef __USE_GLSDL__
#include <SDL/SDL.h>
#else
#include <SDL/glSDL.h>
#endif
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include "stdinttypes.h"
#include "gamecontent/enums.h"
#include "popup.h"

class Console;

class ConsolePanel: public glictPanel
{
public:
	ConsolePanel();
	virtual ~ConsolePanel();

	virtual void SetHeight(float h);
	virtual void SetWidth(float w);
	virtual bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);

	void SetActiveConsole(Console* console);
	void MakeConsole(Console* console, const std::string& name);
	void DeleteConsole(Console* console);

    glictPanel pnlConsoleResizer;
	glictPanel pnlConsoleEntryContainer;
	glictPanel pnlConsoleEntryView;
	glictTextbox txtConsoleEntry;
	std::vector<glictPanel*> pnlConsoleButtons;
    glictPanel pnlConsoleButtonContainer;

    glictButton btnSpeakLevel;
    glictButton btnClose;
    glictButton btnM;
    glictButton btnChannelList;
    glictButton btnIgnore;
protected:
	static void pnlConsoleButton_OnClick(glictPos* relmousepos, glictContainer* caller);
	static void onPaintConsole(glictRect* real, glictRect* clipped, glictContainer* callerclass);
	static void onClickConsole(glictPos* relmousepos, glictContainer* callerclass);

	// popup stuff
	static void makeConsolePopup(Popup* popup, void* owner, void* arg);
	static void onUnimplemented(Popup::Item *parent);
	static void onCopyMessage(Popup::Item *parent);
	static void onMessageTo(Popup::Item *parent);

	static void makeConsoleBtnPopup(Popup* popup, void* owner, void* arg);
	static void onCloseConsole(Popup::Item *parent);
	static void onShowM(Popup::Item *parent);
	static void onSaveConsole(Popup::Item *parent);
	static void onClearConsole(Popup::Item *parent);

	// button clicks
	static void btnSpeak_OnClick(glictPos* relmousepos, glictContainer* caller);
};

class ConsoleEntry {
	public:
		ConsoleEntry(std::string text, TextColor_t c=TEXTCOLOR_WHITE)
		{
			m_text = text;
			m_speaker = "";
			m_color = c;
			m_timestamp = time(NULL);
			m_level = -1;
		}
		ConsoleEntry(std::string text, std::string speaker, int level, TextColor_t c=TEXTCOLOR_WHITE)
		{
			m_text = text;
			m_speaker = speaker;
			m_color = c;
			m_timestamp = time(NULL);
			m_level = level;
		}
		int paintEntry(float x, float y, float width = -1);
		int getHeight();
		const std::string& getSpeaker() const { return m_speaker; }
		std::string getFullText() const;
		TextColor_t getColor() const { return m_color; }
	private:
		std::string m_text, m_speaker;
		TextColor_t m_color;
		uint32_t m_timestamp;
		int m_level;
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

		ConsoleEntry* getConsoleEntryAt(float relx, float rely);
		void clearEntries();
		void dumpText();

		int getHeight();

		// Returns the last entry in the console (for tests).
		const ConsoleEntry& getLastEntry() const { return m_content.back(); }
	private:
		std::vector <ConsoleEntry> m_content;
		std::string m_speakername;
		uint32_t m_channelid;
		int m_consoleid;
		glictPanel* m_assignedButton;
};

#endif
