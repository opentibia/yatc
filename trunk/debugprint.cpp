#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <wincon.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include "debugprint.h"


#include <GL/gl.h>
#if defined(WIN32) && defined(GREMDEY)
    #include <GL/GRemdeyExtensions.h>
    extern PFNGLSTRINGMARKERGREMEDYPROC glStringMarkerGREMEDY;
#endif

#ifndef DEBUGLEVEL_BUILDTIME
    #warning You should define DEBUGLEVEL_BUILDTIME in compiler options.
    #define DEBUGLEVEL_BUILDTIME 0
#endif
char debuglevel=DEBUGLEVEL_BUILDTIME;

std::string DEBUG_FILE; int DEBUG_LINE;
void DEBUGPRINTx (char msgdebuglevel, char type, char* txt, ...) {
    va_list vl;
	va_start(vl, txt);

    if (msgdebuglevel <= debuglevel) {

        char tx[6000];
    	vsnprintf(tx, 6000, txt, vl);


        #ifdef WIN32
            HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
        #endif

        switch (type) {
            default:
                printf(tx);
                break;
            case DEBUGPRINT_ERROR:
                #ifdef WIN32
                    SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_INTENSITY);
                #endif
                printf("[E] %s\n", tx);
                #ifdef WIN32
                    SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                #endif
                break;
            case DEBUGPRINT_WARNING:
                #ifdef WIN32
                    SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_GREEN);
                #endif
                printf("[W] %s\n", tx);
                #ifdef WIN32
                    SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                #endif
                break;
        }

/*
        FILE* DEBUGLOG = fopen("debuglog.txt","a");
		if (DEBUGLOG) {
        	fprintf(DEBUGLOG,tx);
        	fflush(DEBUGLOG);
        	fclose(DEBUGLOG);
		}
*/


/*
    	char tmp[6000];
    	snprintf(tmp,6000, "%s, %d", DEBUG_FILE.c_str(), DEBUG_LINE);
        DEBUGLOG = fopen("debuglin.txt","a");
        fprintf(DEBUGLOG, tmp);
        //for (int i=0;i<glutxNumberOfLines(tx)-1;i++) fprintf(DEBUGLOG, "\n");
        fflush(DEBUGLOG);
        fclose(DEBUGLOG);
*/



    	va_end(vl);
    }
}


void DEBUGMARKER (unsigned int size, const char *val) {
#if defined(WIN32) && defined(GREMDEY)
	if (glStringMarkerGREMEDY) glStringMarkerGREMEDY(size, val);
#endif
}
