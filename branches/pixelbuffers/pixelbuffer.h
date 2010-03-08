/*
 *  pixelbuffer.h
 *  yatc
 *
 *  Created by Ivan Vučica on 08.03.2010..
 *  Copyright 2010 OpenTibia Team. All rights reserved.
 *
 */

#ifndef PIXELBUFFER_H_INCLUDED
#define PIXELBUFFER_H_INCLUDED

struct SDL_Surface;
class PixelBuffer
{
public:
	PixelBuffer() : mPixels(NULL),w(0),h(0),bpp(0) {}
	PixelBuffer(SDL_Surface* original, bool doCopy=true) : mPixels(NULL),w(0),h(0),bpp(0)
	{
		createFromSDLSurface(original,doCopy);
	};
	PixelBuffer(int _w, int _h, int _bpp) : mPixels(NULL), w(_w), h(_h), bpp(_bpp)
	{
		mPixels = (char*)malloc(w*h*bpp);
	}
	virtual ~PixelBuffer() 
	{
		free(mPixels);
	}
	virtual char* lockSelf()   {return mPixels;}
	virtual void  unlockSelf() {}
	
	int getW() const {return w;}
	int getH() const {return h;}
	int getBPP() const {return bpp;}

	virtual void performCrop(int x, int y, int w, int h) 
	{
		// this function should implement generic crop of the content of mPixels
		
		printf("%s: stub!\n", __PRETTY_FUNCTION__);
	}

	
	// getSurface returns an SDL surface and is implemented only while in SDL engine
	// using it in any other engine is incorrect!
	virtual SDL_Surface* getSurface() const 
	{ 
		printf("%s: Calling on non-SDL pixelbuffer\n", __PRETTY_FUNCTION__); 
		return NULL; 
	}
	
	virtual uint32_t mapRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		// TODO (ivucica#8#): depends on arch?
		return a << 24 | b << 16 | g << 8 | r;
	}
	
	
	virtual void fillRect(uint32_t color, int x=0,int y=0,int w=0,int h=0)
	{
		// FIXME does not use x,y,w,h
		for (int i=0;i<w*h;i++)
		{
			
			for(int j=0;j<bpp;j++)
			{
				char *byte=mPixels+i*bpp+j;
				// FIXME presumes bpp of 32, otherwise uses only red channel
				*byte = (color >> j * 8) & 0xFF;
			}
		}
		
	}
	
protected:
	int w,h,bpp;
	char* mPixels;

private:
	virtual void createFromSDLSurface(SDL_Surface* original, bool doCopy=true)
	{
		// generic createFromSDLSurface() is always performing a memcpy()
		// it ignores doCopy
		
		w=original->w;
		h=original->h;
		bpp=original->format->BitsPerPixel;
		mPixels=(char*)malloc(w*h * bpp);
			
		SDL_LockSurface(original);
		memcpy(mPixels, original->pixels, w*h*bpp);
		SDL_UnlockSurface(original);
		
		
	}
	
};

#include <SDL/SDL.h>
class PixelBufferSDL : public PixelBuffer
{
public:
	PixelBufferSDL(int _w, int _h, int _bpp) : PixelBuffer(_w,_h,_bpp) 
	{
		
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		uint32_t rmask = 0xff000000;
		uint32_t gmask = 0x00ff0000;
		uint32_t bmask = 0x0000ff00;
		uint32_t amask = 0x000000ff;
#else
		uint32_t rmask = 0x000000ff;
		uint32_t gmask = 0x0000ff00;
		uint32_t bmask = 0x00ff0000;
		uint32_t amask = 0xff000000;
#endif
		
		
		mSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, _w, _h, _bpp, rmask, gmask, bmask, amask);
	}
	PixelBufferSDL(SDL_Surface* original, bool doCopy)
	{
		// when using doCopy, this function is also converting 
		// to display format this may or may not be what you want to do...
		
		if(doCopy)
			mSurface = SDL_DisplayFormat(original);
		else 
			mSurface = original;
		
		w = mSurface->w;
		h = mSurface->h;
		bpp = mSurface->format->BitsPerPixel;
	}
	void performCrop(int x, int y, int w, int h)
	{
		
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		uint32_t rmask = 0xff000000;
		uint32_t gmask = 0x00ff0000;
		uint32_t bmask = 0x0000ff00;
		uint32_t amask = 0x000000ff;
#else
		uint32_t rmask = 0x000000ff;
		uint32_t gmask = 0x0000ff00;
		uint32_t bmask = 0x00ff0000;
		uint32_t amask = 0xff000000;
#endif
		
		
		SDL_Surface* ns = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
		SDL_Rect src = {x,y,w,h};
		SDL_Rect dst = {0,0,w,h};
		SDL_BlitSurface(mSurface, &src, ns, &dst);
	
		SDL_FreeSurface(mSurface);
		mSurface = ns;

	}
	
	uint32_t mapRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		return SDL_MapRGBA(mSurface->format,r,g,b,a);
	}
	void fillRect(uint32_t color, int x=0,int y=0,int w=0,int h=0)
	{
		// FIXME does not use x,y,w,h
		SDL_FillRect(mSurface, NULL, color);
	}
protected:
	SDL_Surface *mSurface;
};

class PixelBufferGL : public PixelBuffer
{
public:
	PixelBufferGL(int _w,int _h,int _bpp) : PixelBuffer(_w,_h,_bpp) {}
	PixelBufferGL(SDL_Surface*original,bool doCopy) : PixelBuffer(original,doCopy) {}
	
};


#endif