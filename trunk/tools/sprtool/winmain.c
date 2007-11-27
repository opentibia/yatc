/* SDL compatibility layer for Windows
 * original code by Davor Cihlar
 * Adapted for SDL use by Ivan Vucica
 * Licensed under GNU General Public License
 *
 * This file should only be used on Windows.
 * It's here since -lmingw32 -lSDLmain method didn't produce
 * any output in the console (for me) when building with DevCpp's MinGW.
 * They probably messed up stdout.
 */
#include <windows.h>
#include <stdlib.h>
#define MAX_EXE_NAME_LEN	256
#define MAX_ARGS	200

int SDL_main(int argn, char **argv);


static int _parse_args(char *arg_buf, char ***arg_list, char *exename) {
	int i=0;

	*arg_list = (char**)malloc(sizeof(char*)*2);

	if (exename) (*arg_list)[i++] = exename;

	if (!*arg_buf) return i;

	(*arg_list)[i++] = arg_buf;
	for (;*arg_buf;++arg_buf) {
		if (*arg_buf != ' ') continue;
		while (*arg_buf == ' ') *(arg_buf++) = 0;
		if (!*arg_buf) break;
		*arg_list = (char**)realloc(*arg_list, sizeof(char*)*(i+1));
		if (*arg_buf == '"') {
			(*arg_list)[i++] = arg_buf++;
			while (*arg_buf && *arg_buf != '"');
			if (!*arg_buf) break;
			*arg_buf = 0;
		} else {
			(*arg_list)[i++] = arg_buf;
		}
		if (i >= MAX_ARGS) break;
	}
	return i;
}

static char *_get_exe_name() {
	char *exename = malloc(MAX_EXE_NAME_LEN);
	if (!GetModuleFileName(GetModuleHandle(NULL), exename, MAX_EXE_NAME_LEN)) {
		free(exename);
		return NULL;
	}

	return exename;
}

int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow
    )
{
	int r, argn;
	char *cmd, **argv;
	char *exename;

	cmd = lpCmdLine;
	exename = _get_exe_name();

	argn = _parse_args(cmd, &argv, exename);

	r = SDL_main(argn, argv);

	if (exename) free(exename);
	free(argv);
//	free(cmd);

	return r;
}
