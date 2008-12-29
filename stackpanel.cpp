
// Note: No matter what other statements in this file may say,
// this file is released under the same license as the latest
// version of GLICT (currently, GNU Lesser General Public License v2)

// It is not (currently) distributed as part of GLICT because UI
// element it produces is YATC-specific.


#include "stackpanel.h"
#include <GLICT/globals.h>
#include <string.h>
#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif
yatcStackPanel::yatcStackPanel()
{
    strcpy(this->objtype, "yatcStackPanel");

    #if (GLICT_APIREV>=95)
        SetForcedHeight(0);
        defocusabilize_element = false;
    #else
        #warning yatcStackPanel needs GLICT APIREV 95+ to function properly.
    #endif
}

yatcStackPanel::~yatcStackPanel()
{
}

bool yatcStackPanel::CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue)
{
#if (GLICT_APIREV>=95)
    if (evt == GLICT_MOUSEMOVE) {
        //printf("dragged child of %s: %p\n", GetCaption().c_str(), draggedchild);
        if (draggedchild) {
            _updateDraggedChildPos((*(glictPos*)wparam));
            return true;
        }
    } else
    if (evt == GLICT_MOUSEUP) {
        if (draggedchild) {
            _updateDraggedChildPos((*(glictPos*)wparam));
            StopDraggingChild(*(glictPos*)wparam);
            return true;
        }
    }
#endif
    return glictList::CastEvent(evt,wparam,lparam,returnvalue);
}

void yatcStackPanel::StartDraggingChild(glictContainer* draggedChild, const glictPos &relmousepos) {
#if (GLICT_APIREV>=95)
    glictList::StartDraggingChild(draggedChild, relmousepos);
#endif
}

void yatcStackPanel::StopDraggingChild(const glictPos &eventmousepos) {
#if (GLICT_APIREV>=95)
    glictList::StopDraggingChild(eventmousepos);
    RebuildList();
#endif
}

void yatcStackPanel::_updateDraggedChildPos(const glictPos &eventmousepos)
{
#if (GLICT_APIREV>=95)

    int totaldcheight = draggedchild->GetHeight() +draggedchild->GetTopSize()+draggedchild->GetBottomSize();

    int oldy = draggedchild->GetY();
    int newy = MIN(MAX(eventmousepos.y - draggedchild->_GetDragRelMouse().y, 0), totalheight-totaldcheight);

    draggedchild->Focus(NULL);

    int currentheight=0;
    bool met = false;

    for (std::list<glictContainer*>::iterator it = listlist.begin() ; it != listlist.end() ; it++) {
        if (*it == draggedchild) {
            std::list<glictContainer*>::iterator oldit = it;
            it++;
            listlist.erase(oldit);
            if (it == listlist.end()) break;
        }
        if (*it != draggedchild) {

            int totalitheight = (*it)->GetHeight() +(*it)->GetTopSize()+(*it)->GetBottomSize();

            if (currentheight + totalitheight /2 > newy && !met) {
                currentheight += totaldcheight;
                met = true;
                listlist.insert(it, draggedchild);// inserts dragged child before "it"
            }

            (*it)->SetPos(0, currentheight);
            if (forcedheight)
                (*it)->SetHeight(forcedheight);
            (*it)->SetWidth(width - GetCurrentVScrollbarWidth() - (*it)->GetLeftSize() - (*it)->GetRightSize());
            currentheight += totalitheight;
        }

    }
    if (!met)
        listlist.push_back(draggedchild);

    draggedchild->SetPos(
        draggedchild->GetX(),
        newy
    );
#endif
}

void yatcStackPanel::Paint()
{
#if (GLICT_APIREV>=95)
    glictGlobals.PaintRect(clipleft, clipright, cliptop, MIN(clipbottom, cliptop + totalheight), glictColor(0,0,0,1));
#endif
    glictList::Paint();
}
