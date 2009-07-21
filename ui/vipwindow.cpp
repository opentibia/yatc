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

#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include "vipwindow.h"
#include "sbvlpanel.h"
winVIP_t::winVIP_t()
{
    window.SetWidth(160);
    window.SetHeight(GetDefaultHeight());
    window.SetCaption(gettext("VIP"));
    window.AddObject(&container);
    container.SetWidth(160);
    container.SetHeight(GetDefaultHeight());
    container.SetVirtualSize(160, 270);
    container.SetBGActiveness(false);
}
winVIP_t::~winVIP_t()
{
}


void winVIP_t::addVIP(std::string)
{
}
void winVIP_t::updateVIP(std::string, bool status)
{
}
void winVIP_t::removeVIP(std::string)
{
}

float winVIP_t::GetDefaultHeight()
{
    return 40.F;
}

void winVIP_t::OnClose()
{
	controller->closeVIP();
}
