#ifndef YATC_STACKPANEL_H_INCLUDED
#define YATC_STACKPANEL_H_INCLUDED

// Note: No matter what other statements in this file may say,
// this file is released under the same license as the latest
// version of GLICT (currently, GNU Lesser General Public License v2)

// It is not (currently) distributed as part of GLICT because UI
// element it produces is YATC-specific.

#include <GLICT/list.h>
class yatcStackPanel : public glictList
{
public:
    yatcStackPanel();
    ~yatcStackPanel();

    bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);
    void StartDraggingChild(glictContainer* draggedChild, const glictPos &relmousepos);
    void StopDraggingChild(const glictPos &eventmousepos);
    void Paint();
private:
    void _updateDraggedChildPos(const glictPos &eventmousepos);
};

#endif
