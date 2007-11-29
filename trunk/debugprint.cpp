//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Debug output functions
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

#ifdef WIN32
	#ifndef WINCE
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
		#include <wincon.h>
	#else
		#include <windows.h>
	#endif
#endif

#include <stdio.h>
#include <stdarg.h>
#include "debugprint.h"
#include "wince.h"
#if USE_OPENGL
	#include <GL/gl.h>
	#if defined(WIN32) && defined(GREMDEY)
		#include <GL/GRemdeyExtensions.h>
		extern PFNGLSTRINGMARKERGREMEDYPROC glStringMarkerGREMEDY;
	#endif
#endif

#ifndef DEBUGLEVEL_BUILDTIME
	#warning You should define DEBUGLEVEL_BUILDTIME in compiler options.
	#define DEBUGLEVEL_BUILDTIME 0
#endif
char debuglevel=DEBUGLEVEL_BUILDTIME;

std::string DEBUG_FILE; int DEBUG_LINE;
void DEBUGPRINTx (char msgdebuglevel, char type, const char* txt, ...)
{
	va_list vl;
	va_start(vl, txt);

	if(msgdebuglevel <= debuglevel){
		char tx[6000];
		vsnprintf(tx, 6000, txt, vl);

		#if defined(WIN32) && !defined(WINCE)
		HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
		#endif
		
		#ifdef WINCE
		FILE *lf = fopen("log.txt", "a");
		#endif
		
		switch (type) {
			default:
				printf(tx);
				#ifdef WINCE
				fprintf(lf, "%s", tx);
				#endif

				break;
			case DEBUGPRINT_ERROR:
					#if defined(WIN32) && !defined(WINCE)
					SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_INTENSITY);
					#endif
					printf("[E] %s", tx);
					#ifdef WINCE
					fprintf(lf, "[E] %s", tx);
					#endif
					#if defined(WIN32) && !defined(WINCE)
					SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					#endif
				break;
			case DEBUGPRINT_WARNING:
					#if defined(WIN32) && !defined(WINCE)
					SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_GREEN);
					#endif
					printf("[W] %s", tx);
					#ifdef WINCE
					fprintf(lf, "[W] %s", tx);
					#endif
					#if defined(WIN32) && !defined(WINCE)
					SetConsoleTextAttribute(con, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					#endif
				break;
		}
		#ifdef WINCE
		fclose(lf);
		#endif
		va_end(vl);
	}
}

void DEBUGMARKER (unsigned int size, const char *val)
{
#if defined(WIN32) && defined(GREMDEY)
	if(glStringMarkerGREMEDY)
		glStringMarkerGREMEDY(size, val);
#endif
}
