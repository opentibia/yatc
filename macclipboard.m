#include "clipboard.h"

#include <Cocoa/Cocoa.h>
#include <Cocoa/NSPasteboard.h>

const char* getPasteboardText()
{
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSArray *classes = [[NSArray alloc] initWithObjects:[NSString class], nil];

    NSArray *copiedItems = [pasteboard readObjectsForClasses:classes];

    if(copiedItems != nil) {
        return [const char* [NSString [copiedItems objectAtIndex:0] UTF8String]];
    }
}
void putPasteBoardText(const char* text)
{
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    NSArray *objectsToCopy = [[NSArray alloc] initWithObjects:[NSString stringWithUTF8String:text], nil];

    [pasteboard writeObjects:objectsToCopy];
}
