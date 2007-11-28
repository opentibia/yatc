//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Utility functions
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

#include <sstream>
#include "util.h"

#ifdef WIN32
#include <windows.h>
#endif
#include "wince.h"


void str_replace(std::string &s, const std::string& what, const std::string& with) {
	std::string::size_type p, l;
	for (l = 0; (p = s.find(what, l)) != std::string::npos; l = p + with.size()) {
		s.replace(p, what.size(), with);
	}
}


void NativeGUIError(const char* text, const char *title) {
	#ifdef WIN32
		#ifndef WINCE
			MessageBox(HWND_DESKTOP, text, title, MB_ICONSTOP);
		#else
			int a, b;

			BSTR unicodetext = SysAllocStringLen(NULL, a=strlen(text));
			BSTR unicodetitle = SysAllocStringLen(NULL, b=strlen(title));
			MultiByteToWideChar(CP_ACP, 0, text, a, unicodetext, a);
			MultiByteToWideChar(CP_ACP, 0, title, b, unicodetitle, b);



			MessageBox(HWND_DESKTOP, unicodetext, unicodetitle, MB_ICONSTOP);
			SysFreeString(unicodetext);
			SysFreeString(unicodetitle);

		#endif
	#else
		std::string texts=text, titles=title;
		str_replace(texts, "\\", "\\\\");
		str_replace(texts, "\"", "\\\"");

		std::stringstream cmd;
		cmd << "xmessage -title \"" << title << "\" -center \"" << texts << "\"";

		system(cmd.str().c_str());
	#endif
}

bool fileexists(const char* filename){
    FILE *f;
    if ((f = fopen(filename, "r"))) {
        fclose(f);
        return true;
    } else
        return false;
}
