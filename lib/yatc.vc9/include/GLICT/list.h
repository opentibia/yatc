/*
	GLICT - Graphics Library Interface Creation Toolkit
	Copyright (C) 2006-2008 Ivan Vucica

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

#ifndef __GLICT_LIST_H
#define __GLICT_LIST_H

#include <GLICT/panel.h>
#include <GLICT/types.h>
class glictList : public glictPanel {
    public:
        glictList();
        ~glictList();

        void AddObject(glictContainer* object);
        void RemoveObject(glictContainer *object);

        virtual void SetWidth(float w);
        virtual void SetHeight(float h);


        void RebuildList();
        void SetForcedHeight(int forcedheight);
        int GetForcedHeight() const { return forcedheight; }
    protected:
        int forcedheight;
        int totalheight;

        bool defocusabilize_element; // for internal adjustment; should the element be made unfocusable? modified only by inherited classes
        std::list<glictContainer*> listlist;
};

#endif
