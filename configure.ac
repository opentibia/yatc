dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([yatc],[0.2.3SVN])
AC_CONFIG_SRCDIR([spritesdl.h])
AM_INIT_AUTOMAKE([foreign])

AC_REVISION($Revision: 1.0 $)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)

AC_ARG_ENABLE(debug, [  --enable-debug          enable debug], [
	DEBUG_FLAGS=-DDEBUGLEVEL_BUILDTIME=5
	AC_SUBST(DEBUG_FLAGS)
],[
	DEBUG_FLAGS=-DDEBUGLEVEL_BUILDTIME=0
	AC_SUBST(DEBUG_FLAGS)
])

AC_ARG_ENABLE(assert, [  --enable-assert          enable assertions], [
	AC_HEADER_ASSERT
	ASSERT_FLAGS=-DUSEASSERT
	AC_SUBST(ASSERT_FLAGS)
])

AC_ARG_ENABLE(opengl, [  --disable-opengl          don't use opengl], [
	AM_CONDITIONAL(USE_OPENGL, false)
],[
	OPENGL_FLAGS=-DUSE_OPENGL
	AC_SUBST(OPENGL_FLAGS)
	AM_CONDITIONAL(USE_OPENGL, true)
])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])
#
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([getenv])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_SIZE_T


#check GMP
AC_CHECK_HEADERS([gmp.h], ,[AC_MSG_ERROR("*** GMP header not found.")])
AC_CHECK_LIB(gmp, __gmpz_init2, ,[AC_MSG_ERROR("*** GMP library not found or < 4.0.")])

if test -n "$OPENGL_FLAGS"; then
	#check GL
	AC_CHECK_HEADERS(GL/gl.h, ,[AC_MSG_ERROR("*** OpenGL header not found. (You may want to use --disable-opengl, more info in INSTALL)")])
	AC_CHECK_LIB(GL, glLoadIdentity, ,[
		AC_CHECK_LIB(opengl32, glLoadIdentity, , [AC_MSG_ERROR("*** OpenGL library not found. (You may want to use --disable-opengl, more info in INSTALL)")])
	])

	#check GLU
	AC_CHECK_HEADERS(GL/glu.h, ,[AC_MSG_ERROR("*** GLU header not found.")])
	AC_CHECK_LIB(GLU, gluBuild2DMipmaps, ,[
		AC_CHECK_LIB(glu32, gluBuild2DMipmaps, , [AC_MSG_ERROR("*** GLU library not found.")])
	])
fi

# check for GLICT
AC_CHECK_HEADERS([GLICT/globals.h],,[AC_MSG_ERROR("*** GLICT header not found.")])
LIBS="$LIBS -lGLICT"

#check SDL
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :, AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"
AC_SUBST(SDL_LIBS)


#check SDL-gfx
AC_CHECK_HEADERS([SDL/SDL_rotozoom.h], ,[AC_MSG_ERROR("*** SDL_gfx headers not found.")])
## Following test does not work with any Windows-targetting compiler (mingw and mingw32ce included)
## More precisely, over there SDL wants SDL_main and not main, which the standard AC test does
## not provide.
#AC_CHECK_LIB(SDL_gfx, rotozoomSurfaceXY, ,[AC_MSG_ERROR("*** SDL_gfx library not found.")])
LIBS="$LIBS -lSDL_gfx"


#LIBS="$LIBS gamecontent/__all.a net/__all.a"


# output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
])

AC_OUTPUT
