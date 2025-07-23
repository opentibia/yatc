#include "cursorutil.h"

#if !defined(CLI_ONLY)
void resetDefaultCursor()
{
    static SDL_Cursor* defaultcursor = NULL;
    if (!defaultcursor)
        defaultcursor = SDL_GetCursor();
    else
        SDL_SetCursor(defaultcursor);
}
#else
void resetDefaultCursor()
{
}
#endif
