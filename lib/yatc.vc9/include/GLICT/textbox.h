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
 * \file textbox.h
 * \brief Textbox
 * \sa glictTextbox
 */

#ifndef __GLICT_TEXTBOX_H
#define __GLICT_TEXTBOX_H

#include <GLICT/container.h>
#include <GLICT/types.h>

/** \brief Rectangle shaped area that accepts keypress events and changes its
  * own caption accordingly.
  *
  * Rectangle shaped area that accepts keypress events and changes its own
  * caption accordingly. If it contains focus, an underscore character is
  * appended. No scrolling (meaning PalmOS style.)
  *
  * \todo If textcursor goes out of bounds of textbox, scroll the text
  *       appropriatelly to follow the cursor
  */
class glictTextbox : public glictContainer {
    public:
        glictTextbox();
        ~glictTextbox();

        void SetBGColor(float r, float g, float b, float a);

        // redefined functions
        void Paint();
        bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);


		void SetPassProtectCharacter(char asterisk); ///< Used to define which character will be used to display instead of regular characters.
		void SetAllowedChars (std::string s); ///< Used to define which characters are allowed to be typed. If '\n' is ommitted then it's not allowed either
    private:
        glictColor bgcolor;
		char passprotectchar;
		std::string allowedchr;
};
#endif
