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
 * \file container.h
 * \brief Container class header
 * \sa glictContainer
 */



#ifndef _GLICT_CONTAINER_H
#define _GLICT_CONTAINER_H
#include <vector>
#include <list>
#include <string>
#include <GLICT/types.h>

using namespace std;


/**
 * \brief Container class, base for all other widgets
 *
 * This is the Container class, which is actually a base class for all
 * other widgets. Althought this one does not render itself, it actually
 * contains most of reusable code for the widgets to inherit.
 * An example of these inherited functionalities are children management,
 * children rendering, proper clipping, mouse coordinate transformation, et al.
 */
class glictContainer  {
	public:
		glictContainer(); ///< Constructor for the class.
		glictContainer(long guid); ///< Constructor for the class, specifying guid. (Guid is unused at the moment)
		virtual ~glictContainer(); ///< Destructor for the class.

		// the following elements are replaced, not necessarily inherited in child  classes
		virtual void Paint(); ///< Renders the element. Should contain call to CPaint().
		virtual bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue); ///< Casts an event to the class, so it can process it. Example is mouseclick or mousedown.

		// the following elements are always inherited from container. some MAY be overriden, in special circumstances, but it is not recommended.
		virtual void AddObject(glictContainer* object); ///< Adds an object as a child.
		virtual void RemoveObject(glictContainer* object); ///< Removes an object that's a child.
		void CPaint(); ///< Renders all children.
		void DelayedRemove(); ///< Runs delayed removal of objects.
		virtual void SetHeight(float h); ///< Sets object's height.
		virtual void SetWidth(float w); ///< Sets object's width.
		virtual void SetPos(float x, float y); ///< Sets object's position using classical access using two integers.
		virtual void SetPos(glictPos pos); ///< Sets object's position using predefined type. Useful when exchanging data between library's functions.
		void GetPos(float* x, float* y); ///< Gets object's position and writes it into integers x and y.
		void GetPos(glictPos* pos); ///< Gets object's position and writes it into predefined type. Useful when exchanging data between library's functions.
		void GetSize(glictSize* size); ///< Gets object's size (height and width) and writes it into predefined type.
		void SetScissor(); ///< This one adjusts the clipping window through which something can be seen, and which is set by SetClip
		void SetVisible(bool visibility); ///< Sets visibility.
		bool GetVisible(); ///< Retrieves visibility.
		void SetEnabled(bool enabled); ///< Sets enabledness.
		bool GetEnabled(); ///< Retrieves enabledness.
		void RememberTransformations(); ///< When calling parent's paint, call it's 'remember transformations' too, so clicking detection is done properly. if clicking unused, or no transformations done, then not important
		void ResetTransformations(); ///< Resets transformations to default transf matrix (identity matrix)
		void TransformScreenCoords(glictPos *pos); ///< Transforms screen coordinates into plane coordinates
		float GetHeight(); ///< Returns object's height
		float GetWidth(); ///< Returns object's width
		inline float GetX() const {return x;}  ///< Returns object position's x coordinate
		inline float GetY() const {return y;}  ///< Returns object position's y coordinate
		inline float GetTop() const { return top; } ///< Returns top of object's overall rectangle.
		inline float GetBottom() const { return bottom; } ///< Returns bottom of object's overall rectangle.
		inline float GetLeft() const { return left; } ///< Returns left of object's overall rectangle.
		inline float GetRight() const { return right; } ///< Returns right of object's overall rectangle.
		inline float GetClippedTop() const { return cliptop; } ///< Returns top of object's overall rectangle.
		inline float GetClippedBottom() const { return clipbottom; } ///< Returns bottom of object's overall rectangle.
		inline float GetClippedLeft() const { return clipleft; } ///< Returns left of object's overall rectangle.
		inline float GetClippedRight() const { return clipright; } ///< Returns right of object's overall rectangle.

		virtual void SetVirtualSize(float w, float h);
		virtual void VirtualScrollBottom();

		void SetPrevious(glictContainer*);
		void SetNext(glictContainer*);
		glictContainer* GetPrevious() {return previous;}
		glictContainer* GetNext() {return next;}

		bool CastEvent(glictEvents evt, void* wparam, long lparam); ///< Casts an event omitting the returnvalue. (deprecated, I'm lazy and don't want to rewrite code so I abuse namespace)
		bool DefaultCastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue); ///< Casts an event into default event processor, omitting the widget's code.

		void SetOnClick(void(*OnClickFunction)(glictPos* relmousepos, glictContainer* callerclass)); ///< Sets a function to execute upon click. OBJECT MUST NOT DESTROY ITSELF OR REMOVE ITSELF FROM OBJECT LIST OF ITS PARENT FROM WITHIN.
		void SetOnPaint(void(*OnPaintFunction)(glictRect* real, glictRect* clipped, glictContainer* callerclass)); ///< Sets a function to execute whenever drawing. OBJECT MUST NOT DESTROY ITSELF OR REMOVE ITSELF FROM OBJECT LIST OF ITS PARENT FROM WITHIN.
		void SetOnMouseDown(void(*FunctionPtr)(glictPos* relmousepos, glictContainer* callerclass)); ///< Sets a function to execute upon mouse pressed down over this object. OBJECT MUST NOT DESTROY ITSELF OR REMOVE ITSELF FROM OBJECT LIST OF ITS PARENT FROM WITHIN.
		void SetOnMouseUp(void(*FunctionPtr)(glictPos* relmousepos, glictContainer* callerclass)); ///< Sets a function to execute upon mouse pressed down over this object. OBJECT MUST NOT DESTROY ITSELF OR REMOVE ITSELF FROM OBJECT LIST OF ITS PARENT FROM WITHIN.
		void SetCaption(const std::string caption); ///< Sets the caption of the control, if supported.
		std::string GetCaption(); ///< Retrieves the caption of the control, if supported.

		const char *EvtTypeDescriptor(glictEvents evt); ///< Returns string with generated event.
		virtual void Focus(glictContainer* callerchild); ///< Sets the focus to the current object.

		glictContainer* GetParent() const;

		void ReportDebug(); ///< Reports debug information to stdout.

		virtual void RecursiveBoundaryFix(); ///< Fixes boundaries recursively up to the root of the tree (desktop, probably). Also used to make scrollbars appear in case of virtual size, where appropriate. Should be used only internally. FIXME: Make private/protected
		virtual void FixContainerOffsets(); ///< Fixes container offsets. Should be used only internally.

		virtual void SetCustomData(void *param); ///< Allows the application programmer to store custom data assigned to this object.
		virtual void*GetCustomData(); ///< Allows the application programmer to retrieve previously stored custom data assigned to this object.

		virtual void SetFocusable(bool b) {focusable = b;} ///< Allows to set whether or not a widget can be focused
		virtual bool GetFocusable() {return focusable;} ///< Allows to retrieve whether or not a widget can be focused.

		void SetFont(std::string name, unsigned int size=10); ///< Sets the name of the font to be used for rendering of any caption that the widget may be attempting to paint. Also sets the size, which may be disregarded by the font engine.

		void SetCaptionColor(glictColor c); ///< Sets caption's text color, if supported by the widget.
		void SetCaptionColor(double r, double g, double b, double a=1.);

		float height, width; ///< Current height and width of the widget. FIXME: Make private/protected.
		float x, y; ///< Current position of the widget. FIXME: Make private/protected.
		float left, right, top, bottom; ///< Current boundaries of the widget, calculated from height, width, x and y. FIXME: Make private.
		float clipleft, clipright, cliptop, clipbottom; ///< Current clipping boundaries of the widget. Somewhat depends on the parent's clipping. FIXME: Make private.
		float containeroffsetx, containeroffsety; ///< Offsets of container object positions.
		char objtype[50]; ///< Short descriptive string containing name of the object. (Each class actually rewrites this one upon intialization in constructor.)

		virtual glictPos *GetVirtualPos() {return &virtualpos;}
		virtual float GetLeftSize() const {return containeroffsetx;} // FIXME While theoretically ok, this should be overriden in a few places.
		virtual float GetRightSize() const {return containeroffsetx;} // FIXME This is so wrong I can't even describe it. This needs to be overriden ... well, everywhere!
		virtual float GetTopSize() const {return containeroffsety;} // FIXME While theoretically ok, this should be overriden in a few places.
		virtual float GetBottomSize() const {return containeroffsety;} // FIXME This is so wrong I can't even describe it. This needs to be overriden ... well, everywhere!


		virtual void StartDraggingChild(glictContainer* draggedChild, const glictPos &relmousepos);
		virtual void StopDraggingChild(const glictPos &eventmousepos);
		inline void SetDraggable(bool value) { draggable = value; } // generic draggability


		// USE ONLY INTERNALLY:
		glictPos& _GetDragRelMouse() { return dragrelmouse; }

	private:
		// these should be called only internally
		void SetRect(float left, float top, float right, float bottom); ///< Internal function. Sets the boundaries of the widget.
		void SetClip(float left, float top, float right, float bottom); ///< Internal function. Sets the clipping boundaries of the widget.

		bool visible;
		bool enabled;

	protected:

		std::vector <glictContainer*> objects; ///< Contains all the children objects.
		/*int height, width;
		int x, y;
		int left, right, top, bottom;
		int clipleft, clipright, cliptop, clipbottom;*/

		glictContainer* parent; ///< Pointer to class' parents.
		unsigned int guid; ///< Unique identifier; unused so far
		float ModelviewMatrix[16]; ///< Modelview matrix, as remembered last time RememberTransformations() was called.

		void(*OnClick)(glictPos* relmousepos, glictContainer* callerclass); ///< Pointer to function specified as OnClick function.
		void(*OnMouseDown)(glictPos* relmousepos, glictContainer* callerclass); ///< Pointer to function specified as OnMouseDown function.
		void(*OnMouseUp)(glictPos* relmousepos, glictContainer* callerclass); ///< Pointer to function specified as OnMouseUp function.
        void(*OnPaint)(glictRect* real, glictRect* clipped, glictContainer* callerclass); ///< Pointer to function specified as OnPaint function.

		std::string caption; ///< Caption written on the control, if control supports it.
        glictColor captioncolor; ///< Caption's text color.

		bool focusable;
		std::vector <glictContainer*> delayedremove;

		glictSize virtualsize;
		glictPos virtualpos;

		glictContainer* next;
		glictContainer* previous;


		std::string fontname;
		unsigned int fontsize;

		void* customdata;



        glictContainer *draggedchild; ///< Dragged object's parent contains info on: which object is being dragged.
        glictPos dragrelmouse; ///< Dragged object contains info on: Relative position of mouse acquired upon mousepress
        bool dragging; ///< Dragged object contains info on: Is mouse currently down on the window? Is the object being currently dragged.
        bool draggable; ///< Can this object be dragged? This refers to the default event handler in DefaultCastEvent(). Window has separate dragging handling.



    /// \todo Remove this friend!

    friend void _glictMessageBox_Closer(glictPos* relmousepos, glictContainer* caller);

};

#endif
