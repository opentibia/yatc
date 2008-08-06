#ifndef __POPUP_H
#define __POPUP_H

// A few words about this class.
//
// We need a generic menu popup. The idea is pretty simple and
// similar to what we do with regards to resolutions in
// graphic options.
//
// We'll have a glictList which we'll populate with menu options.
// Each menu option will be a glictPanel. We'll handle mouseclick
// and mousemove in both glictList and glictPanel. Upon mouseclick
// we'll call the callback specified when the calling GameMode was
// creating the popup.
//
// On the other hand, mousemove will be used for producing rollover
// effect. It will be handled on listbox too, for unselecting
// all menu items. Unselection will also be produced by moving the
// mouse out of the popup; this will be caused by the GameMode.
//
// So, let's get crackin', then :)

class Popup {
public:
    Popup() {}
    ~Popup() {}
};

#endif
