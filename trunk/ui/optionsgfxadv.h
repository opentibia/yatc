#ifndef __UI_OPTIONSGFXADV_H
#define __UI_OPTIONSGFXADV_H

#include "engine.h"

class winOptionsGraphicsAdvanced_t {
public:
	glictWindow window;

	// engines
	glictPanel lblEngine;           // 14 17, 135 14
	glictButton btnEngineSDL;		// 14 30, 56 18
	glictButton btnEngineOGL;       // 84 30
	//glictButton btnEngineDX9;     // 154 30

    glictButton *activeEngine;
    glictButton *activeEngineOriginal;

	// 9 277, 244 2
	glictPanel pnlSeparator;

	glictButton btnHelp;// 100 286, 41 17
	glictButton btnOk; // 153 286
	glictButton btnCancel; // 206 286
	winOptionsGraphicsAdvanced_t () {
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
		lblEngine.SetCaption("Select Graphics Options:");
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
		//int engineType = (int)caller->GetCustomData();
		winOptionsGraphicsAdvanced_t* woga = (winOptionsGraphicsAdvanced_t*)(caller->GetCustomData());
		woga->activeEngine->SetHold(false);

		options.engine = ENGINE_SDL; // default
		if (caller == &woga->btnEngineSDL) {
		    options.engine = ENGINE_SDL;
		}
		if (caller == &woga->btnEngineOGL) {
		    options.engine = ENGINE_OPENGL;
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
		//  g_engine = engine;
		//  options.engine = engineType;
		//}
	}

};

#endif

