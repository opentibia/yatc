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


    mapcount = 0;

    for (int i = 0; i < 4; i++){
        delete map[i];
        map[i] = NULL;
        mapfns[i] = "";
    }

    for (int j = ys; j<ye; j+=maph-1)
        for (int i = xs; i<xe; i+=mapw-1)
        {

            std::stringstream x,y,z,minimapfnss;
            x << setw(3) << setfill('0') << i / 256;
            y << setw(3) << setfill('0') << j / 256;
            z << setw(2) << setfill('0') << pz;
            minimapfnss << x.str() << y.str() << z.str() << ".map";

            bool has_map = false;
            /*for(int k=0; k<gd->mapcount; k++)
            {
                if(gd->mapfns[i] == minimapfnss.str())
                {
                    has_map = true;
                    break;
                }
            }*/

            if (!has_map)
            {
                mapcount++;
                mapfns[mapcount-1] = minimapfnss.str();
                map[mapcount-1] = g_engine->createSprite(minimapfnss.str());


/*
// We NEED to update sprites! We flush only every n tiles!
                if ((std::map<std::string, std::vector<posAndColor> >::iterator mit = writeFiles.find(minimapfnss.str())) != writeFiles.end())
                {
                    for(std::vector<posAndColor> it = mit->second.begin(); it != mit->second.end(); it++)
                    {
                        if (it->x == i && it->y == j) {

                        }
                    }
                }
*/


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
