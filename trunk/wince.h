#ifndef __WINCE_H
#define __WINCE_H

#ifdef WINCE

#define WINCE_INSTALLDIR "/Storage Card/YATC/"
#define fopen(fn, mode)  fopen_wince(fn, mode)

/* FIXME (ivucica#5#) move to a separate file */
static FILE* fopen_wince(const char* fn, const char* mode) {
	/* ugly as hell, but WINCE's fopen does not use current working dir
	 * in its path ... 
	 * something also tells me SDL_LoadBMP() won't like this, but
	 * we'll be avoiding it anyway */
	char fopen_fn[255]; 
	sprintf(fopen_fn, "%s%s", WINCE_INSTALLDIR, fn); 
	return fopen(fopen_fn, mode);
}

#endif

#endif

