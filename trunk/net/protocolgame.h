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


#ifndef __PROTOCOLGAME_H
#define __PROTOCOLGAME_H

#include <string>
#include "connection.h"

class NetworkMessage;

class ProtocolGame : public Protocol
{
	public:
		ProtocolGame(int account, const std::string& password, const std::string& name, bool isGM);
		virtual ~ProtocolGame();

		virtual void onConnect();
		virtual bool onRecv(NetworkMessage& msg) = 0;

		//send functions - pure virtual

	protected:
		std::string m_password;
		std::string m_name;
		int m_account;
		bool m_isGM;
		NetworkMessage m_outputMessage;
};

#endif

