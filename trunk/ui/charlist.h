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

#ifndef __UI_CHARLIST_H
#define __UI_CHARLIST_H

#include <GLICT/list.h>
#include <sstream>

#if defined(HAVE_CONFIG_H)
    #include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
class winCharlist_t {
public:

	glictWindow window; // 228 273


	glictPanel lblSelectChar; // 14 17 200 12
	glictList lstChars; // 14 30 200 146

	glictPanel lblAccStatus; // 14 191 200 12
	glictPanel pnlAccStatus; // 14 205 200 12

	glictPanel pnlSeparator; // 9 273 210 2

	glictButton btnOk, btnCancel; // 123 247 43 20 ; 176



	std::vector<glictPanel*> lsiChars;
	CharacterList_t currentChar;
    int currentCharId;

	winCharlist_t() {
        currentCharId = 0;

		window.SetWidth(228);
		window.SetHeight(273);
		window.SetCaption(gettext("Select Character"));
		window.SetVisible(false);
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&lblSelectChar);
		window.AddObject(&lstChars);
		window.AddObject(&lblAccStatus);
		window.AddObject(&pnlAccStatus);
		window.AddObject(&pnlSeparator);
		window.AddObject(&btnOk);
		window.AddObject(&btnCancel);

		lblSelectChar.SetWidth(200);
		lblSelectChar.SetHeight(12);
		lblSelectChar.SetCaption(gettext("Select character:"));
		lblSelectChar.SetPos(14,17);
		lblSelectChar.SetBGActiveness(false);
		lblSelectChar.SetFont("aafont");

		lstChars.SetPos(14,30);
		lstChars.SetWidth(200);
		lstChars.SetHeight(146);
		lstChars.SetBGColor(.25,.25,.25,1.);
		lstChars.SetFocusable(true);
		lstChars.SetSkin(&g_skin.txt);
		if (options.ui_compat) lstChars.SetNext(&btnOk);

		lblAccStatus.SetWidth(200);
		lblAccStatus.SetHeight(12);
		lblAccStatus.SetCaption(gettext("Account Status:"));
		lblAccStatus.SetPos(14,191);
		lblAccStatus.SetBGActiveness(false);
		lblAccStatus.SetFont("aafont");

		pnlAccStatus.SetWidth(200);
		pnlAccStatus.SetHeight(12);
		pnlAccStatus.SetCaption(gettext("Unknown"));
		pnlAccStatus.SetPos(14,205);
		pnlAccStatus.SetBGActiveness(false);
		pnlAccStatus.SetFont("aafont");

		pnlSeparator.SetPos(9, 237);
		pnlSeparator.SetBGColor(.1,.1,.1,1.);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetSkin(&g_skin.chk);

		btnOk.SetPos(123, 247);
		btnOk.SetWidth(43);
		btnOk.SetHeight(20);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetBGColor(.1,.1,.1,1.);
		btnOk.SetFont("minifont",8);
		if (options.ui_compat) btnOk.SetNext(&btnCancel);

		btnCancel.SetPos(176, 247);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(20);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetBGColor(.1,.1,.1,1.);
		btnCancel.SetFont("minifont",8);
		if (options.ui_compat) btnCancel.SetNext(&lstChars);
	}

	~winCharlist_t() {
		if (lsiChars.size())
			destroyList();
		printf("Destroyed winCharlist\n");
	}

	void destroyList() {
		printf("Destroying character list...\n");

		for (std::vector<glictPanel*>::iterator it = lsiChars.begin(); it != lsiChars.end(); it++) {
		    #if (GLICT_APIREV >= 71)
		    printf(".");
		    lstChars.RemoveObject(*it);
            lstChars.DelayedRemove();
		    #else
            #warning Stuff wont work ok till you upgrade to GLICT APIREV >= 71
            #endif

            delete (CharacterList_t*)((*it)->GetCustomData());
			delete (*it);
        }
		lsiChars.clear();
		printf("Done!\n");
	}

	void generateList(const std::list<CharacterList_t>& list, int pd) {
		for (std::list<CharacterList_t>::const_iterator it = list.begin(); it != list.end(); it++) {
			addChar(*it);
		}
		if (pd == 0)
			pnlAccStatus.SetCaption (gettext("Free Account"));
		else if (pd > 32767)
			pnlAccStatus.SetCaption (gettext("Gratis Premium Account"));
		else {
			std::stringstream s;
			s << gettext("Premium Account") << " (" << pd << " " << gettext("days left") << ")";
			pnlAccStatus.SetCaption(s.str());
		}
		lstChars.Focus(NULL);
	}

	void addChar (const CharacterList_t& chr) {
		std::stringstream s;

		s << chr.name << " (" << chr.world << ")";
		glictPanel *pnl = new glictPanel;
		CharacterList_t *data = new CharacterList_t;
		*data = chr;
		data->extra = this;

		pnl->SetBGActiveness(false);
		pnl->SetBGColor(.4,.4,.4,1.);
		pnl->SetOnClick(winCharlist_t::OnListbox);
		pnl->SetCustomData(data);
		pnl->SetCaption(s.str());
		pnl->SetFont("aafont");
		pnl->SetFocusable(false);
		#if GLICT_APIREV < 68
		#warning Aesthetic corrections on listboxes wont work without GLICT of apirev 68+
		#else
		pnl->SetTextOffset(4,2);
		#endif

		lsiChars.push_back(pnl);
		lstChars.AddObject(pnl);

		if (lsiChars.size() == 1) {
			OnListbox(NULL, pnl);
		}
	}


	static void OnListbox(glictPos* pos, glictContainer *caller) {
		winCharlist_t *wc = (winCharlist_t*) (((CharacterList_t*)caller->GetCustomData())->extra);
		int i=0;
		for (std::vector<glictPanel*>::iterator it = wc->lsiChars.begin(); it != wc->lsiChars.end(); it++) {
			(*it)->SetBGActiveness(false);
			if (*it == caller) wc->currentCharId = i;
			i++;
		}
		((glictPanel*)caller)->SetBGActiveness(true);
		wc->currentChar = *((CharacterList_t*)(caller->GetCustomData()));
	}



};

#endif

