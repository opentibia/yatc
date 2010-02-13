/*(LGPL)
------------------------------------------------------------
	glSDL 0.8 - SDL 2D API on top of OpenGL
------------------------------------------------------------
 * Copyright (C) 2001-2004, 2006 David Olofson
 * This code is released under the terms of the GNU LGPL.
 */

#ifndef	_GLSDL_H_
#define	_GLSDL_H_

/*
 * If you don't use GNU autotools or similar, uncomment this to
 * compile with OpenGL enabled:
#define HAVE_OPENGL
 *
 * NOTE:
 *	See README about using this glSDL wrapper with
 *	SDL versions that have the glSDL backend!
 */

/* We're still using SDL datatypes here - we just add some stuff. */
#include "SDL.h"

/*
 * Ignore the flag from SDL w/ glSDL backend, since we're going
 * to use glSDL/wrapper by default.
 */
#ifdef	SDL_GLSDL
#undef	SDL_GLSDL
#endif

#ifndef HAVE_OPENGL

/* Fakes to make glSDL code compile with SDL. */
#define SDL_GLSDL		0
#define	GLSDL_FIX_SURFACE(s)

#else	/* HAVE_OPENGL */

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

#if (SDL_MAJOR_VERSION <= 1) && (SDL_MINOR_VERSION <= 2) &&	\
		(SDL_PATCHLEVEL < 5)
#warning glSDL: Using SDL version 1.2.5 or later is strongly recommended!
#endif

/*----------------------------------------------------------
	SDL style API
----------------------------------------------------------*/

typedef enum glSDL_TileModes
{
	GLSDL_TM_SINGLE,
	GLSDL_TM_HORIZONTAL,
	GLSDL_TM_VERTICAL,
	GLSDL_TM_HUGE
} glSDL_TileModes;


typedef struct glSDL_TexInfo
{
	int		textures;
	int		*texture;
	int		texsize;	/* width/height of OpenGL texture */
	glSDL_TileModes	tilemode;
	int		tilew, tileh;	/* At least one must equal texsize! */
	int		tilespertex;

	/* Area of surface to download when/after unlocking */
	SDL_Rect	invalid_area;
} glSDL_TexInfo;

#define	GLSDL_FIX_SURFACE(s)	(s)->unused1 = 0;
#define	IS_GLSDL_SURFACE(s)	((s) && texinfotab && glSDL_GetTexInfo(s))

#ifdef	SDL_GLSDL
#undef	SDL_GLSDL	/* In case SDL has the glSDL backend...  */
#endif
#define SDL_GLSDL	0x00100000	/* Create an OpenGL 2D rendering context */


/*
 * Wraps SDL_SetVideoMode(), and adds support for the SDL_GLSDL flag.
 *
 * If 'flags' contains SDL_GLSDL, glSDL_SetVideoMode() sets up a "pure"
 * OpenGL rendering context for use with the glSDL_ calls.
 *
 * SDL can be closed as usual (using SDL_ calls), but you should call
 * glSDL_Quit() (kludge) to allow glSDL to clean up it's internal stuff.
 */
SDL_Surface *glSDL_SetVideoMode(int width, int height, int bpp, Uint32 flags);
void glSDL_Quit(void);

void glSDL_QuitSubSystem(Uint32 flags);

/* Replaces SDL_Quit() entirely, when using the override defines */
void glSDL_FullQuit(void);

SDL_Surface *glSDL_GetVideoSurface(void);

void glSDL_UpdateRects(SDL_Surface *screen, int numrects, SDL_Rect *rects);
void glSDL_UpdateRect(SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 w, Uint32 h);

/*
 * Works like SDL_Flip(), but may also perform enqueued blits.
 * (That is, it's possible that the implementation renders
 * *nothing* until glSDL_Flip() is called.)
 */
int glSDL_Flip(SDL_Surface *screen);

void glSDL_FreeSurface(SDL_Surface *surface);

int glSDL_LockSurface(SDL_Surface *surface);
void glSDL_UnlockSurface(SDL_Surface *surface);

/*
 * Like the respective SDL functions, although they ignore
 * SDL_RLEACCEL, as it makes no sense in this context.
 */
int glSDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key);
int glSDL_SetAlpha(SDL_Surface *surface, Uint32 flag, Uint8 alpha);

/*
 * Sets up clipping for the screen, or a SDL_Surface.
 *
 * Note that this function takes both SDL_Surfaces and
 * glSDL_Surfaces.
 */
SDL_bool glSDL_SetClipRect(SDL_Surface *surface, SDL_Rect *rect);

int glSDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect,
			 SDL_Surface *dst, SDL_Rect *dstrect);

int glSDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);

/*
 * Convert the given surface into a SDL_Surface (if it isn't
 * one already), and makes sure that the underlying SDL_Surface
 * is of a pixel format suitable for fast texture downloading.
 *
 * Note that you *only* have to use this function if you want
 * fast pixel access to surfaces (ie "procedural textures").
 * Any surfaces that aren't converted will be downloaded
 * automatically upon the first call to glSDL_BlitSurface(),
 * but if conversion is required, it will be required for
 * every glSDL_UnlockSurface() call.
 *
 * IMPORTANT:
 *	You *can* pass an SDL_Surface directly to this function,
 *	and it will try to deal with it nicely. However, this
 *	requires that a temporary SDL_Surface is created, and
 *	this surface is cached only until the texture memory is
 *	needed for new surfaces.
 */
SDL_Surface *glSDL_DisplayFormat(SDL_Surface *surface);
SDL_Surface *glSDL_DisplayFormatAlpha(SDL_Surface *surface);

SDL_Surface *glSDL_ConvertSurface
			(SDL_Surface *src, SDL_PixelFormat *fmt, Uint32 flags);
SDL_Surface *glSDL_CreateRGBSurface
			(Uint32 flags, int width, int height, int depth, 
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
SDL_Surface *glSDL_CreateRGBSurfaceFrom(void *pixels,
			int width, int height, int depth, int pitch,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
SDL_Surface *glSDL_LoadBMP(const char *file);
int glSDL_SaveBMP(SDL_Surface *surface, const char *file);



/*----------------------------------------------------------
	glSDL API extensions, transparent
----------------------------------------------------------*/

/*
 * Invalidate part of a texture.
 *
 * This function can be used either between calls to
 * glSDL_LockSurface() and glSDL_UnlockSurface(), or before
 * calling glSDL_DownloadSurface().
 *
 * In either case, it causes only the specified area to be
 * downloaded when unlocking the surface, or calling
 * glSDL_UnlockSurface(), respectively.
 *
 * Note that if this function is not used, glSDL assumes that
 * the entire surface has to be updated. (That is, it's safe
 * to ignore this function - it's "just a performance hack.")
 *
 * Passing a rectangle with zero height or width cancels the
 * downloading when/after unlocking the surface. Use if you
 * just want to read the texture, but feel like being nice and
 * obeying SDL_MUSTLOCK() - which is a good idea, as things
 * may change...
 *
 * Passing NULL for the 'area' argument results in the entire
 * surface being invalidated.
 *
 * NOTE: This function does NOT perform clipping! Weird or
 *       even Bad Things may happen if you specify areas
 *       that protrude outside the edges of the actual
 *       surface.
 */
void glSDL_Invalidate(SDL_Surface *surface, SDL_Rect *area);

/*
 * Make sure that the texture of the specified surface is up
 * to date in OpenGL texture memory.
 *
 * This can be used together with glSDL_UnloadSurface() to
 * implement custom texture caching schemes.
 *
 * Returns 0 on success, or a negative value if something
 * went wrong.
 */
int glSDL_UploadSurface(SDL_Surface *surface);

/*
 * Free the texture space used by the specified surface.
 *
 * Normally, glSDL should download textures when needed, and
 * unload the oldest (in terms of use) surfaces, if it runs out
 * of texture space.
 */
void glSDL_UnloadSurface(SDL_Surface *surface);


/*----------------------------------------------------------
	glSDL API extensions, OpenGL mode ONLY
----------------------------------------------------------*/

/*
 * Set alpha multiplier. This is multiplied with any full
 * surface alpha or alpha channels of surfaces.
 *
 * The default value is 255, which makes glSDL render like
 * the standard SDL 2D backends, ie based only on the
 * surface settings.
 *
 * This function affects the global state of glSDL.
 *
 * NOTE: NOT available without OpenGL acceleration!
 */
void glSDL_SetBlendAlpha(Uint8 alpha);

/*
 * Set color multipliers. These are multiplied with the
 * respective color channers of rendered surfaces,
 * modulating the brightness of the resulting output.
 *
 * The default values are 255, which makes glSDL render
 * like the standard SDL 2D backends.
 *
 * This function affects the global state of glSDL.
 *
 * NOTE: NOT available without OpenGL acceleration!
 */
void glSDL_SetBlendColor(Uint8 r, Uint8 g, Uint8 b);

/*
 * Set center offset in pixels for scaling and rotation.
 *
 * The default is (0, 0), which makes surfaces rotate and
 * scale around their centers.
 *
 * This function affects the global state of glSDL.
 *
 * NOTE: NOT available without OpenGL acceleration!
 */
void glSDL_SetCenter(float x, float y);


/*
 * Set rotation angle in degrees. Rotation is done around
 * the center point, which is defined as
 *	(w/2 + cx, h/2 + cy)
 * where w and h are the width and height of the source
 * surface, and cx and cy are the center offsets set by
 * glSDL_SetCenter().
 *
 * The default value is 0.0f, which makes glSDL render
 * like a normal SDL 2D backend.
 *
 * This function affects the global state of glSDL.
 *
 * NOTE: NOT available without OpenGL acceleration!
 */
void glSDL_SetRotation(float angle);

/*
 * Set scale factor. Scaling is centered around around
 * the point
 *	(w/2 + cx, h/2 + cy)
 * where w and h are the width and height of the source
 * surface, and cx and cy are the center offsets set by
 * glSDL_SetCenter().
 *
 * The default value is 1.0f, which makes glSDL render
 * like a normal SDL 2D backend.
 *
 * This function affects the global state of glSDL.
 *
 * NOTE: NOT available without OpenGL acceleration!
 */
void glSDL_SetScale(float x, float y);

/*
 * Reset glSDL global state to defaults.
 */
void glSDL_ResetState(void);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

/* Some ugly "overriding"... */
#ifndef	_GLSDL_NO_REDEFINES_
/*
 * You *always* need to lock and unlock a glSDL surface in
 * order to get glSDL to update the OpenGL texture!
 */
#undef	SDL_MUSTLOCK
#define SDL_MUSTLOCK(surface)   \
  (surface->offset ||           \
  ((surface->flags & (SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_RLEACCEL)) != 0) ||	\
  IS_GLSDL_SURFACE(surface))

#define	SDL_SetVideoMode		glSDL_SetVideoMode
#define	SDL_GetVideoSurface		glSDL_GetVideoSurface
#define	SDL_Quit			glSDL_FullQuit
#define	SDL_QuitSubSystem		glSDL_QuitSubSystem
#define	SDL_UpdateRects			glSDL_UpdateRects
#define	SDL_UpdateRect			glSDL_UpdateRect
#define	SDL_Flip			glSDL_Flip
#define	SDL_FreeSurface			glSDL_FreeSurface
#define	SDL_LockSurface			glSDL_LockSurface
#define	SDL_UnlockSurface		glSDL_UnlockSurface
#define	SDL_SetColorKey			glSDL_SetColorKey
#define	SDL_SetAlpha			glSDL_SetAlpha
#define	SDL_SetClipRect			glSDL_SetClipRect
#undef	SDL_BlitSurface
#define	SDL_BlitSurface			glSDL_BlitSurface
#define	SDL_FillRect			glSDL_FillRect
#define	SDL_DisplayFormat		glSDL_DisplayFormat
#define	SDL_DisplayFormatAlpha		glSDL_DisplayFormatAlpha
#define	SDL_ConvertSurface		glSDL_ConvertSurface
#define	SDL_CreateRGBSurface		glSDL_CreateRGBSurface
#define	SDL_CreateRGBSurfaceFrom	glSDL_CreateRGBSurfaceFrom
#undef	SDL_AllocSurface
#define SDL_AllocSurface		glSDL_CreateRGBSurface
#undef	SDL_LoadBMP
#define	SDL_LoadBMP			glSDL_LoadBMP
#undef	SDL_SaveBMP
#define	SDL_SaveBMP			glSDL_SaveBMP
#define IMG_Load(x)			glSDL_IMG_Load(x)
#endif

/* Some extra overloading for common external lib calls... */
#include "SDL_image.h"
#ifdef __cplusplus
extern "C" {
#endif
SDL_Surface *glSDL_IMG_Load(const char *file);
#ifdef __cplusplus
}
#endif

#endif	/* HAVE_OPENGL */

#endif
