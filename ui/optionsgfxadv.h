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

#include "../engine.h"

#include <sstream>

class winOptionsGraphicsAdvanced_t {
public:
	glictWindow window;

	// engines
	glictPanel lblEngine;           // 14 17, 135 14
	glictButton btnEngineSDL;		// 14 30, 56 18
	glictButton btnEngineOGL;       // 84 30
	//glictButton btnEngineDX9;     // 154 30

    glictPanel lblMaxFPS; // 13 213 , 230 12
	glictScrollbar scbMaxFPS; // 13 229, 230 12

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
		window.SetHeight(317);
		window.SetWidth(263);
		window.SetCaption("Advanced Graphics Options");
		window.SetBGColor(.4, .4, .4, 1.);

		// Select Graphics Engine
		window.AddObject(&lblEngine);
		lblEngine.SetPos(14, 17);
		lblEngine.SetWidth(135 + 15); // 15 is just a failsafe
		lblEngine.SetHeight(14);
		lblEngine.SetCaption("Select graphics engine:");
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

		//window.AddObject(&btnEngineDX);
		//btnEngineDX.SetPos(154, 30);
		//btnEngineDX.SetWidth(56);
		//btnEngineDX.SetHeight(18);
		//btnEngineDX.SetCaption("DirectX 9");
		//btnEngineDX.SetFont("minifont",8);
		//btnEngineDX.SetCustomData((void*)2);
		//btnEngineDX.SetOnClick(winOptionsGraphicsAdvanced_t::OnChangeEngine);


        // scrollbar for fps
		window.AddObject(&scbMaxFPS);
		scbMaxFPS.SetPos(13,229);
		scbMaxFPS.SetWidth(230);
		scbMaxFPS.SetHeight(12);
		scbMaxFPS.SetMin(0);
		scbMaxFPS.SetMax(80);
		scbMaxFPS.SetStep(5);
		scbMaxFPS.SetValue(options.maxfps);
		scbMaxFPS.SetCustomData(this);
		scbMaxFPS.SetOnClick(winOptionsGraphicsAdvanced_t::OnChangeFPS);

		window.AddObject(&lblMaxFPS);
		lblMaxFPS.SetPos(13, 213);
		lblMaxFPS.SetWidth(230);
		lblMaxFPS.SetHeight(12);
		//lblMaxFPS.SetCaption("Adjust framerate limit: XX");
		ss << "Adjust framerate limit: ";
		if(options.maxfps == 0) {
			ss << "Unlimited";
		} else {
			ss << options.maxfps;
		}
		lblMaxFPS.SetCaption(ss.str());
		lblMaxFPS.SetBGActiveness(false);
		lblMaxFPS.SetCustomData(this);


		// Bottom stuff, every window has 'em.
		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(9, 277);
		pnlSeparator.SetWidth(241);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(100, 286);
		btnHelp.SetWidth(41);
		btnHelp.SetHeight(17);
		btnHelp.SetCaption("Help");
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(153, 286);
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption("Ok");
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(206, 286);
		btnCancel.SetWidth(41);
		btnCancel.SetHeight(17);
		btnCancel.SetCaption("Cancel");
		btnCancel.SetFont("minifont",8);
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
	}

	static void OnChangeFPS(glictPos* pos, glictContainer *caller) {
		glictScrollbar* sb = (glictScrollbar*)caller;
		winOptionsGraphicsAdvanced_t* win = (winOptionsGraphicsAdvanced_t*)caller->GetCustomData();
		std::stringstream ss;

		options.maxfps = sb->GetValue();
		DEBUGPRINT(0, 0, "New FPS: %d\n", options.maxfps);

		ss << "Adjust framerate limit: ";
		if(options.maxfps == 0) {
			ss << "Unlimited";
		} else {
			ss << options.maxfps;
		}
		win->lblMaxFPS.SetCaption(ss.str());
	}

};

#endif

