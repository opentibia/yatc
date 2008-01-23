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


#ifndef __UTIL_H
#define __UTIL_H

#include <string>
void str_replace(std::string &s, const std::string& what, const std::string& with);
void NativeGUIError(const char* text, const char *title);
bool fileexists(const char* filename);
FILE *yatc_fopen(const char* filename, const char* mode);
void yatc_fopen_init();
#define nextpow(n) (n <= 2 ? 2 : (n <= 4 ? 4 : (n <= 8 ? 8 : (n <= 16 ? 16 : (n <= 32 ? 32 : (n <= 64 ? 64 : (n <= 128 ? 128 : (n <= 256 ? 256 : (n <= 512 ? 512 : 1024)))))))))

//	NOTE: If you get an error compiling on MSVC
//	add the preprocessor definition _CRT_SECURE_NO_DEPRECATE

inline char* yatc_itoa(int val)
{
	char* buffer = new char[64];
	sprintf(buffer, "%d", val);
	return buffer;
}

#endif
