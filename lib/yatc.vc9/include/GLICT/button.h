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
 * \file button.h
 * \brief Button class header
 * \sa glictButton
 */

#ifndef __GLICT_BUTTON_H
#define __GLICT_BUTTON_H

#include <string>

#include <GLICT/container.h>
#include <GLICT/types.h>

class glictSkinner;

/**
 * \brief Button class widget
 *
 * Buttons are clickable widgets that have a text written on the center of it,
 * with settable background color. Upon click on a button, the function
 * specified as OnClick function is called.
 */
class glictButton : public glictContainer {
    public:
        glictButton(); ///< Constructor for the button widget.
        ~glictButton(); ///< Destructor for the button widget.

        void SetBGColor(float r, float g, float b, float a); ///< Sets the background color of the button.
        void SetFGColor(float r, float g, float b, float a); ///< Sets the foreground color of the button.

		void SetHold (bool holdvalue);

        inline void SetSkin(glictSkinner* newskin)           { skin  = newskin; }
        inline void SetHighlightSkin(glictSkinner* newskin)  { hskin = newskin; }

        // redefined functions
        void Paint(); ///< Paints the button.
        bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue); ///< Event processing for button widget.


    private:
        glictColor bgcolor; ///< Stores the background color.
        glictColor fgcolor; ///< Stores the foreground color.
        bool highlighted; ///< Is mouse pressed or not
        bool hold; ///< Should we render as if mouse is pressed no matter what
        glictSkinner *skin, *hskin; ///< Which skin should we use. Overrides the glictGlobals setting
};
#endif

