#ifndef __ENGINE_H
#define __ENGINE_H


#include "debugprint.h"
class Engine {
    public:
        virtual ~Engine() {
            DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_DEBUGGING, "Engine::~Engine(): Function not overloaded");
        }
        virtual void BlitRect() {
            DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_DEBUGGING, "Engine::BlitRect(): Function not overloaded");
        }
        virtual bool Supported() {
            DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_DEBUGGING, "Engine::Supported(): Function not overloaded");
            return false;
        }
};

#include "enginesdl.h"
#include "enginegl.h"
#include "enginedx.h"

#endif
