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

#ifndef __UI_CHECKBOX_H
#define __UI_CHECKBOX_H

#include "../skin.h"
class uiCheckbox {
public:
    uiCheckbox() {
        pnlPanel.SetWidth(198);
		pnlPanel.SetHeight(20);
        pnlPanel.AddObject(&btnButton);
        pnlPanel.AddObject(&lblLabel);
        pnlPanel.SetBGActiveness(false);

        btnButton.SetPos(5,5);
        btnButton.SetWidth(12);
        btnButton.SetHeight(12);
        btnButton.SetOnClick(uiCheckbox::OnCheckbox);
        lblLabel.SetPos(22,7);
        lblLabel.SetWidth(170);
        lblLabel.SetHeight(11);
        lblLabel.SetFont("aafont");
		lblLabel.SetBGActiveness(false);

        SetValue(false);

    }

    void SetPos(int x, int y) {
        pnlPanel.SetPos(x,y);
    }

    void SetWidth(int w) {
        pnlPanel.SetWidth(w);
        lblLabel.SetWidth(w-28);
    }

    void SetHeight(int h) {
        pnlPanel.SetHeight(h);
    }

    void SetSize(int w, int h) {
        SetWidth(w);
        SetHeight(h);
    }

    void SetCaption(const std::string& s) {
        lblLabel.SetCaption(s);
    }

    bool GetValue() {
        return btnButton.GetCustomData() != NULL;
    }

    void SetValue(bool value) {
        btnButton.SetCustomData(value ? (void*)1 : NULL);
        btnButton.SetSkin(value ? &g_skin.chk : &g_skin.txt);

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


	glictPanel pnlPanel;
	glictPanel btnButton;
	glictPanel lblLabel;

};
#endif
