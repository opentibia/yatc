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

#ifndef __CONNECTIONREPLAY_H
#define __CONNECTIONREPLAY_H

// This class performs local file replay services.
// Replay files are of this format:
//
// repeat {
//  <uint32_t> time
//  <uint16_t> message length
//  {
//   message itself
//  }
// }
//
// No encryption or checksum applies.
// Only gameworld is replayable.

#include "connection.h"

class ConnectionReplay : public Connection
{
    public:
        ConnectionReplay(const std::string& file, Protocol* protocol);
        ~ConnectionReplay();

        void executeNetwork();
        void closeConnection();
        void sendMessage(NetworkMessage& msg) {}
    private:
        unsigned long getPendingInput();
        int internalRead(unsigned int n, bool all);
        void checkSocketReadState();

        FILE* m_file;

        uint32_t m_lastticks;
        uint32_t m_nextticks; // time offset of the next message
};

#endif
