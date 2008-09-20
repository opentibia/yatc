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

#ifndef __UI_OPTIONSCONSOLE_H
#define __UI_OPTIONSCONSOLE_H

#include "../skin.h"

class winOptionsConsole_t {
public:


	glictWindow window;

	// 14 15, 261 22
	glictPanel pnlInfoMsgs;
	glictPanel btnInfoMsgs;
	glictPanel lblInfoMsgs;

	// 14 50, 261 22
	glictPanel pnlEventMsgs;
	glictPanel btnEventMsgs;
	glictPanel lblEventMsgs;

	// 14 85
	glictPanel pnlStatusMsgs;
	glictPanel btnStatusMsgs;
	glictPanel lblStatusMsgs;

	// 14 120
	glictPanel pnlTimestamps;
	glictPanel btnTimestamps;
	glictPanel lblTimestamps;

	// 14 155
	glictPanel pnlLevels;
	glictPanel btnLevels;
	glictPanel lblLevels;

    // 14 190
    glictPanel pnlPrivateMsgs;
	glictPanel btnPrivateMsgs;
	glictPanel lblPrivateMsgs;


	// 9 232, 270 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 131 244, 40 17
	glictButton btnOk; // 185 244
	glictButton btnCancel; // 237 244
	winOptionsConsole_t () {

		window.SetVisible(false);
		window.SetHeight(276);
		window.SetWidth(296);
		window.SetCaption("Console Options");
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&pnlInfoMsgs);
		pnlInfoMsgs.SetPos(14,15);
		pnlInfoMsgs.SetWidth(261);
		pnlInfoMsgs.SetHeight(22);
		pnlInfoMsgs.AddObject(&btnInfoMsgs);
		pnlInfoMsgs.AddObject(&lblInfoMsgs);
		pnlInfoMsgs.SetBGActiveness(false);
		btnInfoMsgs.SetWidth(12);
		btnInfoMsgs.SetHeight(12);
		btnInfoMsgs.SetPos(5, 5);
		btnInfoMsgs.SetOnClick(winOptionsConsole_t::OnCheckbox);
		lblInfoMsgs.SetWidth(230);
		lblInfoMsgs.SetHeight(11);
		lblInfoMsgs.SetPos(22,7);
		lblInfoMsgs.SetCaption("Show Info Messages in Console");
		lblInfoMsgs.SetFont("aafont");
		lblInfoMsgs.SetBGActiveness(false);

		window.AddObject(&pnlEventMsgs);
		pnlEventMsgs.SetPos(14, 50);
		pnlEventMsgs.SetWidth(261);
		pnlEventMsgs.SetHeight(22);
		pnlEventMsgs.AddObject(&btnEventMsgs);
		pnlEventMsgs.AddObject(&lblEventMsgs);
		pnlEventMsgs.SetBGActiveness(false);
		btnEventMsgs.SetWidth(12);
		btnEventMsgs.SetHeight(12);
		btnEventMsgs.SetPos(5, 5);
		btnEventMsgs.SetOnClick(winOptionsConsole_t::OnCheckbox);
		lblEventMsgs.SetWidth(230);
		lblEventMsgs.SetHeight(11);
		lblEventMsgs.SetPos(22,7);
		lblEventMsgs.SetCaption("Show Event Messages in Console");
		lblEventMsgs.SetFont("aafont");
		lblEventMsgs.SetBGActiveness(false);

		window.AddObject(&pnlStatusMsgs);
		pnlStatusMsgs.SetPos(14, 85);
		pnlStatusMsgs.SetWidth(261);
		pnlStatusMsgs.SetHeight(22);
		pnlStatusMsgs.AddObject(&btnStatusMsgs);
		pnlStatusMsgs.AddObject(&lblStatusMsgs);
		pnlStatusMsgs.SetBGActiveness(false);
		btnStatusMsgs.SetWidth(12);
		btnStatusMsgs.SetHeight(12);
		btnStatusMsgs.SetPos(5, 5);
		btnStatusMsgs.SetOnClick(winOptionsConsole_t::OnCheckbox);
		lblStatusMsgs.SetWidth(230);
		lblStatusMsgs.SetHeight(11);
		lblStatusMsgs.SetPos(22,7);
		lblStatusMsgs.SetCaption("Show Status Messages in Console");
		lblStatusMsgs.SetFont("aafont");
		lblStatusMsgs.SetBGActiveness(false);

		window.AddObject(&pnlTimestamps);
		pnlTimestamps.SetPos(14, 120);
		pnlTimestamps.SetWidth(261);
		pnlTimestamps.SetHeight(22);
		pnlTimestamps.AddObject(&btnTimestamps);
		pnlTimestamps.AddObject(&lblTimestamps);
		pnlTimestamps.SetBGActiveness(false);
		btnTimestamps.SetWidth(12);
		btnTimestamps.SetHeight(12);
		btnTimestamps.SetPos(5, 5);
		btnTimestamps.SetOnClick(winOptionsConsole_t::OnCheckbox);
		lblTimestamps.SetWidth(230);
		lblTimestamps.SetHeight(11);
		lblTimestamps.SetPos(22,7);
		lblTimestamps.SetCaption("Show Timestamps in Console");
		lblTimestamps.SetFont("aafont");
		lblTimestamps.SetBGActiveness(false);

		window.AddObject(&pnlLevels);
		pnlLevels.SetPos(14, 155);
		pnlLevels.SetWidth(261);
		pnlLevels.SetHeight(22);
		pnlLevels.AddObject(&btnLevels);
		pnlLevels.AddObject(&lblLevels);
		pnlLevels.SetBGActiveness(false);
		btnLevels.SetWidth(12);
		btnLevels.SetHeight(12);
		btnLevels.SetPos(5, 5);
		btnLevels.SetOnClick(winOptionsConsole_t::OnCheckbox);
		lblLevels.SetWidth(230);
		lblLevels.SetHeight(11);
		lblLevels.SetPos(22,7);
		lblLevels.SetCaption("Show Levels in Console");
		lblLevels.SetFont("aafont");
		lblLevels.SetBGActiveness(false);

		window.AddObject(&pnlPrivateMsgs);
		pnlPrivateMsgs.SetPos(14, 190);
		pnlPrivateMsgs.SetWidth(261);
		pnlPrivateMsgs.SetHeight(22);
		pnlPrivateMsgs.AddObject(&btnPrivateMsgs);
		pnlPrivateMsgs.AddObject(&lblPrivateMsgs);
		pnlPrivateMsgs.SetBGActiveness(false);
		btnPrivateMsgs.SetWidth(12);
		btnPrivateMsgs.SetHeight(12);
		btnPrivateMsgs.SetPos(5, 5);
		btnPrivateMsgs.SetOnClick(winOptionsConsole_t::OnCheckbox);
		lblPrivateMsgs.SetWidth(230);
		lblPrivateMsgs.SetHeight(11);
		lblPrivateMsgs.SetPos(22,7);
		lblPrivateMsgs.SetCaption("Show Private Messages in Game Window");
		lblPrivateMsgs.SetFont("aafont");
		lblPrivateMsgs.SetBGActiveness(false);

		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(9, 232);
		pnlSeparator.SetWidth(270);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(131, 244);
		btnHelp.SetWidth(41);
		btnHelp.SetHeight(17);
		btnHelp.SetCaption("Help");
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(185, 244);
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption("Ok");
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(237, 244);
		btnCancel.SetWidth(41);
		btnCancel.SetHeight(17);
		btnCancel.SetCaption("Cancel");
		btnCancel.SetFont("minifont",8);
	}


	void Init() {
		btnInfoMsgs.SetCustomData(options.infomsgs ? (void*)1 : NULL);
		btnInfoMsgs.SetSkin(options.infomsgs ? &g_skin.chk : &g_skin.txt);
		btnEventMsgs.SetCustomData(options.eventmsgs? (void*)1 : NULL);
		btnEventMsgs.SetSkin(options.eventmsgs ? &g_skin.chk : &g_skin.txt);
		btnStatusMsgs.SetCustomData(options.statusmsgs ? (void*)1 : NULL);
		btnStatusMsgs.SetSkin(options.statusmsgs ? &g_skin.chk : &g_skin.txt);
		btnTimestamps.SetCustomData(options.timestamps? (void*)1 : NULL);
		btnTimestamps.SetSkin(options.timestamps ? &g_skin.chk : &g_skin.txt);
		btnLevels.SetCustomData(options.levels ? (void*)1 : NULL);
		btnLevels.SetSkin(options.levels ? &g_skin.chk : &g_skin.txt);
		btnPrivateMsgs.SetCustomData(options.privatemsgs ? (void*)1 : NULL);
		btnPrivateMsgs.SetSkin(options.privatemsgs ? &g_skin.chk : &g_skin.txt);
	}

	void Store() {
		options.infomsgs = (btnInfoMsgs.GetCustomData() != NULL);
		options.eventmsgs = (btnEventMsgs.GetCustomData() != NULL);
		options.statusmsgs = (btnStatusMsgs.GetCustomData() != NULL);
		options.timestamps = (btnTimestamps.GetCustomData() != NULL);
		options.levels = (btnLevels.GetCustomData() != NULL);
		options.privatemsgs = (btnPrivateMsgs.GetCustomData() != NULL);
		options.Save();
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

