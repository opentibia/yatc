//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// CLI version.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <fstream>
#include "../util.h"
#include "../objects.h"
#include "../options.h"
#include "../net/connection.h"

#if BAZEL_BUILD && WIN32
#include <SDL/SDL.h> // Give SDL a chance to rename main() for sake of SDL_main.
#endif

std::string g_recordfilename="debugrecord.rec";

unsigned int g_frameTime = 0;
unsigned int g_frameDiff = 0;
Connection *g_connection = NULL;

int main(int argc, char** argv) {
    auto i = Objects::getInstance();
    if (argc != 3) {
        fprintf(stderr, "usage: %s datfile jsonfile\n", argv[0]);
        return 1;
    }
    i->loadDat(argv[1]);
    
    std::ofstream outFile;
	outFile.open(argv[2]);
	i->asJSON(outFile);
	outFile.close();

    return 0;
}
