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

#ifndef __ASSERT_H
#define __ASSERT_H


#if defined(USEASSERT) && !defined(ASSERTFRIENDLY)
	#ifdef WIN32
    	#include <windows.h>
        #include <assert.h>

		#ifdef ASSERT
			#undef ASSERT
		#endif

        #define ASSERT(x) assert(x);
        #define ASSERTFRIENDLY(x, y) if (!(x)) { MessageBox(HWND_DESKTOP, y, "Something unpredicted happened! :(", MB_ICONSTOP); assert(x); }
    #else

		#ifdef ASSERT
			#undef ASSERT
		#endif

        #define ASSERT(x) if (!(x)) {fprintf(stderr, "Assertion failure, forcing crash\n"); exit(1); } /* crash is intentionally done this way, because then we can see in which line did program crash with core dump; if there's an assertor for GNU/Linux i'd love to see it */
        #define ASSERTFRIENDLY(x, y) if (!(x)) { printf("Assertion failure, forcing crash. (Reason: %s)\n", y); fflush(stdout); exit(1);/*printf("%d", 43/0);*/ }
    #endif
#else
    #define ASSERT(x)
    #define ASSERTFRIENDLY(x, y)
#endif


#endif
