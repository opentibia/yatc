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
 * \file fonts.h
 * \brief Font management
 *
 * Declares stuff used in fonts.cpp
 * \sa fonts.cpp
 */
#ifndef __GLICT_FONTS_H
#define __GLICT_FONTS_H

#include <string>
#include <vector>
#include <GLICT/types.h>
typedef bool(*_glictFontRenderFuncBoolNoSize)(const char* text, const void* font, float x, float y) ;
typedef bool(*_glictFontRenderFuncBoolSize)(const char* text, const void* font, float size, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidNoSize)(const char* text, const void* font, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidSize)(const char* text, const void* font, float size, float x, float y) ;


typedef bool(*_glictFontRenderFuncBoolNoSize)(const char* text, const void* font, float x, float y) ;
typedef bool(*_glictFontRenderFuncBoolSize)(const char* text, const void* font, float size, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidNoSize)(const char* text, const void* font, float x, float y) ;
typedef void(*_glictFontRenderFuncVoidSize)(const char* text, const void* font, float size, float x, float y) ;


typedef float(*_glictFontSizeFuncSize)(const char* text, const void* font, float size) ;
typedef float(*_glictFontSizeFuncNoSize)(const char* text, const void* font) ;

typedef void(*_glictFontColorFunc)(const void* font, glictColor &col) ;


/// Represents one font usable by GLICT. (GLICT currently uses only one, "system")
class glictFont {
    public:
        glictFont();
        ~glictFont();

        void SetName(const char* newname);
        std::string GetName();
        void SetFontParam(void* font);

        void* GetFontParam () {return fontparam;}

        void SetRenderFunc(_glictFontRenderFuncBoolNoSize funcpointer);
        void SetRenderFunc(_glictFontRenderFuncBoolSize funcpointer);
        void SetRenderFunc(_glictFontRenderFuncVoidNoSize funcpointer);
        void SetRenderFunc(_glictFontRenderFuncVoidSize funcpointer);

        void SetSizeFunc(_glictFontSizeFuncSize funcpointer);
        void SetSizeFunc(_glictFontSizeFuncNoSize funcpointer);


        void SetColorFunc(_glictFontColorFunc funcpointer);
    private:
        std::string name;
        _glictFontRenderFuncBoolNoSize RenderBoolNoSize;
        _glictFontRenderFuncBoolSize RenderBoolSize;
        _glictFontRenderFuncVoidNoSize RenderVoidNoSize;
        _glictFontRenderFuncVoidSize RenderVoidSize;

        _glictFontSizeFuncSize SizeSize;
        _glictFontSizeFuncNoSize SizeNoSize;

        _glictFontColorFunc SetColor;

        glictColor activecolor;
        void* fontparam;

    friend bool glictFontRender(const char* text, const char* fontname, float x, float y);
    friend bool glictFontRender(const char* text, const char* fontname, float fontsize, float x, float y);

    friend float glictFontSize(const char* name, const char* font);
    friend float glictFontSize(const char* name, const char* font, float size);

    friend void glictFontColor(const char* font, glictColor &col);
    friend glictColor glictFontColor(const char* font);

};



typedef std::vector<glictFont*> _GLICTFONTVECTOR;
typedef _GLICTFONTVECTOR::iterator _GLICTFONTVECTORITERATOR;


glictFont* glictCreateFont(const char* name);
glictFont* glictFindFont(const char* name);
bool glictDeleteFont(const char* name);

float glictFontSize(const char* name, const char* font);
float glictFontSize(const char* name, const char* font, float size);

int glictFontNumberOfLines(const char* text);

bool glictFontRender(const char* text, const char* fontname, float x, float y);
bool glictFontRender(const char* text, const char* fontname, float fontsize, float x, float y);

void glictFontColor(const char* fontname, glictColor &col);
glictColor glictFontColor(const char* fontname);

#endif
