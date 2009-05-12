#ifndef YATC_STACKPANEL_H_INCLUDED
#define YATC_STACKPANEL_H_INCLUDED

// Note: No matter what other statements in this file may say,
// this file is released under the same license as the latest
// version of GLICT (currently, GNU Lesser General Public License v2)

// It is not (currently) distributed as part of GLICT because UI
// element it produces is YATC-specific.

#include <GLICT/list.h>
#include <GLICT/window.h>
#include <GLICT/button.h>

class yatcStackPanel : public glictList
{
public:
    yatcStackPanel();
    ~yatcStackPanel();

    bool CastEvent(glictEvents evt, void* wparam, long lparam, void* returnvalue);
    void StartDraggingChild(glictContainer* draggedChild, const glictPos &relmousepos);
    void StopDraggingChild(const glictPos &eventmousepos);
    void Paint();
    int GetTotalHeight() { return totalheight; }
private:
    void _updateDraggedChildPos(const glictPos &eventmousepos);
};


// note (nfries88): leaving this note here in case I don't manage
//					to add resizable windows to GLICT before my next
//					commit (seems likely I won't)
//	OnResized(glictSize*, glictContainer*) is a callback given to glict
//					and called when a glictContainer is resized.
//	OnResize(float w) allows a yatcStackPanel to determine its own behavior
//					after resizing
//					example: a container window in CipSoft's client always
//						displays one full row so this would allow a container
//						window to resize itself if necessary.
class yatcStackPanelWindow
{
public:
	yatcStackPanelWindow();
    virtual ~yatcStackPanelWindow() {}

	static void OnClose(glictPos* pos, glictContainer *caller);
	static void OnCollapse(glictPos* pos, glictContainer *caller);
	static void OnExpand(glictPos* pos, glictContainer *caller);
	static void OnResized(glictSize* size, glictContainer* caller);

	virtual float GetDefaultHeight(){ return 0.F; };
	virtual void OnClose(){ };
	virtual void OnResize(float h){ };

	glictWindow window;
	glictPanel winpanel;

    glictButton closebtn;
    glictButton btnCollapse;
    //glictPanel pnlIcon;
};

#endif
