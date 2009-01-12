//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
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
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include "automap.h"
#include "sprite.h"
#include "engine.h"
#include "util.h"

Automap::Automap()
{
    map[0] = map[1] = map[2] = map[3] = NULL;
    mapcount = 0;
    mapw = 256;
    maph = 256;
    m_tileCount=0;
}
Automap::~Automap()
{
}

void Automap::updateSelf()
{
    int xs = px - mapw/2;
    int ys = py - maph/2;
    int xe = px + mapw/2;
    int ye = py + maph/2;

    int oldmapcount = mapcount;
    mapcount = 0;
    printf("Updating automap\n");
    for (int j = ys; j<ye; j+=maph-1)
        for (int i = xs; i<xe; i+=mapw-1)
        {


            std::stringstream x,y,z,minimapfnss;
            x << setw(3) << setfill('0') << i / 256;
            y << setw(3) << setfill('0') << j / 256;
            z << setw(2) << setfill('0') << pz;
            minimapfnss << x.str() << y.str() << z.str() << ".map";

            bool has_map = false;
            if (mapcount < oldmapcount && minimapfnss.str() != mapfns[mapcount]) {
                delete map[mapcount];
                map[mapcount] = NULL;
                mapfns[mapcount] = "";
            } else {
                has_map = true;
            }
            mapcount++;
            Sprite* activeMap = NULL;
            if (!has_map)
            {
                mapfns[mapcount-1] = minimapfnss.str();
                map[mapcount-1] = activeMap = g_engine->createSprite(minimapfnss.str());
                if (!map[mapcount-1]->isLoaded()) {
                    printf("Trying to spawn map by flushtiling\n");
                    delete map[mapcount-1];
                    flushTiles();
                    map[mapcount-1] = activeMap = g_engine->createSprite(minimapfnss.str());
                    printf("Loaded: %s\n", activeMap->isLoaded() ? "yes" : "no");
                }
            } else {
                activeMap = map[mapcount-1];
            }


            writeFilesMap::iterator mit;
            // We NEED to update sprites! We flush only every n tiles!
            if (( mit = writeFiles.find(minimapfnss.str())) != writeFiles.end())
            {
                if(activeMap && activeMap->isLoaded()) {
                    printf("locking\n");
                    SDL_Surface* s=activeMap->lockSurface();
                    printf("locked for %d pixels\n", mit->second.size());
                    for(std::vector<posAndColor>::iterator it = mit->second.begin(); it != mit->second.end(); it++)
                    {
                        //printf("it %p\n", it);
                        uint8_t c = it->color;
                        uint8_t b = uint8_t((c % 6) / 5. * 255);
                        uint8_t g = uint8_t(((c / 6) % 6) / 5. * 255);
                        uint8_t r = uint8_t((c / 36.) / 6. * 255);

                        activeMap->putPixel(it->x % 256,it->y % 256,SDL_MapRGB(s->format,r,g,b),s);

                    }
                    printf("unlocked\n");
                    activeMap->unlockSurface();
                } else {
                    printf("Found but not updated since it's unloadable\n");
                    delete activeMap;
                    flushTiles();
                    map[mapcount-1] = activeMap = g_engine->createSprite(minimapfnss.str());
                    printf("Retried. If this fails .... *sigh*\n");
                }


            }
        }
}

void Automap::setPos(int x, int y, int z)
{
    px = x;
    py = y;
    pz = z;
    updateSelf();
}

void Automap::renderSelf(int x, int y, int w, int h) // parameters specify where on the screen it should be painted
{
    // FIXME (ivucica#1#): fix use of w and h; currently they need to be default values 256x256
    int xs = px - w/2;
    int ys = py - h/2;
    int xe = px + w/2;
    int ye = py + h/2;


    if (map[0]) map[0]->Blit(x,y, xs%256, ys%256, 256-(xs%256), 256-(ys%256));
    if (map[1]) map[1]->Blit(x+256-(xs%256),y, 0, ys%256, w-(256-(xs%256)), 256-(ys%256));
    if (map[2]) map[2]->Blit(x,y+256-(ys%256), xs%256, 0, 256-(xs%256), h-(256-(ys%256)));
    if (map[3]) map[3]->Blit(x+256-(xs%256),y+256-(ys%256), 0, 0, w-(256-(xs%256)), h-(256-(ys%256)));

    g_engine->drawRectangle(x+w/2,y+h/2,3,3, oRGBA(1,1,1,1));
}

void Automap::setTileColor(int i, int j, int k, uint8_t color, uint8_t speedindex)
{
    std::stringstream x,y,z,minimapfnss;
    x << setw(3) << setfill('0') << i / 256;
    y << setw(3) << setfill('0') << j / 256;
    z << setw(2) << setfill('0') << k;
    minimapfnss << x.str() << y.str() << z.str() << ".map";

    posAndColor p;
    p.x = i; p.y = j; p.z = k; p.color = color;

    writeFiles[minimapfnss.str()].push_back(p);

    m_tileCount++;
}

void Automap::flushTiles()
{
    writeFilesMap::iterator it;
    for(it=writeFiles.begin();it!=writeFiles.end();it++)
    {
        FILE* f=NULL;
        if (!fileexists(it->first.c_str()))
        {
            f = yatc_fopen(it->first.c_str(),"wb+");
            if (!f) {
		printf("Could not create %s\n", it->first.c_str());
                continue;
            }
            // let's put an integer, 0, to the end of the file ...
            // this should denote how many map marks there are on the map ...
            uint32_t zero=0;
            fseek(f, 256*256*2, SEEK_SET);
            fwrite(&zero, 4,1,f);
        }
        if (!f) f = yatc_fopen(it->first.c_str(),"rb+");
        if (!f) {
            printf("Could not open %s\n", it->first.c_str());
            continue;
        }
        //fseek(f, 256*256 *2, SEEK_SET);
        for (std::vector<posAndColor>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            fseek(f, (it2->x%256) * 256 + (it2->y%256), SEEK_SET);
            fwrite(&it2->color, 1, 1, f);
            fseek(f, 256*256 + (it2->x%256) * 256 + (it2->y%256), SEEK_SET);
            fwrite(&it2->speedindex, 1, 1, f);
        }
        fclose(f);
    }
    m_tileCount = 0;
    writeFiles.clear();
}

void Automap::getTileColor(int i, int j, int k, uint8_t &color, uint8_t &speedindex)
{
    std::stringstream x,y,z,minimapfnss;

    x << setw(3) << setfill('0') << i / 256;
    y << setw(3) << setfill('0') << j / 256;
    z << setw(2) << setfill('0') << k;
    minimapfnss << x.str() << y.str() << z.str() << ".map";
    FILE *f = yatc_fopen(minimapfnss.str().c_str(),"rb");
    if(!f)
    {
        color = 0;
        speedindex = 255;
        return;
    }

    fseek(f, (i%256) * 256 + (j%256), SEEK_SET);
    fread(&color, 1, 1, f);
    fseek(f, 256*256 + (i%256) * 256 + (j%256), SEEK_SET);
    fread(&speedindex, 1, 1, f);

    fclose(f);
    return;

}
