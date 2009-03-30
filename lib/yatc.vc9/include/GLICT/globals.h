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

#ifndef __GLICT_GLOBALS_H
#define __GLICT_GLOBALS_H

#include <GLICT/container.h>
#include <GLICT/skinner.h>
#include <GLICT/types.h>


typedef void (*GLICTDEBUGCALLBACKPROC )(unsigned int len, const char *string);
typedef void (*GLICTPAINTRECTPROC)( float left, float right, float top, float bottom, const glictColor &color );
typedef void (*GLICTCLIPPERPROC)( float left, float right, float top, float bottom );

/// Stores some global settings.
class glictGlobalsClass {
    public:
        glictGlobalsClass();
        ~glictGlobalsClass();

        glictColor windowTitleBgColor;
        glictColor windowTitleColor;
        glictSkinner *windowTitleSkin;
        glictSkinner *windowBodySkin;
        glictSkinner *buttonSkin, *buttonHighlightSkin;
        glictSkinner *textboxSkin;
        glictSkinner *scrollbarUpperSkin, *scrollbarUpperHighlightSkin;
        glictSkinner *scrollbarLowerSkin, *scrollbarLowerHighlightSkin;
        glictSkinner *scrollbarLeftSkin, *scrollbarLeftHighlightSkin;
        glictSkinner *scrollbarRightSkin, *scrollbarRightHighlightSkin;
        glictSkinner *scrollbarPanelSkin;
        glictSkinner *scrollbarDragSkin;
        glictColor buttonTextColor, buttonHighlightTextColor;
        glictColor panelTextColor;
        glictColor textboxTextColor;

        float w,h;
        glictPos lastMousePos; // last coordinates where user clicked
        glictClippingMode clippingMode;
        glictContainer* topFocused;

        bool enableGlTranslate;
        glictPos translation;

        bool drawPartialOut;
        bool drawFocus;

		bool mayUseGL;

		GLICTDEBUGCALLBACKPROC debugCallback;
		GLICTPAINTRECTPROC paintrectCallback;
		GLICTPAINTRECTPROC paintrectlinesCallback;
        GLICTCLIPPERPROC clipperCallback;

		// functions
		void Translatef(float, float, float);
		void PaintRect(float left, float right, float top, float bottom);
		void PaintRect(float left, float right, float top, float bottom, const glictColor &col);
		void PaintRect(const glictRect &r, const glictColor &col);
		void PaintRectLines(float top, float bottom, float left, float right);
		void PaintRectLines(float left, float right, float top, float bottom, const glictColor &col);
		void SetColor(float r, float g, float b, float a);
};
extern glictGlobalsClass glictGlobals;

#define GLICT_RENDERING false
#define GLICT_SELECTING true

#define GLICT_APIREV 98 // from which subversion repository revision does this install come (manually changed upon relevant API changes)

#endif
