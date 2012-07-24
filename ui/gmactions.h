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

#ifndef __UI_GMACTIONS_H
#define __UI_GMACTIONS_H

#include <string>
#include <map>
#include "../stdinttypes.h"
#include "../net/connection.h" // clientversion_t
class UIGMActions {
    public:
    static UIGMActions& getInstance() {
        static UIGMActions instance;
        return instance;
    }
    const std::string& getAction(ClientVersion_t version, uint8_t id);
    const std::string& getReason(ClientVersion_t version, uint8_t id);

    bool showAction(ClientVersion_t version, uint8_t id) {
        if (action.find(version) == action.end())
            return true;
        else if (action[version].find(id) != action[version].end())
            return action[version][id].size() > 0;
        else
            return true; // show the "not found" text
    }
    bool showReason(ClientVersion_t version, uint8_t id) {
        if (reason.find(version) == reason.end())
            return true;
        else if (reason[version].find(id) != reason[version].end())
            return reason[version][id].size() > 0;
        else
            return true; // show the "not found" text
    }


    private:
    UIGMActions();
    std::map<ClientVersion_t, std::map<uint8_t, std::string> > action, reason;

};


#endif
