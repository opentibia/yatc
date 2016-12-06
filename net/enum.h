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

#ifndef __NET_ENUM_H
#define __NET_ENUM_H

enum ClientVersion_t{
	CLIENT_VERSION_AUTO = 0, // used only in ui, feeding it elsewhere is invalid
	//note (nfries88): Although many client versions are listed here, not all of them are supported yet.
	CLIENT_VERSION_710 = 710,

	CLIENT_VERSION_740 = 740,

	CLIENT_VERSION_750 = 750,
    CLIENT_VERSION_755 = 755,

	CLIENT_VERSION_760 = 760,

	CLIENT_VERSION_770 = 770,
	CLIENT_VERSION_771 = 771,
	CLIENT_VERSION_772 = 772,

	CLIENT_VERSION_780 = 780,
	CLIENT_VERSION_781 = 781,
	CLIENT_VERSION_782 = 782,

	CLIENT_VERSION_790 = 790,
	CLIENT_VERSION_791 = 791,
	CLIENT_VERSION_792 = 792,

	CLIENT_VERSION_800 = 800,
	CLIENT_VERSION_810 = 810,
	CLIENT_VERSION_811 = 811,

	CLIENT_VERSION_820 = 820,
	CLIENT_VERSION_821 = 821,
	CLIENT_VERSION_822 = 822,

	CLIENT_VERSION_830 = 830,
	CLIENT_VERSION_831 = 831,

	CLIENT_VERSION_840 = 840,
	CLIENT_VERSION_841 = 841,
	CLIENT_VERSION_842 = 842,

	CLIENT_VERSION_850 = 850,
	CLIENT_VERSION_853 = 853,
	CLIENT_VERSION_854 = 854
};

enum ClientOS_t{
	CLIENT_OS_LINUX = 1,
	CLIENT_OS_WIN = 2
};

enum ServerType_t{
	SERVER_CIP = 1,
	SERVER_OTSERV = 2
};

#endif
