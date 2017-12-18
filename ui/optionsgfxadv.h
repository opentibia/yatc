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

#ifndef __UI_OPTIONSGFXADV_H
#define __UI_OPTIONSGFXADV_H
#if defined(HAVE_CONFIG_H)
    #include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include "../engine.h"
#include "../choicegrid.h"
#include "../util.h"

#include <sstream>
#include <algorithm>

class winOptionsGraphicsAdvanced_t {
public:
	glictWindow window;

	// engines
	glictPanel lblEngine;           // 14 17, 135 14
	glictButton btnEngineSDL;		// 14 30, 56 18
	glictButton btnEngineOGL;       // 84 30
	//glictButton btnEngineDX9;     // 154 30

	glictPanel lblAmbLight; // 13 62, 230 12
	glictScrollbar scbAmbLight; // 13 80, 230 12

	ChoiceGrid btnLightEffects;	// 4 16, 12 12
	ChoiceGrid::Item* btnsLight[4];
	uiCheckbox chkStrShrGWind;	// 4 16, 12 12
	uiCheckbox chkWinMouse;	// 4 16, 12 12
	uiCheckbox chkSmoothStretch; // 4 16, 12 12

    glictPanel lblMaxFPS; // 13 213 , 230 12
	glictScrollbar scbMaxFPS; // 13 229, 230 12
	glictPanel lblFPS;

    glictButton *activeEngine;
    glictButton *activeEngineOriginal;

	// 9 277, 244 2
	glictPanel pnlSeparator;

	glictButton btnHelp;// 100 286, 41 17
	glictButton btnOk; // 153 286
	glictButton btnCancel; // 206 286
	winOptionsGraphicsAdvanced_t () {
		std::stringstream ss;

		window.SetVisible(false);
		window.SetHeight(351);
		window.SetWidth(263);
		window.SetCaption(gettext("Advanced Graphics Options"));
		window.SetBGColor(.4, .4, .4, 1.);

		// Select Graphics Engine
		window.AddObject(&lblEngine);
		lblEngine.SetPos(14, 17);
		lblEngine.SetWidth(135 + 15); // 15 is just a failsafe
		lblEngine.SetHeight(14);
		lblEngine.SetCaption(std::string() + gettext("Select graphics engine") + ":");
		lblEngine.SetFont("aafont");
		lblEngine.SetBGActiveness(false);

        window.AddObject(&btnEngineSDL);
		btnEngineSDL.SetPos(14, 30);
		btnEngineSDL.SetWidth(56);
		btnEngineSDL.SetHeight(18);
		btnEngineSDL.SetCaption("SDL");
		btnEngineSDL.SetFont("minifont",8);
		btnEngineSDL.SetCustomData(this);
		btnEngineSDL.SetOnClick(winOptionsGraphicsAdvanced_t::OnChangeEngine);

		window.AddObject(&btnEngineOGL);
		btnEngineOGL.SetPos(84, 30);
		btnEngineOGL.SetWidth(56);
		btnEngineOGL.SetHeight(18);
		btnEngineOGL.SetCaption("OpenGL");
		btnEngineOGL.SetFont("minifont",8);
		btnEngineOGL.SetCustomData(this);
		btnEngineOGL.SetOnClick(winOptionsGraphicsAdvanced_t::OnChangeEngine);
#ifndef USE_OPENGL
		btnEngineOGL.SetEnabled(false);
#endif

		//window.AddObject(&btnEngineDX);
		//btnEngineDX.SetPos(154, 30);
		//btnEngineDX.SetWidth(56);
		//btnEngineDX.SetHeight(18);
		//btnEngineDX.SetCaption("DirectX 9");
		//btnEngineDX.SetFont("minifont",8);
		//btnEngineDX.SetCustomData((void*)2);
		//btnEngineDX.SetOnClick(winOptionsGraphicsAdvanced_t::OnChangeEngine);

		// "Set Ambient Light"
		{
			window.AddObject(&lblAmbLight);
			lblAmbLight.SetPos(13, 62);
			lblAmbLight.SetWidth(230);
			lblAmbLight.SetHeight(12);
			/*
			ss << gettext("Set Ambient Light") << ": " << options.ambientlight << "%";
			lblAmbLight.SetCaption(ss.str());
			ss.str("");
			*/
			lblAmbLight.SetCaption(gettext("Ambient Light"));
			#if (GLICT_APIREV >= 85)
    		lblAmbLight.SetCaptionColor(.5f, .5f, .5f);
			#else
			#warning No support for setcaptioncolor before glict apirev 85
			#endif
			lblAmbLight.SetFont("aafont");
			lblAmbLight.SetBGActiveness(false);
			lblAmbLight.SetCustomData(this);

			window.AddObject(&scbAmbLight);
			scbAmbLight.SetPos(13,80);
			scbAmbLight.SetWidth(230);
			scbAmbLight.SetHeight(12);
			scbAmbLight.SetMin(0);
			scbAmbLight.SetMax(/*10*/0);
			scbAmbLight.SetStep(5);
			scbAmbLight.SetValue(0/*options.ambientlight*/);
			scbAmbLight.SetCustomData(this);
			//scbMaxFPS.SetOnClick(winOptionsGraphicsAdvanced_t::OnChangeAmbLight);
		}

		// "Show Light Effects"
		{
			window.AddObject(btnLightEffects.getGrid());
			//chkLightEffects.SetPos(13, 108);
			//chkLightEffects.SetWidth(230);
			//chkLightEffects.SetHeight(22);
			//chkLightEffects.SetCaption(gettext("Show Light Effects"));
			//chkLightEffects.SetValue(options.showlighteffects != 0);
			btnLightEffects.getGrid()->SetPos(13, 108);
			btnLightEffects.setItemSize(50, 20);
			btnLightEffects.setPadding(4, 4);
			btnLightEffects.setRows(4);
			//btnLightEffects.SetOnClick(OnLightEffect);
			btnsLight[0] = btnLightEffects.addItem(gettext("No Light"), NULL, (void*)0);
			btnsLight[1] = btnLightEffects.addItem(gettext("Simple Light"), NULL, (void*)1);
			btnsLight[2] = btnLightEffects.addItem(gettext("Better Light"), NULL, (void*)2);
			btnsLight[3] = btnLightEffects.addItem(gettext("Best Light"), NULL, (void*)3);
			btnLightEffects.setSelected(btnsLight[std::max(std::min(3, options.showlighteffects), 0)]);
		}

		// "Don't Stretch/Shrink Game Window"
		{
			window.AddObject(&chkStrShrGWind.pnlPanel);
			chkStrShrGWind.SetPos(13, 144);
			chkStrShrGWind.SetWidth(230);
			chkStrShrGWind.SetHeight(22);
			chkStrShrGWind.SetCaption(gettext("Don't Stretch/Shrink Game Window"));
			chkStrShrGWind.SetValue(options.stretchGameWindow);
		}

		// "Windows Mouse Pointer"
		{
			window.AddObject(&chkWinMouse.pnlPanel);
			chkWinMouse.SetPos(13, 178);
			chkWinMouse.SetWidth(230);
			chkWinMouse.SetHeight(22);
			chkWinMouse.SetCaption(gettext("Windows Mouse Pointer"));
			chkWinMouse.SetValue(false);
			chkWinMouse.SetEnabled(false);
		}

		// YATC specific: Smooth stretching
		{
			window.AddObject(&chkSmoothStretch.pnlPanel);
			chkSmoothStretch.SetPos(13, 212);
			chkSmoothStretch.SetWidth(230);
			chkSmoothStretch.SetHeight(22);
			chkSmoothStretch.SetCaption(gettext("Use Smooth Stretching"));
			chkSmoothStretch.SetValue(options.smoothstretch);
			if(options.engine != ENGINE_SDL){
				chkSmoothStretch.SetEnabled(false);
			}
		}

        // scrollbar for fps
		{
			window.AddObject(&lblMaxFPS);
			lblMaxFPS.SetPos(13, 247);
			lblMaxFPS.SetWidth(230);
			lblMaxFPS.SetHeight(12);
			ss << gettext("Adjust framerate limit") << ": ";
			if(options.maxfps == 0) {
				ss << gettext("Unlimited");
			} else {
				ss << options.maxfps;
			}
			lblMaxFPS.SetCaption(ss.str());
			lblMaxFPS.SetBGActiveness(false);
			lblMaxFPS.SetCustomData(this);

			window.AddObject(&scbMaxFPS);
			scbMaxFPS.SetPos(13,263);
			scbMaxFPS.SetWidth(230);
			scbMaxFPS.SetHeight(12);
			scbMaxFPS.SetMin(0);
			scbMaxFPS.SetMax(80);
			scbMaxFPS.SetStep(5);
			scbMaxFPS.SetValue(options.maxfps);
			scbMaxFPS.SetCustomData(this);
			scbMaxFPS.SetOnClick(winOptionsGraphicsAdvanced_t::OnChangeFPS);
			// NOTE (nfries88): For when we implement horizontal scrollbar chip dragging in glict
			scbMaxFPS.SetOnMouseUp(winOptionsGraphicsAdvanced_t::OnChangeFPS);

			window.AddObject(&lblFPS);
			lblFPS.SetPos(13, 280);
			lblFPS.SetWidth(230);
			lblFPS.SetHeight(12);
			lblFPS.SetBGActiveness(false);
			lblFPS.SetCustomData(this);
			lblFPS.SetOnPaint(lblFPSOnPaint);
		}

		// Bottom stuff, every window has 'em.
		{
			window.AddObject(&pnlSeparator);
			pnlSeparator.SetPos(9, 311);
			pnlSeparator.SetWidth(241);
			pnlSeparator.SetHeight(2);
			pnlSeparator.SetBGColor(.2,.2,.2,1.);
			pnlSeparator.SetSkin(&g_skin.chk);

			window.AddObject(&btnHelp);
			btnHelp.SetPos(100, 320);
			btnHelp.SetWidth(41);
			btnHelp.SetHeight(17);
			btnHelp.SetCaption(gettext("Help"));
			btnHelp.SetFont("minifont",8);

			window.AddObject(&btnOk);
			btnOk.SetPos(153, 320);
			btnOk.SetWidth(41);
			btnOk.SetHeight(17);
			btnOk.SetCaption(gettext("Ok"));
			btnOk.SetFont("minifont",8);

			window.AddObject(&btnCancel);
			btnCancel.SetPos(206, 320);
			btnCancel.SetWidth(41);
			btnCancel.SetHeight(17);
			btnCancel.SetCaption(gettext("Cancel"));
			btnCancel.SetFont("minifont",8);
		}
	}


	void Init() {
	    btnEngineSDL.SetHold(false);
	    btnEngineOGL.SetHold(false);
		switch (options.engine) {
		    default:
		    case ENGINE_SDL:
                btnEngineSDL.SetHold(true);
                activeEngine = &btnEngineSDL;
                break;
            case ENGINE_OPENGL:
                btnEngineOGL.SetHold(true);
                activeEngine = &btnEngineOGL;
                break;
		}
		activeEngineOriginal = activeEngine;
	}

	void Store() {
		//
		options.stretchGameWindow = chkStrShrGWind.GetValue();
		options.smoothstretch = chkSmoothStretch.GetValue();
		//options.showlighteffects = chkLightEffects.GetValue();
		options.showlighteffects = VOIDP2INT(btnLightEffects.getSelected()->data);
		options.Save();
	}

	static void OnChangeEngine(glictPos* pos, glictContainer *caller) {
		winOptionsGraphicsAdvanced_t* woga = (winOptionsGraphicsAdvanced_t*)(caller->GetCustomData());
		woga->activeEngine->SetHold(false);

		enginelist_t engineType = ENGINE_SDL;
		if (caller == &woga->btnEngineSDL) {
		    engineType = ENGINE_SDL;
		}
		else if (caller == &woga->btnEngineOGL) {
		    engineType = ENGINE_OPENGL;
		}
		woga->activeEngine = (glictButton*)caller;
		woga->activeEngine->SetHold(true);

		//Engine* engine = NULL;
  		//switch(engineType) {
		//#ifdef USE_OPENGL
		//	case ENGINE_OPENGL:
		//		engine = new EngineGL(g_engine);
		//		break;
		//#endif
		//	default: case ENGINE_SDL:
		//		engine = new EngineSDL(g_engine);
		//		break;
		//}
		//if(engine->isSupported()) {
		//	delete g_engine;
		//	g_engine = engine;
			options.engine = engineType;
		//}
		if(options.engine != ENGINE_SDL){
			woga->chkSmoothStretch.SetEnabled(false);
		}
	}

	static void OnChangeFPS(glictPos* pos, glictContainer *caller) {
		glictScrollbar* sb = (glictScrollbar*)caller;
		winOptionsGraphicsAdvanced_t* win = (winOptionsGraphicsAdvanced_t*)caller->GetCustomData();
		std::stringstream ss;

		options.maxfps = int(sb->GetValue());
		DEBUGPRINT(0, 0, "New FPS: %d\n", options.maxfps);

		ss << gettext("Adjust framerate limit") << ": ";
		if(options.maxfps == 0) {
			ss << gettext("Unlimited");
		} else {
			ss << options.maxfps;
		}
		win->lblMaxFPS.SetCaption(ss.str());
	}

	static void lblFPSOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
	{
		std::stringstream ss;
		ss << gettext("Current framerate: ") << g_engine->getFPS();
		caller->SetCaption(ss.str());
	}

};

#endif
