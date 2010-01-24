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

#include <string>

#if defined(USE_X11_CLIPBOARD)
    // NOTE (nfries88): This should include req'd X11 headers for compilation.
	#include <SDL/SDL_syswm.h>
#elif defined(__APPLE__)
    extern "C" const char* getPasteboardText();
    extern "C" void putPasteBoardText(const char*);
#elif !defined(WIN32)
	// NOTE (nfries88): Using an internal std::string on unsupported platforms.
	#define __USE_INTERNAL_CLIPBOARD
#endif

// NOTE (nfries88): we'll only bother supporting text for the clipboard,
//		since that's all the official client uses it for.
class yatcClipboard
{
public:
	yatcClipboard();
	~yatcClipboard();

	std::string getText();
	void setText(const std::string& text);

private:
	#ifdef __USE_INTERNAL_CLIPBOARD
	std::string m_text;
	#elif defined(USE_X11_CLIPBOARD)
    Display *m_display;
    Window m_window;
    void (*m_lockdisplay)(void);
    void (*m_unlockdisplay)(void);

    static int clipboard_filter(const SDL_Event *event);
	#endif
};
