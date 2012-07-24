#include <Cocoa/Cocoa.h>
#include "macutil.h"
int macutil_NSRunAlertPanel(const char* title, const char* text, const char *btn1, const char *btn2, const char *btn3,...)
{
	return 
	NSRunAlertPanel([NSString stringWithUTF8String:title], 
					[NSString stringWithUTF8String:text],
					btn1 ? [NSString stringWithUTF8String:btn1] : nil, 
					btn2 ? [NSString stringWithUTF8String:btn2] : nil,
					btn3 ? [NSString stringWithUTF8String:btn3] : nil
					/*, args for a printf-style msg go here */);
}