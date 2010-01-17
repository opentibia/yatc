#include "clipboard.h"

#include <Cocoa/Cocoa.h>
#include <Cocoa/NSPasteboard.h>

#include <stdlib.h>
#include <string.h>

const char* getPasteboardText()
{
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    NSString* string = [pasteboard stringForType:NSStringPboardType];
    const char* cstring = [string cString];
    char* ret = (char*)malloc(strlen(cstring));
    strcpy(ret, cstring);
    return ret;
}

void putPasteBoardText(const char* text)
{
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    NSString* string = [NSString stringWithCString:text];
    [pasteboard clearContents];
    [pasteboard setString:string forType:NSStringPboardType];
}
