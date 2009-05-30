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

#ifndef __FASSERT_H
#define __FASSERT_H

#if defined(USEASSERT)

	#include <assert.h>
    #include <malloc.h>
    #include <string.h> // strlen
    #include <stdlib.h> // abort

	#include "util.h"
	#define ASSERTFRIENDLY(x,y) if (!(x)) { char *res = (char*)malloc(strlen(y) + 100); sprintf(res,"%s\n\nAt file: " __FILE__ "; in line: %d", (y), __LINE__); printf("Assertion failed!\n\n%s\n", res); NativeGUIError(res, "Assertion failed"); abort(); }
	#define ASSERT(x) ASSERTFRIENDLY(x, "Assertion '" #x "' failed");
#else
	#define ASSERT(x)
	#define ASSERTFRIENDLY(x, y)
#endif

#endif
