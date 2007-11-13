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

#include "item.h"

Item::Item(uint16_t id, uint8_t count) : ItemUI(id)
{
	m_id = id;
	m_count = count;
	m_it = Objects::getInstance()->getItemType(m_id);
}

// FIXME (ivucica#1#) mips, I'm so mad right now at you. We said we don't mix graphics code with gamecontent code.  I want this moved back where it belongs -- into ItemUI. I don't want to mess with your code, so I want to separate mine as much as possible
void Item::Blit(int x, int y, float scale) const
{
	BlitItem(x, y, m_count, m_it, scale);
}

Item* Item::CreateItem(const uint16_t type, const uint8_t count /*= 0*/)
{
	return new Item(type, count);
}
