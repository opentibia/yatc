#ifndef __GLICT_IMAGE_H
#define __GLICT_IMAGE_H

/** \brief Purely virtual base class for representing an image
  *
  * Application programmer should derive a class from this one
  * in order to make it represent an image which can be used by
  * glictSkinner.
  *
  * This was introduced to make it easier to use GLICT skins
  * on a non-OpenGL rendering platform.
  */

class glictImage {
	public:
		glictImage() {}
		virtual ~glictImage() {}

		virtual void Paint(float destx, float desty, float w, float h) = 0;
		virtual float GetW() = 0;
		virtual float GetH() = 0;
		glictSize GetSize() {glictSize s; s.w = GetW(); s.h = GetH(); return s;}
};

#endif
