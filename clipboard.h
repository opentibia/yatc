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

#ifdef __APPLE__
#include <Carbon/Carbon.h>
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
	/*#ifdef WIN32
	struct {
		int len;
		char* data;
	} strMem;
	#endif*/
	#ifdef __APPLE__
	PasteboardRef m_clipboard;
	#endif
};
