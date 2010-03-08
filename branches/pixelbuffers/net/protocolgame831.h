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


#ifndef __PROTOCOLGAME831_H
#define __PROTOCOLGAME831_H

#include "protocolgame83.h"

class Thing;
class Item;

class ProtocolGame831 : public ProtocolGame83
{
	public:
		virtual ~ProtocolGame831();

		virtual const char* getProtocolName() { return "Protocol831";}
		virtual ClientVersion_t getVersion() const { return CLIENT_VERSION_831; }

	protected:
		ProtocolGame831(const std::string& accountname, const std::string& password, const std::string& name, bool isGM);

		virtual void checkVersion();

		friend class ProtocolConfig;
};

#endif


