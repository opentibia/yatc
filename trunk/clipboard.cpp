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

#include "clipboard.h"

yatcClipboard::yatcClipboard()
{
	#ifdef __USE_INTERNAL_CLIPBOARD
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
	#elif defined(__APPLE__)
	return getPasteboardText();
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
	#elif defined(__APPLE__)
	putPasteboardText(text.c_str());
	#endif
}
