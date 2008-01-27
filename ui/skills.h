#ifndef __SKILLS_H
#define __SKILLS_H

#include <GLICT/window.h>
#include <GLICT/progressbar.h>
class winSkills_t {
public:
	glictWindow window;
	glictProgressBar pbExperience;
	winSkills_t() {
		window.AddObject(pbExperience);
	}

};
#endif
