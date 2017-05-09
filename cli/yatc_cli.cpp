//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// CLI version.
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
#include <iostream>
#include "../net/connection.h"
#include "../net/protocolconfig.h"

std::string g_recordfilename="debugrecord.rec";

float g_frameTime = 0;
float g_frameDiff = 0;
Connection *g_connection = NULL;

int main(int argc, const char ** argv) {
  ClientVersion_t proto = CLIENT_VERSION_854;
  std::cout << "Warning: In case of crash after this line check you put Tibia.{dat,spr,pic} in CWD." << std::endl << std::flush;
  ProtocolConfig::getInstance().setServerType(SERVER_OTSERV);
  ProtocolConfig::getInstance().setVersion(CLIENT_OS_WIN, proto);
  ProtocolConfig::getInstance().setVersionOverride(CLIENT_VERSION_AUTO);
  ProtocolConfig::getInstance().setServer("localhost", 7171);

  ProtocolConfig::createLoginConnection("1", "1");

  for (;;)
    {
      g_connection->executeNetwork();
    }
  return 0;
}
