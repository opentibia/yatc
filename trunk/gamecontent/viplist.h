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

#ifndef VIPLIST_H
#define VIPLIST_H

#include <string>
#include <map>
#if !defined(_MSC_VER)
#include <stdint.h>
#else
#include <SDL/SDL_config.h> // sdl conveniently gives us stdints under msvc
#endif
class VipEntry
{
    public:
    VipEntry()
    {
        m_creatureid = 0;
        m_vipname = "";
        m_online = false;
    }
    VipEntry(uint32_t creatureID, const std::string& vipname, bool online)
    {
        m_creatureid = creatureID;
        m_vipname = vipname;
        m_online = online;
    }
    ~VipEntry() { }

    bool isOnline() const { return m_online; }
    void setOnline(bool y) { m_online = y; }
    std::string getName() const { return m_vipname; }

    private:
    uint32_t m_creatureid;
    std::string m_vipname;
    bool m_online;
};

class VipList
{
    public:
    VipList();
    ~VipList();

    static VipList& getInstance();

    void setEntry(uint32_t id, const std::string& name, bool online)
    {
        m_entries[id] = VipEntry(id, name, online);
    }
    void setEntry(uint32_t id, bool online)
    {
        std::map<uint32_t,VipEntry>::iterator it = m_entries.find(id);
        if (it != m_entries.end())
        {
            it->second.setOnline(online);
        }
    }


    VipEntry getEntry(uint32_t id) const
    {
        std::map<uint32_t,VipEntry>::const_iterator it = m_entries.find(id);
        if (it != m_entries.end())
        {
            return it->second;
        }
        else
        {
            return VipEntry(id, "",false);
        }
    }
    bool entryIsOnline(uint32_t id) const
    {
        std::map<uint32_t,VipEntry>::const_iterator it = m_entries.find(id);
        if (it != m_entries.end())
            return it->second.isOnline();
        else
            return false;
    }
    void removeEntry(uint32_t id)
    {
        std::map<uint32_t,VipEntry>::iterator it = m_entries.find(id);
        m_entries.erase(it);
    }


    const std::map<uint32_t, VipEntry>& getEntries() const
    {
        return m_entries;
    }

    private:
    std::map<uint32_t, VipEntry> m_entries;
};

#endif
