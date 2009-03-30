#ifndef __GLICT_PROGRESSBAR_H
#define __GLICT_PROGRESSBAR_H
#include <GLICT/container.h>

class glictProgressBar : public glictContainer {
	public:
		glictProgressBar();
		~glictProgressBar();

		void Paint();

		void SetValue(float newpcnt);
		void AddValue(float pcnt);
		float GetValue();

	private:
		float percent;
};

#endif
