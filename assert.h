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
