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

#ifndef __UI_OPTIONSGRAPHICS_H
#define __UI_OPTIONSGRAPHICS_H

#include <GLICT/list.h>
#include <sstream>
#if !defined(__APPLE__) && !defined(WIN32)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
#include "../engine.h"
#include "checkbox.h"
class winOptionsGraphics_t {
public:


	struct resdata_t {
		resdata_t () {}
		resdata_t operator= (resdata_t& op) {
			w = op.w; h = op.h; bpp = op.bpp; wog = op.wog;
			return *this;
		}
		resdata_t (int w1, int h1, int bpp1, winOptionsGraphics_t* wog1) { w = w1; h = h1; bpp = bpp1; wog = wog1;}
		int w, h, bpp;
		winOptionsGraphics_t *wog;
	};
	resdata_t currentres;

	glictWindow window;

	// 15 16, 198 20
	uiCheckbox chkFullscreen;

	glictPanel lblResolutions; // 14 52, 194 12
	glictList lstResolutions; // 14 65, 200 104
	std::vector<glictPanel*> lsiResolutions;

	glictPanel lblAdvancedSettings; // 14 176, 113 12
	glictButton btnAdvancedSettings; // 131 184 81 16

	// 9 220, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 232, 40 17
	glictButton btnOk; // 125 232
	glictButton btnCancel; // 178 232
	winOptionsGraphics_t () {

		window.SetVisible(false);
		window.SetHeight(256);
		window.SetWidth(228);
		window.SetCaption(gettext("Graphics Options"));
		window.SetBGColor(.4, .4, .4, 1.);

        window.AddObject(&chkFullscreen.pnlPanel);
		chkFullscreen.SetPos(16,16);
		chkFullscreen.SetWidth(200);
		chkFullscreen.SetHeight(22);
		chkFullscreen.SetCaption(gettext("Fullscreen"));

		window.AddObject(&lblResolutions);
		lblResolutions.SetPos(14, 52);
		lblResolutions.SetWidth(200);
		lblResolutions.SetHeight(12);
		lblResolutions.SetCaption(gettext("Available resolutions:"));
		lblResolutions.SetFont("aafont");
		lblResolutions.SetBGActiveness(false);

		window.AddObject(&lstResolutions);
		lstResolutions.SetPos(14, 65);
		lstResolutions.SetWidth(200);
		lstResolutions.SetHeight(104);
		lstResolutions.SetBGColor(.2,.2,.2,1.);



		window.AddObject(&lblAdvancedSettings);
		lblAdvancedSettings.SetPos(14, 186);
		lblAdvancedSettings.SetWidth(200);
		lblAdvancedSettings.SetHeight(12);
		lblAdvancedSettings.SetCaption(gettext("Advanced settings:"));
		lblAdvancedSettings.SetFont("aafont");
		lblAdvancedSettings.SetBGActiveness(false);

		window.AddObject(&btnAdvancedSettings);
		btnAdvancedSettings.SetPos(131, 184);
		btnAdvancedSettings.SetWidth(81);
		btnAdvancedSettings.SetHeight(16);
		btnAdvancedSettings.SetCaption(gettext("Advanced"));
		btnAdvancedSettings.SetFont("minifont",8);




		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(9, 220);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(72, 232);
		btnHelp.SetWidth(43);
		btnHelp.SetHeight(19);
		btnHelp.SetCaption(gettext("Help"));
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(125, 232);
		btnOk.SetWidth(43);
		btnOk.SetHeight(19);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(178, 232);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(19);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetFont("minifont",8);

		FillResolutions();
	}

	void Init() {
        chkFullscreen.SetValue(options.fullscreen);

		currentres.w = 0; currentres.h = 0; currentres.bpp = 0;
		for (std::vector<glictPanel*>::iterator it = lsiResolutions.begin(); it != lsiResolutions.end(); it++) {
			resdata_t *data = (resdata_t *)((*it)->GetCustomData());
			if (data->w == options.w && data->h == options.h && data->bpp == options.bpp) {
				(*it)->SetBGActiveness(true);
				currentres = *((resdata_t*)(*it)->GetCustomData());
			} else {
				(*it)->SetBGActiveness(false);
			}
		}
	}

	void Store() {
	    options.fullscreen = chkFullscreen.GetValue();
		if (currentres.w) {
			options.w = currentres.w; options.h = currentres.h; options.bpp = currentres.bpp;
			g_engine->doResize(currentres.w, currentres.h);
		}
        options.Save();

	}


	static void OnListbox(glictPos* pos, glictContainer *caller) {
		// worthy oriental gentlemen ;)
		winOptionsGraphics_t *wog = ((resdata_t*)(caller->GetCustomData()))->wog;
		for (std::vector<glictPanel*>::iterator it = wog->lsiResolutions.begin(); it != wog->lsiResolutions.end(); it++) {
			(*it)->SetBGActiveness(false);
		}
		((glictPanel*)caller)->SetBGActiveness(true);
		wog->currentres = *((resdata_t*)(caller->GetCustomData()));
	}


	void FillResolutions() {


		SDL_Rect **modes;
		int i,j;

		/* Get available fullscreen/hardware modes */
		modes=SDL_ListModes(NULL, g_engine->m_videoflags | SDL_FULLSCREEN);

		/* Check is there are any modes available */
		if(modes == (SDL_Rect **)0){
			return;
		}

		/* Check if our resolution is restricted */
		if(modes == (SDL_Rect **)-1){
			// all res are available
			AddResolution(640, 480, 16);
			AddResolution(800, 600, 16);
			AddResolution(1024, 768, 16);

			AddResolution(640, 480, 32);
			AddResolution(800, 600, 32);
			AddResolution(1024, 768, 32);

		}
		else{
		  /* Print valid modes */
			for(i=0;modes[i+1];++i){
				for(j=i+1;modes[j];++j){
					if(modes[i]->w > modes[j]->w){
						SDL_Rect *r;
						r = modes[i];
						modes[i] = modes[j];
						modes[j] = r;
					}
				}
			}
			
			for(i=0;modes[i];++i)
				AddResolution(modes[i]->w, modes[i]->h, 16);
			for(i=0;modes[i];++i)
				AddResolution(modes[i]->w, modes[i]->h, 32);
		}



	}

	void AddResolution (int w, int h, int bpp) {


		std::stringstream s;

		s << w << "*" << h << ", " << bpp << " bit";

		glictPanel *res = new glictPanel;
		resdata_t *data = new resdata_t(w,h,bpp,this);

		res->SetBGActiveness(false);
		res->SetBGColor(.4,.4,.4,1.);
		res->SetOnClick(winOptionsGraphics_t::OnListbox);
		res->SetCustomData(data);
		res->SetCaption(s.str());
		res->SetFont("aafont");
		#if GLICT_APIREV < 68
		#warning Aesthetic corrections on listboxes wont work without GLICT of apirev 68+
		#else
		res->SetTextOffset(4,2);
		#endif

		lsiResolutions.insert(lsiResolutions.end(), res);
		lstResolutions.AddObject(res);

	}

};


#endif
