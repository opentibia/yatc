#include "popup.h"
Popup::Popup() {
    wantdeath = false;
}
Popup::~Popup() {
    for (std::vector<PopupItem*>::iterator it = items.begin(); it != items.end(); it++) {
        delete *it;
    }
}

void Popup::addItem(const std::string &txt, popupCallback_t cb) {
    PopupItem* pi = new PopupItem;
    pi->pnl.SetCustomData(pi);
    pi->txt = txt;
    pi->cb = cb;
    pi->pnl.SetCaption(txt);
    pi->pnl.SetBGActiveness(false);
    pi->pnl.SetHeight(12);
    pi->pnl.SetWidth(150);
    pi->pnl.SetOnClick(onClick);
    pi->pnl.SetFont("aafont");
    pi->pnl.SetFocusable(false);
    pi->pnl.SetBGColor(.4, .4, .4, 1.);
    pi->parent = this;
    items.push_back(pi);
    list.AddObject(&pi->pnl);

    list.SetHeight(12*items.size()+4);
    list.SetWidth(150);
    list.SetBGColor(.1, .1, .1,1);



}

void Popup::onClick(glictPos* pos, glictContainer *caller) {
    PopupItem* pi = (PopupItem*)caller->GetCustomData();
    if (pi->cb)
        pi->cb();
    pi->parent->prepareToDie();
}

void Popup::mouseOver(float x, float y) {
    for (std::vector<PopupItem*>::iterator it = items.begin(); it != items.end(); it++) {
        PopupItem* pi = *it;

        pi->pnl.SetBGActiveness(false);
    }
    if (cursorInside(x,y)) {
        y -= list.GetY();
        y /= 12;
        if (y < items.size())
            items[y]->pnl.SetBGActiveness(true);
        else
            printf("Warning: popup trid to highligh nonexisting listitem\n");
    }

}

bool Popup::cursorInside(float x, float y) {
    return (x >= list.GetX() && x < list.GetX() + list.GetWidth() && y >= list.GetY() && y < list.GetY() + list.GetHeight());

}
