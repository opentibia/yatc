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

#ifndef DEBUGPRINT
#include <string>


#define DEBUGPRINT /*DEBUG_LINE = __LINE__; DEBUG_FILE = __FILE__; */DEBUGPRINTx
// debugprint types
#define DEBUGPRINT_NORMAL -1
#define DEBUGPRINT_ERROR -2
#define DEBUGPRINT_WARNING -3
// debugprint levels
#define DEBUGPRINT_LEVEL_OBLIGATORY 0
#define DEBUGPRINT_LEVEL_USEFUL 1
#define DEBUGPRINT_LEVEL_DEBUGGING 2
#define DEBUGPRINT_LEVEL_JUNK 3

extern char debuglevel;
void DEBUGPRINTx (char msgdebuglevel, char type, const char* txt, ...);
extern std::string DEBUG_FILE; extern int DEBUG_LINE;
void DEBUGMARKER (unsigned int size, const char *val);

#endif
