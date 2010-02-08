//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Utility functions
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

#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef WIN32
	#include <sys/stat.h> // mkdir
	#include <errno.h>
#endif
#include "util.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h> // needed for ShellExecute
#endif

#include "macutil.h"
#include "product.h"


#define WINCE_INSTALLDIR "/Storage Card/YATC/"

std::string str_replace(const std::string& what, const std::string& with, const std::string &src) {
	std::string::size_type p, l;
	std::string s = src;
	for (l = 0; (p = s.find(what, l)) != std::string::npos; l = p + with.size()) {
		s.replace(p, what.size(), with);
	}
	return s;
}


void NativeGUIError(const char* text, const char *title) {
	#ifdef __APPLE__
	printf("NativeGUIError('%s','%s');\n", text, title);
	#endif
	#ifdef WIN32
		#ifndef WINCE
			MessageBoxA(HWND_DESKTOP, text, title, MB_ICONSTOP);
		#else
			int a, b;

			BSTR unicodetext = SysAllocStringLen(NULL, a=strlen(text));
			BSTR unicodetitle = SysAllocStringLen(NULL, b=strlen(title));
			MultiByteToWideChar(CP_ACP, 0, text, a, unicodetext, a);
			MultiByteToWideChar(CP_ACP, 0, title, b, unicodetitle, b);



			MessageBox(HWND_DESKTOP, unicodetext, unicodetitle, MB_ICONSTOP);
			SysFreeString(unicodetext);
			SysFreeString(unicodetitle);

		#endif
	#else
		#ifdef __APPLE__
			macutil_NSRunAlertPanel(title, text, "OK", NULL, NULL	);
		#else

			std::string texts=text, titles=title;
			texts = str_replace("\\", "\\\\",texts);
			texts = str_replace("\"", "\\\"",texts);

			std::stringstream cmd;
			cmd << "xmessage -title \"" << title << "\" -center \"" << texts << "\"";

			int dummy = system(cmd.str().c_str());
			dummy = 0;
		#endif
	#endif
}


bool fileexists(const char* filename){
    FILE *f;
    if ((f = yatc_fopen(filename, "r"))) {
        fclose(f);
        return true;
    } else {
        return false;
	}
}

static bool __internal_fileexists(const char* filename){
    FILE *f;
    if ((f = fopen(filename, "r"))) {
        fclose(f);
        return true;
    } else
        return false;
}

static std::vector<std::string > searchpaths;


std::string yatc_findfile(const char* filename){
    for (std::vector<std::string>::iterator it = searchpaths.begin(); it != searchpaths.end(); it++) {
		std::string fn = (*it + "/" + filename).c_str();
		#ifndef WIN32
		if (fn[0] == '~')
			fn = std::string(getenv("HOME")) + "/" + fn.substr(1);
		#endif

		if (__internal_fileexists(fn.c_str())) {
			FILE *f=fopen(fn.c_str(), "r");

			if (f) {
			    fclose(f);
				return fn;
			}
			// if the above is false, we probably don't have enough permissions for requested mode
		}
	}
	FILE *f=fopen(filename, "r");
	if(f){
	    fclose(f);
	    return filename;
	}
	return "";
}

void yatc_fread(void* ptr, size_t size, size_t count, FILE *stream) {
	/*
	 * "warning: ignoring return value of ‘size_t fread(void*, size_t, size_t, FILE*)’, declared with attribute warn_unused_result"
	 */

	size_t dummy = fread(ptr, size, count, stream);
	dummy = 0;
}

void yatc_fwrite(void* ptr, size_t size, size_t count, FILE *stream) {
	/*
	 * "warning: ignoring return value of ‘fwrite’, declared with attribute warn_unused_result"
	 */

	size_t dummy = fwrite(ptr, size, count, stream);
	dummy = 0;
}

#ifndef WIN32
int yatc_makedirs(const char* path, int mode)
{
	char* tmppath = (char*)malloc(strlen(path)+1);
	for(const char* c = path; *c; c++)
	{
		if(*c=='/' && c-path > 0)
		{
			memcpy(tmppath,path,c-path);
			tmppath[c-path]=0;
			struct stat s;
			int ret = access(tmppath, 0); //&s);
			if(ret)
			{
				int mdret;
				if(mdret=mkdir(tmppath, mode))
				{
					free(tmppath);
					return mdret;
				}
			}
		}
	}
	return 0;
}
#endif

FILE *yatc_fopen(const char* filename, const char* mode) {
#ifdef WINCE
	/* ugly as hell, but WINCE's fopen does not use current working dir
	 * in its path ... it's also probably faster than using the
	 * searchpath.
	 * something also tells me SDL_LoadBMP() won't like this, but
	 * we'll be avoiding it anyway */

	char fopen_fn[255];
	sprintf(fopen_fn, "%s%s", WINCE_INSTALLDIR, filename);


	return fopen(fopen_fn, mode);
#else
	if (__internal_fileexists(filename)) {
		FILE *f = fopen(filename, mode);
		if (f)
			return f;
	}
	for (std::vector<std::string>::iterator it = searchpaths.begin(); it != searchpaths.end(); it++) {
		std::string fn = (*it + "/" + filename).c_str();
		#ifndef WIN32
		if (fn[0] == '~')
			fn = std::string(getenv("HOME")) + "/" + fn.substr(1);
		#endif

		if (__internal_fileexists(fn.c_str())) {
			FILE *f=fopen(fn.c_str(), mode);
			if (f) {
				return f;
			}
			// if the above is false, we probably don't have enough permissions for requested mode
		}
	}
	// if not found anywhere in the path, let's see what we can do with it
	#ifndef WIN32 // if these aren't windows, it's probably a unioxid; if not, we'll port later
	if (mode[0] == 'w' || mode[0] == 'a') {// if we're trying to access for writing
		std::string subdir = "/.yatc/";
		#ifdef __APPLE__
		subdir = "/Library/Application Support/OpenTibia/" PRODUCTSHORT "/";
		#endif

		std::string outfn = (std::string(getenv("HOME")) + subdir + filename);
		FILE* f = fopen(outfn.c_str(), mode);
		if(f)
			return f;
		printf("Trying to make cfg dir in home, fopen(...,w) was failing: %s\n", strerror(errno));
		if(yatc_makedirs((std::string(getenv("HOME")) + subdir).c_str(), 0700)){
			printf("Failed to make cfg dir: %s\n", strerror(errno));
		}
		else{
			f = fopen(outfn.c_str(), mode);
			if(f)
				return f;
		}
		printf("Still failed to write in home: %s\n", strerror(errno));
	}
	#endif

	// if we resume here, either we're under windows, or we're not attempting to open for writing


	return fopen(filename, mode);
#endif
}

#ifndef DESTDIRS
	#ifndef __APPLE__
		#define DESTDIRS ""
	#else
		#define DESTDIRS "Resources/"
	#endif
#endif

void yatc_fopen_init(char *cmdline) {
#if (HAVE_GETENV==1)
	const char *searchpath = getenv("YATC_PATH");
#else
	printf("Getenv() not supported; leaving searchpath empty. (Perhaps HAVE_GETENV isn't defined)\n");
	const char *searchpath = NULL;
#endif
	//char temp[250];
	const char *lp;
	if (!searchpath)
	searchpath =
	#ifndef WIN32
	#ifdef __APPLE__
		"~/Library/Application Support/OpenTibia/" PRODUCTSHORT ":"
	#endif
		"~/.yatc/:"
		"/usr/share/games/yatc-data/:"
		"/usr/share/games/tibia/:"
	#endif
		DESTDIRS;

	lp = searchpath;
	for (const char* p=searchpath; ; p++) {
		if (*p==':' || *p==0) {
			char *a = new char[p-lp + 1];
			memcpy(a, lp, p-lp);
			a[p-lp] = 0;
			if (strlen(a)) {
				printf("Adding %s to search path\n", a);
				searchpaths.insert(searchpaths.end(),std::string(a));
			}
			delete []a;
			lp = p+1;
			if (!*p)
				break;
		}
	}
	if (cmdline) {
		for (int i = strlen(cmdline)-1; i >= 0; i--) {
			if (cmdline[i] == '/') {
				char *tmp = new char[i+2];
				memcpy(tmp, cmdline, i+1);
				tmp[i+1]=0;
				searchpaths.insert(searchpaths.end(), std::string(tmp));
				printf("Adding extra path %s\n", tmp);
#ifdef __APPLE__
				std::string tmp2 = std::string(tmp) + "../Resources/";
				printf("Also adding Mac bundle resource path %s\n", tmp2.c_str());
				searchpaths.insert(searchpaths.end(), tmp2);
#endif
				delete[] tmp;
				break;
			}
		}
	} else {
        printf("cmdline: null, can't be used for yatc_fopen_init()\n");
	}
}


#if defined(WINCE) && defined(_MSC_VER)
time_t time( time_t *inTT )
{
	SYSTEMTIME sysTimeStruct;
	FILETIME fTime;
	ULARGE_INTEGER int64time;
	time_t locTT = 0;

	if ( inTT == NULL ) {
		inTT = &locTT;
	}

	GetSystemTime( &sysTimeStruct );
	if ( SystemTimeToFileTime( &sysTimeStruct, &fTime ) ) {
		memcpy( &int64time, &fTime, sizeof( FILETIME ) );
		/* Subtract the value for 1970-01-01 00:00 (UTC) */
		int64time.QuadPart -= 0x19db1ded53e8000;
		/* Convert to seconds. */
		int64time.QuadPart /= 10000000;
		*inTT = (time_t)int64time.QuadPart;
	}
	return *inTT;
}
#endif



void
openurl(const char *url)
{
    // lifted off http://www.archivum.info/sdl@lists.libsdl.org/2007-10/msg00078.html
    // from a post by Gabriele Greco
    // will use later on
#ifdef WIN32
    ShellExecuteA(GetActiveWindow(),
         "open", url, NULL, NULL, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "open %s", url);
    system(buffer);
#else
    char *apps[] = {"x-www-browser",
                    "firefox", // iceweasel has an alias on debian
                    "opera",
                    "mozilla",
                    "galeon",
                    "konqueror", NULL};

    char buffer[256];
    int i = 0;

    while (apps[i]) {
        snprintf(buffer, sizeof(buffer), "which %s >/dev/null", apps[i]);
        if (system(buffer) == 0) {
            snprintf(buffer, sizeof(buffer), "%s %s", apps[i], url);
            int dummy = system(buffer);
            dummy = 0;
            return;
        }
        i++;
    }
#endif

}









// hex printing code:
/*for(int i = 0; i < to-from; i+=8)
 {
 
 if(i % 16 == 0)
 {
 printf("\t");
 
 for(int j = i-16; j < i; j++)
 {
 if(i<0)break;
 printf("%c", isprint(memory[from+j]) ? memory[from+j] : '.');
 }
 printf("\n%08X\t\t", from+i);
 }
 else
 {
 printf("\t");
 }
 
 int adr = 0;
 for(int j = 0; j < 8 && adr < to; j++)
 {
 adr = from + i + j;
 printf("%02hhX ", memory[adr]); // hhX -- forsira da koristimo cetvrt-int, tj. half-half-int.
 }
 
 }*/
