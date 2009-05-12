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
#endif

#include "clipboard.h"

yatcClipboard::yatcClipboard()
{
	#ifdef WIN32
	/*strMem.len = 0;
	strMem.data = NULL;*/
	#endif
}
yatcClipboard::~yatcClipboard()
{
}

std::string yatcClipboard::getText()
{
	#ifdef WIN32
	OpenClipboard(NULL);
	std::string ret = (const char*)GetClipboardData(CF_TEXT);
	CloseClipboard();
	return ret;
	#endif
}
void yatcClipboard::setText(const std::string& text)
{
	#ifdef WIN32
	/*if(strMem.data) {
		delete [] strMem.data;
	}
	strMem.len = text.length()+1;
	strMem.data = new char[strMem.len];*/

	OpenClipboard(NULL);
	SetClipboardData(CF_TEXT, (HANDLE)text.c_str());
	CloseClipboard();
	#endif
}
