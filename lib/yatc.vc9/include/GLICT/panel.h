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

#ifndef __GLICT_PANEL_H
#define __GLICT_PANEL_H

#include <GLICT/container.h>
#include <GLICT/scrollbar.h>
#include <GLICT/types.h>
#include <GLICT/skinner.h>


/// Simple rectangular widget that can contain children, has changeable background color, can have a skin, and has a virtual area
class glictPanel : public glictContainer {
    public:
        glictPanel();
        ~glictPanel();

        void SetBGColor(float r, float g, float b, float a);

        // redefined functions
        virtual void Paint();
        virtual bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);

        void SetBGActiveness(bool bg);

        virtual void SetVirtualSize(float w, float h);
        virtual void SetHeight(float h);
		void VirtualScrollBottom();
		void SetSkin(glictSkinner *skin);
		void SetTextOffset(int x, int y) { textoffx = x; textoffy = y; }
		inline bool IsVScrollbarVisible() const { return virtualsize.h > height; }
		inline int GetCurrentVScrollbarWidth() const { return IsVScrollbarVisible() ? GetVScrollbarWidth() : 0; }
    private:
        glictColor bgcolor;
        glictSkinner *skin;
        bool bgactive;

        glictScrollbar sbVertical, sbHorizontal;
        int textoffx, textoffy;
    protected:
        int GetVScrollbarWidth() const;
        int GetVScrollbarInternalWidth() const;


};
#endif
