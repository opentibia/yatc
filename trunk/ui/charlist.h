#ifndef __UI_CHARLIST_H
#define __UI_CHARLIST_H

#include <GLICT/list.h>
#include <sstream>
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


	winCharlist_t() {

		window.SetWidth(228);
		window.SetHeight(273);
		window.SetCaption("Select Character");
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
		lblSelectChar.SetCaption("Select character:");
		lblSelectChar.SetPos(14,17);
		lblSelectChar.SetBGActiveness(false);
		lblSelectChar.SetFont("aafont");

		lstChars.SetPos(14,30);
		lstChars.SetWidth(200);
		lstChars.SetHeight(146);
		lstChars.SetBGColor(.2,.2,.2,1.);
		if (options.ui_compat) lstChars.SetNext(&btnOk);

		lblAccStatus.SetWidth(200);
		lblAccStatus.SetHeight(12);
		lblAccStatus.SetCaption("Account Status:");
		lblAccStatus.SetPos(14,191);
		lblAccStatus.SetBGActiveness(false);
		lblAccStatus.SetFont("aafont");

		pnlAccStatus.SetWidth(200);
		pnlAccStatus.SetHeight(12);
		pnlAccStatus.SetCaption("Unknown");
		pnlAccStatus.SetPos(14,205);
		pnlAccStatus.SetBGActiveness(false);
		pnlAccStatus.SetFont("aafont");

		pnlSeparator.SetPos(9, 237);
		pnlSeparator.SetBGColor(.1,.1,.1,1.);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);

		btnOk.SetPos(123, 247);
		btnOk.SetWidth(43);
		btnOk.SetHeight(20);
		btnOk.SetCaption("Ok");
		btnOk.SetBGColor(.1,.1,.1,1.);
		btnOk.SetFont("minifont",8);
		if (options.ui_compat) btnOk.SetNext(&btnCancel);

		btnCancel.SetPos(176, 247);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(20);
		btnCancel.SetCaption("Cancel");
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
			pnlAccStatus.SetCaption ("Free Account");
		else if (pd > 32767)
			pnlAccStatus.SetCaption ("Gratis Premium Account");
		else {
			std::stringstream s;
			s << "Premium Account (" << pd << " days left)";
			pnlAccStatus.SetCaption(s.str());
		}
		lstChars.Focus(NULL);
	}

	void addChar (const CharacterList_t& chr) {
		std::stringstream s;

		s << chr.name << " (" << chr.world << ")";
		// FIXME (ivucica#5#) change "res" into "pnl"
		glictPanel *res = new glictPanel;
		CharacterList_t *data = new CharacterList_t;
		*data = chr;
		data->extra = this;

		res->SetBGActiveness(false);
		res->SetBGColor(.4,.4,.4,1.);
		res->SetOnClick(winCharlist_t::OnListbox);
		res->SetCustomData(data);
		res->SetCaption(s.str());
		res->SetFont("aafont");
		res->SetFocusable(false);

		lsiChars.insert(lsiChars.end(), res);
		lstChars.AddObject(res);

		if (lsiChars.size() == 1) {
			OnListbox(NULL, res);
		}
	}


	static void OnListbox(glictPos* pos, glictContainer *caller) {
		winCharlist_t *wc = (winCharlist_t*) (((CharacterList_t*)caller->GetCustomData())->extra);
		for (std::vector<glictPanel*>::iterator it = wc->lsiChars.begin(); it != wc->lsiChars.end(); it++) {
			(*it)->SetBGActiveness(false);
		}
		((glictPanel*)caller)->SetBGActiveness(true);
		wc->currentChar = *((CharacterList_t*)(caller->GetCustomData()));
	}



};

#endif

