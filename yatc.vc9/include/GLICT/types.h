/*
    GLICT - Graphics Library Interface Creation Toolkit
    Copyright (C) 2006-2007 OBJECT Networks

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/



/**
 * \file types.h
 * \brief Global library's types
 */

#ifndef _GLICT_TYPES_H
#define _GLICT_TYPES_H


/**
 * \brief Used to store either position or size. (later typedefed to equal glictSize - contains unions so both names can be used for elements)
 *
 * It appears that doxygen doesnt handle unions well. For that purpose I'll list them here:
 * <int x, y;>
 * or
 * <int w, h;>
 */
typedef struct {
    union {
        float x,w; ///< X coordinate of position or width
    };
    union {
        float y,h; ///< Y coordinate of position or height
    };
} glictPos;


/**
 * \brief Same as glitctPos. Just a typedef to aforementioned struct.
 */
typedef glictPos glictSize;


/**
 * \brief Used to store all color elements supported by OpenGL - red, green, blue and alpha channel. Stored as float. Values 0.f-1.f
 */
struct glictColor {
	glictColor() {}
	glictColor (float red,float green, float blue, float alpha) { r=red; g = green; b = blue; a = alpha; }
    /// Red, green, blue and alpha element.
    float r, g, b, a;
} ;

/**
 * \brief Stores a rectangular portion of the screen
 */
typedef struct glictRect_s {
    /// Left, right, top and bottom.
    float left, right, top, bottom;
} glictRect;

/**
 * \brief Enumerates events that can occur.
 */
enum glictEvents {
    GLICT_MOUSECLICK = 0, ///< Mouse click event
    GLICT_MOUSEDOWN = 1, ///< Mouse down event
    GLICT_MOUSEUP = 2, ///< Mouse up event
    GLICT_MOUSEMOVE = 6, ///< Mouse move event
    GLICT_KEYPRESS = 3, ///< Key press event
    GLICT_KEYDOWN = 4, ///< Key down event
    GLICT_KEYUP = 5 ///< Key release event
};

/**
 * \brief Enumerates scissor modes that can be used.
 */
enum glictClippingMode {
    GLICT_NOCLIP = 0, ///< Do not use clipping; max performance, UI designer needs to be careful
    GLICT_SCISSORTEST = 1, ///< Use scissoring test; cannot be used with transformations
    GLICT_STENCILTEST = 2 ///< Use stencil testing; compatible with transformations, slow
};


#endif
