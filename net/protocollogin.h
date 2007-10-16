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

#ifndef __PROTOCOLLOGIN_H
#define __PROTOCOLLOGIN_H

#include <string>
#include "connection.h"

class ProtocolLogin : public Protocol
{
	public:
		virtual ~ProtocolLogin() {};

		void setCallback(void*);

		virtual void onConnect();
		virtual bool onRecv(NetworkMessage& msg);

	protected:
		ProtocolLogin(int account, const std::string& password);

		const char* getProtocolName() { return "Protocol Login"; }

		std::string m_password;
		int m_account;

		friend class ProtocolConfig;
};

#endif
