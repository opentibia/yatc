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
	#elif defined(__APPLE__)
	OSStatus err = PasteboardCreate(kPasteboardClipboard, &m_clipboard);
	// TODO: error handling?
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
	OSStatus err = noErr;
	ItemCount count;
	CFArrayRef flavorArray;
	// synchronize pasteboard
	PasteboardSyncFlags sflags;
	sflags = PasteboardSynchronize(m_clipboard);
	// get the number of things in the clipboard
	count = PasteboardGetItemCount(m_clipboard, &count);
	if((int)count >= 1)
	{
		PasteboardItemID itemID;
        CFArrayRef flavorTypeArray;
        CFIndex flavorCount;
		err = PasteboardGetItemIdentifier(m_clipboard, (ItemCount)1, &itemID);
		// determine if data is text and retrieve it if so
		err = PasteboardCopyItemFlavors(m_clipboard, itemID, &flavorTypeArray);
		flavorCount = CFArrayGetCount(flavorTypeArray);
		for(CFIndex flavorIndex = 0; flavorIndex < flavorCount; flavorIndex++)
		{
			CFStringRef flavorType;
            CFDataRef flavorData;
            CFIndex flavorDataSize;

			flavorType = (CFStringRef)CFArrayGetValueAtIndex(flavorTypeArray, flavorIndex);
			// note (nfries88): the example I'm using converts utf16 into a char type, I'd rather not unless
			//	it's found to be needed.
			if (UTTypeConformsTo(flavorType, CFSTR("public.utf8-plain-text")))
			{
				err = PasteboardCopyItemFlavorData(m_clipboard, itemID, flavorType, &flavorData);
                flavorDataSize = CFDataGetLength(flavorData);
                std::string ret = (const char*)CFDataGetBytePtr(flavorData);
                CFRelease(flavorData);
                return ret;
			}
		}
	}
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
	OSStatus err = noErr;
	// synchronize pasteboard
	PasteboardSyncFlags sflags;
	sflags = PasteboardSynchronize(m_clipboard);
	// add the data
	CFDataRef textdata = CFDataCreate(kCFAllocatorDefault, (UInt8*)text.c_str(), text.length());
	err = PasteboardPutItemFlavor(m_clipboard, (PasteboardItemID)1, CFSTR("public.utf8-plain-text"), textdata, 0);
	// TODO: handle errors?
	#endif
}
