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

#include <stdlib.h>

#ifdef WIN32
#include <windows.h>

#include "util.h"
#endif

#ifdef __APPLE__
#warning No clue whether or not Mac OS X clipboard works as of yet.
#endif

#ifdef USE_X11_CLIPBOARD
// NOTE (nfries88): now we use GtkClipboard... old code didn't work, I guess.
bool callback_called = false;
#endif

#include "clipboard.h"

yatcClipboard g_clipboard;

yatcClipboard::yatcClipboard()
{
	#if defined(__USE_INTERNAL_CLIPBOARD) || defined(USE_X11_CLIPBOARD)
	m_text = "";
	#endif
}
yatcClipboard::~yatcClipboard()
{
}

std::string yatcClipboard::getText()
{
	#ifdef WIN32
	OpenClipboard(GetActiveWindow());
	const char* str = (const char*)GetClipboardData(CF_TEXT);
	if(str == NULL)
		return "";
	std::string ret = str;
	CloseClipboard();
	return ret;
	#elif defined (__USE_INTERNAL_CLIPBOARD)
	return m_text;
	#elif defined(USE_X11_CLIPBOARD)
	// grab the clipboard
    GtkClipboard* clipboard = gtk_clipboard_get(gdk_atom_intern("CLIPBOARD", FALSE));
    // return stored text in case of epic failure
    if(!clipboard) return m_text;
    // request text from a callback
    gtk_clipboard_request_text(clipboard, yatcClipboard::clipboard_filter, (gpointer)this);
    // make gtk call the callback
    do{
        gtk_main_iteration_do((gboolean)false);
    } while(!callback_called)
    callback_called = false;

    return m_text;
	#elif defined(__APPLE__)
	char* ret = getPasteboardText();
	std::string str = ret;
	free(ret);  // ret is allocated within getPasteboardText
	return str;
	#endif
	return "";
}
void yatcClipboard::setText(const std::string& text)
{
	#ifdef WIN32
	OpenClipboard(GetActiveWindow());
	EmptyClipboard();

	HGLOBAL m_hmem = GlobalAlloc(GMEM_MOVEABLE, (text.length()+1));
	LPVOID mem = GlobalLock(m_hmem);

	ZeroMemory(mem, (text.length()+1));
	strncpy((char*)mem, text.c_str(), text.length());
	GlobalUnlock(m_hmem);

	SetClipboardData(CF_OEMTEXT, m_hmem);
	CloseClipboard();
	#elif defined (__USE_INTERNAL_CLIPBOARD)
	m_text = text;
	#elif defined(USE_X11_CLIPBOARD)
	m_text = text;
	// grab the clipboard
    GtkClipboard* clipboard = gtk_clipboard_get(gdk_atom_intern("CLIPBOARD", FALSE));
    // stop in case of epic failure
    if(!clipboard) return;
    // set the text
    gtk_clipboard_set_text(clipboard, (const gchar*)m_text.str(), (gint)m_text.length());
    // store the data so it remains after yatc closes
    gtk_clipboard_store(clipboard);
	#elif defined(__APPLE__)
	putPasteboardText(text.c_str());
	#endif
}

#ifdef USE_X11_CLIPBOARD
void yatcClipboard::clipboard_filter(GtkClipboard *clipboard, GdkAtom format, const guint8 *text, gsize length, gpointer data)
{
    yatcClipboard* self = (yatcClipboard*)data;
    if(text != NULL){
        self->m_text = text;
        gfree((gpointer)text);
    }

    callback_called = true;
}
#endif
