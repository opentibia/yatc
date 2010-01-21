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
#include <sstream>
#if defined(WIN64) || __WORDSIZE == 64
    #define VOIDP2INT(x) (   static_cast<int>(reinterpret_cast<long long>(x)) )
#else
    #define VOIDP2INT(x) ((int)(x))
#endif
std::string str_replace(const std::string& what, const std::string& with, const std::string &s);
void NativeGUIError(const char* text, const char *title);
bool fileexists(const char* filename);
void yatc_fread(void* ptr, size_t size, size_t count, FILE *stream);
void yatc_fwrite(void* ptr, size_t size, size_t count, FILE *stream);
FILE *yatc_fopen(const char* filename, const char* mode);
void yatc_fopen_init(char* cmdline);
std::string yatc_findfile(const char* filename);
#define nextpow(n) (n <= 2 ? 2 : (n <= 4 ? 4 : (n <= 8 ? 8 : (n <= 16 ? 16 : (n <= 32 ? 32 : (n <= 64 ? 64 : (n <= 128 ? 128 : (n <= 256 ? 256 : (n <= 512 ? 512 : 1024)))))))))

inline std::string yatc_itoa(int val)
{
	std::stringstream b;
	b << val;
	return b.str();
}

// Support on apple for cmd+c copying and cmd+v pasting, as well as old ctrl+c/ctrl+v copy/paste.
#ifdef __APPLE__
#define ISCOPYEVENT(key) (key == 3 || (key == SDLK_c && superkey_state))
#define ISPASTEEVENT(key) (key == 22 || (key == SDLK_v && superkey_state))
#else
#define ISCOPYEVENT(key) (key==3)
#define ISPASTEEVENT(key) (key==22)
#endif

#endif
