#ifndef CURSORUTIL_H
#define CURSORUTIL_H

// If SDL is enabled, use the real implementation
#if !defined(CLI_ONLY)
#include <SDL/SDL.h>
void resetDefaultCursor();
#else
// Stub for non-SDL builds (tests, text UI, etc)
void resetDefaultCursor();
#endif

#endif // CURSORUTIL_H
