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

#ifndef __GLICT_WINDOW_H
#define __GLICT_WINDOW_H
#include <string>
#include <GLICT/container.h>
#include <GLICT/panel.h>
#include <GLICT/types.h>
#include <GLICT/globals.h>
/// A draggable window that can contain subelements. Similar to many other "windowing systems" (but a lot uglier and flatter)
class glictWindow : public glictContainer {
    public:
        glictWindow();
        virtual ~glictWindow();

        void SetBGColor(float r, float g, float b, float a);

        // redefined functions
        virtual void Paint(); ///< Paints the window.
		//void AddObject(glictContainer *object);
		//void RemoveObject(glictContainer *object);
        virtual bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue); ///< Reacts to events in a way a window should.
        float GetTopSize() const {if (glictGlobals.windowBodySkin) return glictGlobals.windowBodySkin->GetTopSize().h; else return 0; }
        float GetBottomSize() const {if (glictGlobals.windowBodySkin) return glictGlobals.windowBodySkin->GetBottomSize().h; else return 0; }
        void FixContainerOffsets();

        virtual void SetWidth(float w);
        virtual void SetHeight(float h);
        virtual void SetPos(float x, float y);

        void AddTitlebarObject(glictContainer* object);
        void RemoveTitlebarObject(glictContainer* object);

    private:
        glictColor bgcolor;
		glictPanel titlebarpanel;
    friend class glictContainer;
};
#endif

