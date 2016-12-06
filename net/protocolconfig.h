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

#ifndef __NET_PROTOCOLCONFIG_H
#define __NET_PROTOCOLCONFIG_H

#include <string>
#include "enum.h"
#include "../stdinttypes.h"

class ProtocolGame;

class ProtocolConfig
{
	public:
		~ProtocolConfig() {}

		static ProtocolConfig& getInstance()
		{
			static ProtocolConfig instance;
			return instance;
		}

        static uint32_t readSignature(const char* fn);
        static ClientVersion_t detectVersion();
		void setVersion(ClientOS_t os, ClientVersion_t version);
		void setVersionOverride(uint16_t version);
		void setServerType(ServerType_t type);
		void setServer(const std::string& host, uint16_t port){
			m_host = host;
			m_port = port;
		}

		ClientOS_t getOS() { return m_os; }
		ClientVersion_t getClientVersion() { return m_clientVersion; }
		uint16_t getVersionOverride() { return m_overrideVersion; }
		uint32_t getDatSignature() { return m_datSignature; }
		uint32_t getSprSignature() { return m_sprSignature; }
		uint32_t getPicSignature() { return m_picSignature; }
		ServerType_t getServerType() { return m_serverType; }
		const std::string& getServerHost() {return m_host; }
		uint16_t getServerPort() { return m_port; }

		static void createLoginConnection(const std::string& accountname, const std::string& password);
		static ProtocolGame* createGameConnection(const std::string& accountname, const std::string& password, const std::string& name, bool isGM);
        static ProtocolGame* createGameProtocol(int version, const std::string&accountname, const std::string&password, const std::string&name, bool isGM);

	protected:


		ClientOS_t m_os;
		ClientVersion_t m_clientVersion;
		uint16_t m_overrideVersion;
		uint32_t m_datSignature;
		uint32_t m_sprSignature;
		uint32_t m_picSignature;
		ServerType_t m_serverType;
		std::string m_host;
		uint16_t m_port;

		ProtocolConfig();
};

#endif
