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


#ifndef __PROTOCOLGAME76_H
#define __PROTOCOLGAME76_H

#include "protocolgame74.h"

class ProtocolGame76 : public ProtocolGame74
{
	public:
		virtual ~ProtocolGame76();

        //receive functions
        virtual bool parsePlayerStats(NetworkMessage& msg); // 0xA0
		virtual bool parsePlayerSkills(NetworkMessage& msg); // 0xA1

		//send functions
		//virtual void sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure);
		//virtual void sendGMReport(const std::string& targetplayer, uint8_t reason, const std::string& comment, const std::string& statement, uint8_t action, bool ipban);

		// protocol version assertion
		virtual void checkVersion();
		virtual const char* getProtocolName() { return "Protocol7.6";}
		virtual ClientVersion_t getVersion() const { return CLIENT_VERSION_760; }

	protected:
		ProtocolGame76(const std::string& accountname, const std::string& password, const std::string& name, bool isGM);

		friend class ProtocolConfig;
};

#endif

