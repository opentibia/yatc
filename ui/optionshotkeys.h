#ifndef __UI_OPTIONSHOTKEYS_H
#define __UI_OPTIONSHOTKEYS_H

#include "util.h"
class winOptionsHotkeys_t {
public:


  glictWindow window;

  glictPanel lblHotkeys; // 18 33 268 43
  glictList lstHotkeys; // 18 47 268 152

  glictList lblText;   // 18 166 268 175
  glictTextbox txtText; // 18 180 268 195

  // 18 209 268 230

  glictPanel pnlSendAuto;
  glictPanel btnSendAuto;
  glictPanel lblSendAuto;

  // Hotkey Object
  glictPanel pnlSeparator;// 12 366 275 368


  //glictButton btnHelp;// 72 200, 40 17
  glictButton btnOk; // 178 376
  glictButton btnCancel; // 230 376


  ClientVersion_t currentversion;


  winOptionsHotkeys_t() {

    window.SetVisible(false);
    window.SetHeight(404);
    window.SetWidth(286);
    window.SetCaption("Hotkey Options");
    window.SetBGColor(.4, .4, .4, 1.);

    window.AddObject(&lblHotkeys);
    lblHotkeys.SetCaption("Available hotkeys:");
    lblHotkeys.SetPos(18, 33);
    lblHotkeys.SetWidth(268-18);
    lblHotkeys.SetHeight(43-33);
    lblHotkeys.SetBGActiveness(false);
    lblHotkeys.SetFont("aafont");

    window.AddObject(&lstHotkeys);
    lstHotkeys.SetPos(18, 47);
    lstHotkeys.SetWidth(268-18);
    lstHotkeys.SetHeight(152-47);
    lstHotkeys.SetBGColor(.2, .2, .2, 1.);
    lstHotkeys.SetFont("aafont");

    window.AddObject(&lblText);
    lblText.SetCaption("Edit hotkey text:");
    lblText.SetPos(18, 166);
    lblText.SetWidth(268-18);
    lblText.SetHeight(195-180);
    lblText.SetBGActiveness(false);
    lblText.SetFont("aafont");

    window.AddObject(&txtText);
    txtText.SetPos(18, 180);
    txtText.SetWidth(268-18);
    txtText.SetHeight(195-180);
    txtText.SetFont("aafont");



    // 18 209 268 230
    window.AddObject(&pnlSendAuto);
    pnlSendAuto.SetPos(18,209);
    pnlSendAuto.SetWidth(268-18);
    pnlSendAuto.SetHeight(230-209);
    pnlSendAuto.AddObject(&btnSendAuto);
    pnlSendAuto.AddObject(&lblSendAuto);
    pnlSendAuto.SetBGActiveness(false);
    btnSendAuto.SetPos(5, 5);
    btnSendAuto.SetWidth(12);
    btnSendAuto.SetHeight(12);
    btnSendAuto.SetOnClick(winOptionsHotkeys_t::OnCheckbox);
    lblSendAuto.SetPos(40-18,214-209);
    lblSendAuto.SetWidth(230);
    lblSendAuto.SetHeight(11);
    lblSendAuto.SetCaption("Send automatically");
    lblSendAuto.SetFont("aafont");
    lblSendAuto.SetBGActiveness(false);


    window.AddObject(&pnlSeparator);
    pnlSeparator.SetPos(12, 366);
    pnlSeparator.SetWidth(275-12);
    pnlSeparator.SetHeight(368-366);
    pnlSeparator.SetBGColor(.2,.2,.2,1.);

    /*
       window.AddObject(&btnHelp);
       btnHelp.SetPos(, 200);
       btnHelp.SetWidth(43);
       btnHelp.SetHeight(19);
       btnHelp.SetCaption("Help");
       btnHelp.SetFont("minifont",8);
       */

    window.AddObject(&btnOk);
    btnOk.SetPos(178, 376);
    btnOk.SetWidth(43);
    btnOk.SetHeight(19);
    btnOk.SetCaption("Ok");
    btnOk.SetFont("minifont",8);

    window.AddObject(&btnCancel);
    btnCancel.SetPos(230, 376);
    btnCancel.SetWidth(43);
    btnCancel.SetHeight(19);
    btnCancel.SetCaption("Cancel");
    btnCancel.SetFont("minifont",8);
  }

  void Init() {
		btnSendAuto.SetCustomData(/*TODO*/ 0 ? (void*)1 : NULL);
		btnSendAuto.SetSkin(/*TODO*/ true ? &g_skin.chk : &g_skin.txt);
  }

  void Store() {
  }

  static void OnCheckbox(glictPos* pos, glictContainer *caller) {
    if ((long)caller->GetCustomData() == 1) {
      caller->SetCustomData(NULL);
      ((glictPanel*)caller)->SetSkin(&g_skin.txt);
    } else {
      caller->SetCustomData((void*)1);
      ((glictPanel*)caller)->SetSkin(&g_skin.chk);
    }
  }
};

#endif

