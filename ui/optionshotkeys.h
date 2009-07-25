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

#ifndef __UI_OPTIONSHOTKEYS_H
#define __UI_OPTIONSHOTKEYS_H

#include <GLICT/textbox.h>
#include "../util.h"
#include "../options.h"
#include "checkbox.h"
#include "../gamecontent/item.h"
#if defined(HAVE_CONFIG_H)
    #include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

class winOptionsHotkeys_t {
public:
	glictWindow window;

	glictPanel lblHotkeys; // 18 33 268 43
	glictList lstHotkeys; // 18 47 268 152

	glictPanel lsiHotkeys[36];

	glictList lblText;   // 18 166 268 175
	glictTextbox txtText; // 18 180 268 195

	// 18 209 268 230
	uiCheckbox chkSendAuto;

	// Item Hotkeys
	glictPanel lblObjText;
	glictPanel pnlObjView;
	glictButton btnSelectObj;
	glictButton btnClearObj;
	glictButton btnUseSelf;
	glictButton btnUseTarget;
	glictButton btnUseCrosshair;

	// Hotkey Object
	glictPanel pnlSeparator;// 12 366 275 368


	//glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 178 376
	glictButton btnCancel; // 230 376


	ClientVersion_t currentversion;

	int currenthotkey;

	Item* item;

	winOptionsHotkeys_t() {
		currenthotkey = -1;
		item = NULL;

		window.SetVisible(false);
		window.SetHeight(389);
		window.SetWidth(286);
		window.SetCaption(gettext("Hotkey Options"));
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&lblHotkeys);
		lblHotkeys.SetCaption(gettext("Available hotkeys:"));
		lblHotkeys.SetPos(18, 18);
		lblHotkeys.SetWidth(250);
		lblHotkeys.SetHeight(15);
		lblHotkeys.SetBGActiveness(false);
		lblHotkeys.SetFont("aafont");

		window.AddObject(&lstHotkeys);
		lstHotkeys.SetPos(18, 32);
		lstHotkeys.SetWidth(250);
		lstHotkeys.SetHeight(105);
		lstHotkeys.SetBGColor(.2, .2, .2, 1.);
		lstHotkeys.SetFocusable(true);

		window.AddObject(&lblText);
		lblText.SetCaption(gettext("Edit hotkey text:"));
		lblText.SetPos(18, 150);
		lblText.SetWidth(250);
		lblText.SetHeight(15);
		lblText.SetBGActiveness(false);
		lblText.SetFont("aafont");

		window.AddObject(&txtText);
		txtText.SetPos(18, 165);
		txtText.SetWidth(250);
		txtText.SetHeight(15);
		txtText.SetFont("aafont");

		// 18 209 268 230
		window.AddObject(&chkSendAuto.pnlPanel);
		chkSendAuto.SetPos(18,194);
		chkSendAuto.SetWidth(250);
		chkSendAuto.SetHeight(21);
		chkSendAuto.SetCaption(gettext("Send automatically"));
		chkSendAuto.lblLabel.SetCaptionColor(glictColor(.5f, .5f, .5f, .5f));

		// Item Hotkeys
		window.AddObject(&lblObjText);
		lblObjText.SetPos(18, 229);
		lblObjText.SetHeight(15);
		lblObjText.SetWidth(120);
		lblObjText.SetCaption(gettext("Edit Hotkey Object: "));
		lblObjText.SetBGActiveness(false);
		lblObjText.SetFont("aafont");
		// Display Items Here
		window.AddObject(&pnlObjView);
		pnlObjView.SetPos(18, 243);
		pnlObjView.SetHeight(53);
		pnlObjView.SetWidth(53);
		pnlObjView.SetCustomData(this);
		pnlObjView.SetOnPaint(&winOptionsHotkeys_t::drawObject);
		//pnlObjView.SetBGActiveness(false);
		pnlObjView.SetBGColor(.1,.1,.1,1);
		pnlObjView.SetSkin(&g_skin.txt);

		window.AddObject(&btnSelectObj);
		btnSelectObj.SetPos(82, 243);
		btnSelectObj.SetHeight(20);
		btnSelectObj.SetWidth(86);
		btnSelectObj.SetCaption(gettext("Select Object"));
		btnSelectObj.SetOnClick(&winOptionsHotkeys_t::onSelectObj);
		btnSelectObj.SetFont("minifont", 8);
		btnSelectObj.SetCustomData(this);

		window.AddObject(&btnClearObj);
		btnClearObj.SetPos(82, 275);
		btnClearObj.SetHeight(20);
		btnClearObj.SetWidth(86);
		btnClearObj.SetCaption(gettext("Clear Object"));
		btnClearObj.SetOnClick(&winOptionsHotkeys_t::onClearObj);
		btnClearObj.SetFont("minifont", 8);
		btnClearObj.SetCustomData(this);

		window.AddObject(&btnUseSelf);
		btnUseSelf.SetPos(181, 243);
		btnUseSelf.SetHeight(20);
		btnUseSelf.SetWidth(86);
		btnUseSelf.SetCaption(gettext("Use on yourself"));
		btnUseSelf.SetOnClick(&winOptionsHotkeys_t::onObjUse_Self);
		btnUseSelf.SetFont("minifont", 8);
		btnUseSelf.SetCustomData(this);

		window.AddObject(&btnUseTarget);
		btnUseTarget.SetPos(181, 275);
		btnUseTarget.SetHeight(20);
		btnUseTarget.SetWidth(86);
		btnUseTarget.SetCaption(gettext("Use on target"));
		btnUseTarget.SetOnClick(&winOptionsHotkeys_t::onObjUse_Target);
		btnUseTarget.SetFont("minifont", 8);
		btnUseTarget.SetCustomData(this);

		window.AddObject(&btnUseCrosshair);
		btnUseCrosshair.SetPos(181, 309);
		btnUseCrosshair.SetHeight(20);
		btnUseCrosshair.SetWidth(86);
		btnUseCrosshair.SetCaption(gettext("Use on crosshair"));
		btnUseCrosshair.SetOnClick(&winOptionsHotkeys_t::onObjUse_Crosshair);
		btnUseCrosshair.SetFont("minifont", 8);
		btnUseCrosshair.SetCustomData(this);

		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(12, 351);
		pnlSeparator.SetWidth(275-12);
		pnlSeparator.SetHeight(368-366);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);
		pnlSeparator.SetSkin(&g_skin.chk);

		window.AddObject(&btnOk);
		btnOk.SetPos(178, 361);
		btnOk.SetWidth(43);
		btnOk.SetHeight(19);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(230, 361);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(19);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetFont("minifont",8);

		for(int i = 0; i != 36; ++i)
		{
			lsiHotkeys[i].SetBGColor(.4,.4,.4,1.);
			lsiHotkeys[i].SetBGActiveness(false);
			lsiHotkeys[i].SetOnClick(winOptionsHotkeys_t::onHotkey);
			lsiHotkeys[i].SetCustomData(this);
			lsiHotkeys[i].SetFont("aafont");
			lsiHotkeys[i].SetFocusable(false);
			lstHotkeys.AddObject(&lsiHotkeys[i]);
			#if GLICT_APIREV < 68
			#warning Aesthetic corrections on listboxes wont work without GLICT of apirev 68+
			#else
			lsiHotkeys[i].SetTextOffset(4,2);
			#endif
		}
	}

	void Init() {
		currenthotkey = -1;
		item = NULL;
		chkSendAuto.SetValue(0);

		// set caption
		for(int i = 0; i != 36; ++i)
		{
			CreateHotkey(i);
		}
	}

	void SaveHotkeyText(int hkid)
	{
		if(hkid >= 0 && hkid < 36)
		{
			std::string text = txtText.GetCaption();
			if(text.length())
			{
				options.hotkeys[hkid].isText = true;
				options.hotkeys[hkid].text = text;
				options.hotkeys[hkid].sendAuto = chkSendAuto.GetValue();
			}
			else if(options.hotkeys[hkid].isText)
			{
				options.hotkeys[hkid].text = "";
			}
			CreateHotkey(hkid);
		}
	}

	void CreateHotkey(int hkid)
	{
		std::stringstream ss;
		ss << hotkeystrs[hkid] << ": ";
		if(options.hotkeys[hkid].isText)
		{
			lsiHotkeys[hkid].SetCaptionColor(1.f, 1.f, 1.f);
			ss << options.hotkeys[hkid].text;
		}
		else if(options.hotkeys[hkid].item.useOnSelf)
		{
			lsiHotkeys[hkid].SetCaptionColor(0.0f, 0.95f, 0.4f);
			ss << gettext("(use object on yourself)");
		}
		else if(options.hotkeys[hkid].item.useOnTarget)
		{
			lsiHotkeys[hkid].SetCaptionColor(0.1f, 0.2f, 0.9f);
			ss << gettext("(use object on target)");
		}
		else if(options.hotkeys[hkid].item.useXHairs)
		{
			lsiHotkeys[hkid].SetCaptionColor(0.2f, 0.2f, 0.7f);
			ss << gettext("(use object with crosshairs)");
		}
		lsiHotkeys[hkid].SetCaption(ss.str());
	}

	void Store() {
		if(currenthotkey >= 0)
		{
			SaveHotkeyText(currenthotkey);
			lsiHotkeys[currenthotkey].SetBGActiveness(false);
			txtText.SetCaption("");
			chkSendAuto.SetValue(false);
			chkSendAuto.lblLabel.SetCaptionColor(glictColor(.5f, .5f, .5f, .5f));
			btnUseCrosshair.SetHold(false);
			btnUseSelf.SetHold(false);
			btnUseTarget.SetHold(false);
			if(item) {
				delete item;
			}
		}
		currenthotkey = -1;
		options.Save();
	}

	void UpdateHotkey()
	{
		Hotkey& hotkey = options.hotkeys[currenthotkey];

		chkSendAuto.SetValue(false);

		btnUseSelf.SetHold(false);
		btnUseTarget.SetHold(false);
		btnUseCrosshair.SetHold(false);
		if(!hotkey.isText)
		{
			hotkey.text = "";

			item = Item::CreateItem(hotkey.item.itemid, hotkey.item.type);
			txtText.SetCaption("");

			if(hotkey.item.useOnSelf)
			{
				btnUseSelf.SetHold(true);
			}
			if(hotkey.item.useOnTarget)
			{
				btnUseTarget.SetHold(true);
			}
			if(hotkey.item.useXHairs)
			{
				btnUseCrosshair.SetHold(true);
			}

			chkSendAuto.SetEnabled(false);
		}
		else
		{
			if(!hotkey.text.empty()) {
				txtText.SetCaption(hotkey.text);
				chkSendAuto.SetValue(hotkey.sendAuto);
				chkSendAuto.SetEnabled(true);
			}
			else
			{
				txtText.SetCaption("");
				chkSendAuto.SetValue(false);
				chkSendAuto.SetEnabled(false);
			}
			hotkey.item.itemid = 0;
		}
	}

	static void onHotkey(glictPos* pos, glictContainer *caller)
	{
		winOptionsHotkeys_t* winHK = (winOptionsHotkeys_t*)(caller->GetCustomData());
		// ensure last selected hotkey is as expected
		if(winHK->currenthotkey >= 0 && winHK->currenthotkey < 36) {
			winHK->SaveHotkeyText(winHK->currenthotkey);
			winHK->lsiHotkeys[winHK->currenthotkey].SetBGActiveness(false);
			if(winHK->item != NULL){
				delete winHK->item;
				winHK->item = NULL;
			}
		}

		// set hotkey data to this hotkey
		winHK->currenthotkey = (glictPanel*)caller - winHK->lsiHotkeys;
		winHK->UpdateHotkey();
		((glictPanel*)caller)->SetBGActiveness(true);
	}

	static void drawObject(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void onSelectObj(glictPos* pos, glictContainer *caller);
	static void onClearObj(glictPos* pos, glictContainer *caller);
	static void onObjUse_Self(glictPos* pos, glictContainer *caller);
	static void onObjUse_Target(glictPos* pos, glictContainer *caller);
	static void onObjUse_Crosshair(glictPos* pos, glictContainer *caller);
};

#endif

