#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <string>
#include "database.h"

enum enginelist_t {
    ENGINE_SDL = 0, // software renderer
    ENGINE_OPENGL,
    ENGINE_DIRECTX
};

class Options {
    public:
        Options();
        ~Options();

        void Save();
        void Load();

        bool maptrack;
        bool fullscreen;
		bool os_cursor;
		std::string skin;

		enginelist_t engine;

};


extern Options options;
#endif
