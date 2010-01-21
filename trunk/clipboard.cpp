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

#ifdef WIN32
#include <windows.h>

#include "util.h"
#endif

#ifdef __APPLE__
#warning No clue whether or not Mac OS X clipboard works as of yet.
#endif

#ifdef USE_X11_CLIPBOARD
// Important note: X11 clipboard code is lifted and modified from SDL_scrap (by Sam Lantinga) with his explicit permission.
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#endif

#include "clipboard.h"

yatcClipboard::yatcClipboard()
{
	#ifdef __USE_INTERNAL_CLIPBOARD
	m_text = "";
	#elifdef USE_X11_CLIPBOARD
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
    if(SDL_GetWMInfo(&info))
    {
        /* Save the information for later use */
        if (info.subsystem == SDL_SYSWM_X11)
        {
            m_display = info.info.x11.display;
            m_window = info.info.x11.window;
            m_lockdisplay = info.info.x11.lock_func;
            m_unlockdisplay = info.info.x11.unlock_func;

            /* Enable the special window hook events */
            SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
            SDL_SetEventFilter(clipboard_filter);
        }
        else
        {
            NativeGUIError(("SDL is not running on X11!\n"
                            "Please undefine USE_X11_CLIPBOARD\n"
                            "and rebuild " PRODUCTSHORT "."), "Clipboard Failure");
        }
    }
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
    Window owner;
    Atom selection;
    Atom seln_type;
    int seln_format;
    unsigned long nbytes;
    unsigned long overflow;
    char *src;

    m_lockdisplay();
    owner = XGetSelectionOwner(m_display, XA_PRIMARY);
    m_unlockdisplay();
    if((owner == None) || (owner == m_window))
    {
        owner = DefaultRootWindow(m_display);
        selection = XA_CUT_BUFFER0;
    }
    else
    {
        int selection_response = 0;
        SDL_Event event;

        owner = m_window;
        m_lockdisplay();
        selection = XInternAtom(m_display, "SDL_SELECTION", False);
        XConvertSelection(m_display, XA_PRIMARY, XA_STRING,
                                        selection, owner, CurrentTime);
        m_unlockdisplay();
        // Question to ask Sam about this part of code: Is it proper to just drop any events
        // that are unfortunately before the selection event?
        while (!selection_response)
        {
            SDL_WaitEvent(&event);
            if(event.type == SDL_SYSWMEVENT)
            {
                XEvent xevent = event.syswm.msg->event.xevent;

                if((xevent.type == SelectionNotify) &&
                     (xevent.xselection.requestor == owner))
                    selection_response = 1;
            }
        }
    }
    m_lockdisplay();
    if(XGetWindowProperty(m_display, owner, selection, 0, INT_MAX/4,
                            False, XA_STRING, &seln_type, &seln_format,
                       &nbytes, &overflow, (unsigned char **)&src) == Success)
    {
        if (seln_type == XA_STRING)
        {
            return std::string(src);
        }
        XFree(src);
    }
    m_unlockdisplay();
	#elif defined(__APPLE__)
	const char* ret = getPasteboardText();
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
	m_lockdisplay();
    XChangeProperty(m_display, DefaultRootWindow(m_display),
        XA_CUT_BUFFER0, XA_STRING, 8, PropModeReplace, src, srclen);
    if (lost_scrap())
        XSetSelectionOwner(m_display, XA_PRIMARY, m_window, CurrentTime);
    m_unlockdisplay();
	#elif defined(__APPLE__)
	putPasteboardText(text.c_str());
	#endif
}

#ifdef USE_X11_CLIPBOARD
int clipboard_filter(const SDL_Event *event)
{
    /* Post all non-window manager specific events */
    if(event->type != SDL_SYSWMEVENT) {
        return(1);
    }

    /* Handle window-manager specific clipboard events */
    switch (event->syswm.msg->event.xevent.type) {
        /* Copy the selection from XA_CUT_BUFFER0 to the requested property */
        case SelectionRequest: {
            XSelectionRequestEvent *req;
            XEvent sevent;
            int seln_format;
            unsigned long nbytes;
            unsigned long overflow;
            unsigned char *seln_data;

            req = &event->syswm.msg->event.xevent.xselectionrequest;
            sevent.xselection.type = SelectionNotify;
            sevent.xselection.display = req->display;
            sevent.xselection.selection = req->selection;
            sevent.xselection.target = None;
            sevent.xselection.property = None;
            sevent.xselection.requestor = req->requestor;
            sevent.xselection.time = req->time;
            if(XGetWindowProperty(m_display, DefaultRootWindow(m_display),
                              XA_CUT_BUFFER0, 0, INT_MAX/4, False, req->target,
                              &sevent.xselection.target, &seln_format,
                              &nbytes, &overflow, &seln_data) == Success)
            {
                if (sevent.xselection.target == req->target)
                {
                    if (sevent.xselection.target == XA_STRING)
                    {
                        if (seln_data[nbytes-1] == '\0')
                            --nbytes;
                    }
                    XChangeProperty(m_display, req->requestor, req->property,
                        sevent.xselection.target, seln_format, PropModeReplace,
                                                      seln_data, nbytes);
                    sevent.xselection.property = req->property;
                }
                XFree(seln_data);
            }
            XSendEvent(m_display,req->requestor,False,0,&sevent);
            XSync(m_display, False);
        }
        break;
    }

    /* Post the event for X11 clipboard reading above */
    return(1);
}
#endif
