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

#ifndef __UI_TUTORIALHINTS_H
#define __UI_TUTORIALHINTS_H

#include <string>
#include <map>
#include "../stdinttypes.h"

class UITutorialHints {
    public:
    static UITutorialHints& getInstance() {
        static UITutorialHints instance;
        return instance;
    }
    const std::string& getTutorialHint(uint8_t id);
    bool showHint(uint8_t id) {
        if (m_tutorialhints.find(id) != m_tutorialhints.end())
            return m_tutorialhints[id].size() > 0;
        else
            return true; // show the "not found" text

    }
    private:
    UITutorialHints();
    std::map<uint8_t, std::string> m_tutorialhints;

};


#endif
